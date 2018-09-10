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

#include <tmpl/recursive_union.hpp>
#include <measure.hpp>

namespace unitpp
{
namespace detail
{
  template<class T>
  struct uncompoundify_helper
  {
    static_assert(tmpl::is_list_v<T>, "Expecting a list for uncompoundify");
  };

  template<typename... D>
  struct uncompoundify_helper<tmpl::X<D...>>
  {
    template<class H, int val>
    struct helper
    { using type = H; /*ugly*/ };

    template<class... T, int exp>
    struct helper<tmpl::X<T...>, exp>
    {
      using hd = tmpl::head<tmpl::X<T...>>;

      using obj = std::conditional_t<is_compound_measure_v<hd>,
                                     typename helper<typename hd::kind, exp * hd::exponent>::type,
                                     tmpl::X<measure<hd::type, typename hd::kind, exp * hd::exponent>>>;
      using type = tmpl::conc<obj, typename helper<tmpl::tail<tmpl::X<T...>>, exp>::type>;
    };
    template<int exp>
    struct helper<tmpl::X<>, exp>
    {
      using type = tmpl::X<>;
    };

    using type = typename helper<tmpl::X<D...>, 1>::type;
  };

  template<>
  struct uncompoundify_helper<tmpl::X<>>
  {
    using type = tmpl::X<>;
  };

  template<class T>
  using uncompoundify = typename uncompoundify_helper<tmpl::recursive_union_t<T>>::type;
}
}
