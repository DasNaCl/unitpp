/*
   Copyright 2018 Matthis Kruse

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#pragma once 

#include <tmpl/sort.hpp>
#include <tmpl/erase.hpp>
#include <tmpl/nth.hpp>
#include <detail/exists_compound.hpp>
#include <detail/simplify.hpp>
#include <detail/explode.hpp>
#include <measure.hpp>

namespace unitpp
{
namespace detail
{
  template<class T>
  struct compoundify;

  namespace hidden
  {
    template<class A, class B>
    using kind_cmp = std::bool_constant<(A::id < B::id)>;

    template<class A, class B>
    using base_cmp = std::bool_constant<(A::base_id < B::base_id)>;

    template<class L>
    using compoundify_op = explode_t<tmpl::sort_t<kind_cmp, L>>;

    // do the compoundify check if some compound for a given thing exists
    template<class T>
    struct check
    {
      using sorted = typename simplify<false, T>::type;
      using new_list = tmpl::sort_t<kind_cmp, sorted>;

      // either this is shortened or the same as before
      using intermediate = unitpp::exists_compound_t<new_list>;

      constexpr static bool value = exists_compound_v<new_list>;
      using type = compoundify_op<std::conditional_t<value, intermediate, new_list>>;
    };

    template<class List, int chunk>
    struct single_rm_A;
    template<class List, int chunk, std::size_t rm, bool condition>
    struct single_rm_B;

    template<int chunk, class W>
    struct single_rm_E
    {
      using tmp = single_rm_A<W, chunk - 1>;

      using type = typename tmp::type;
      constexpr static bool value = tmp::value;
    };
    template<class W>
    struct single_rm_E<0, W>
    {
      using tmp = check<W>;

      using type = typename tmp::type;
      constexpr static bool value = tmp::value;
    };

    template<class List, int chunk, std::size_t rm, class W>
    struct single_rm_D
    {
      using bail_out = single_rm_E<chunk, W>;

      template<class Garbage, bool v>
      struct helper
      {
        struct R
        {
          constexpr static bool value = true;
          using type = compoundify_op<tmpl::cons<tmpl::nth_t<rm, List>, typename bail_out::type>>;
        };
        using type = R;
      };
      template<class Garbage>
      struct helper<Garbage, false>
      {
        using type = single_rm_B<List, chunk, rm + 1, ((rm + 1) < tmpl::length_v<List>)>;
      };

      using tmp = typename helper<void, bail_out::value>::type;

      constexpr static bool value = tmp::value;
      using type = typename tmp::type;
    };

    template<class List, int chunk, std::size_t rm, bool condition>
    struct single_rm_C
    {
      using w = tmpl::erase_t<rm, List>;

      using tmp = single_rm_D<List, chunk, rm, w>;

      using type = typename tmp::type;
      constexpr static bool value = tmp::value;
    };
    template<class List, int chunk, std::size_t rm>
    struct single_rm_C<List, chunk, rm, false>
    {
      using tmp = single_rm_B<compoundify_op<List>, chunk, rm + 1, (rm + 1 < tmpl::length_v<compoundify_op<List>>)>;

      using type = typename tmp::type;
      constexpr static bool value = tmp::value;
    };


    template<class List, int chunk, std::size_t rm, bool condition>
    struct single_rm_B
    {
      using tmp = single_rm_C<compoundify_op<List>, chunk, rm, (tmpl::length_v<compoundify_op<List>> > 1)>;

      using type = typename tmp::type;
      constexpr static bool value = tmp::value;
    };
    template<class List, int chunk, std::size_t rm>
    struct single_rm_B<List, chunk, rm, false>
    {
      using type = List;
      constexpr static bool value = false;
    };

    template<class List, int chunk>
    struct single_rm_A // recurse
    {
      using res_t = single_rm_B<compoundify_op<List>, chunk, 0, (0 < tmpl::length_v<compoundify_op<List>>)>;

      using type = typename res_t::type;
      constexpr static bool value = res_t::value;
    };
    template<class List>
    struct single_rm_A<List, -1> // recursion stop
    {
      using tmp = check<List>;

      struct res
      {
        constexpr static bool value = true;
        using type = typename tmp::type;
      };
      template<class H>
      constexpr static auto f()
        -> std::enable_if_t<H::value, res>;

      template<class H>
      constexpr static auto f()
        -> std::enable_if_t<!H::value, single_rm_A<compoundify_op<List>, 0>>;

      using res_t = decltype(f<tmp>());
      
      using type = typename res_t::type;
      constexpr static bool value = res_t::value;
    };
  }

  template<typename... T>
  struct compoundify<tmpl::X<T...>>
  {
    template<int chunk, bool loop_cond, class List>
    struct helper
    {
      using new_t = hidden::single_rm_A<hidden::compoundify_op<List>, chunk>;

      constexpr static bool bail_out = new_t::value;
      constexpr static int next_chunk = (bail_out ? -1 : (chunk + 1));

      using new_list = typename new_t::type;
      using type = typename helper<next_chunk,
                                   (static_cast<unsigned int>(next_chunk) < tmpl::length_v<new_list>),
                                   hidden::compoundify_op<new_list>>::type;
    };
    template<int chunk, class List>
    struct helper<chunk, false, List>
    {
      using type = tmpl::sort_t<hidden::kind_cmp, List>;
    };

    using tmp = typename helper<-1, true, tmpl::X<T...>>::type;
    using sorted = typename simplify<false, tmp>::type;

    using type = sorted;
  };

  template<>
  struct compoundify<tmpl::X<>>
  {
    using type = tmpl::X<>;
  };

  template<typename L>
  using compoundify_t = typename compoundify<hidden::compoundify_op<L>>::type;

  template<class L, bool condition>
  using compoundifier = std::conditional_t<condition, compoundify_t<L>, L>;
}
}
