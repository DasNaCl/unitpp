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

#include <detail/measure.hpp>
#include <detail/add_exponents.hpp>
#include <detail/uncompoundify.hpp>
#include <compoundify.hpp>

namespace unitpp
{
  template<bool shall_uncompoundify, class... Args>
  struct simplify<shall_uncompoundify, tmpl::X<Args...>>
  {
    using hd = tmpl::head<tmpl::X<Args...>>;
    using tl = tmpl::tail<tmpl::X<Args...>>;

    template<class B>
    using my_same_kind = detail::is_same_kind<hd, B>;

    template<detail::MeasureType t, class m, class l>
    struct intermediate_simplify
    {
      using added = add_exponents_t<hd, tmpl::X<Args...>>;
      using type = tmpl::cons<added, typename simplify<shall_uncompoundify, me::filter_t<my_same_kind, tl>>::type>;
    };

    template<class m, class l>
    struct intermediate_simplify<detail::MeasureType::Compound, m, l>
    {
      using new_list = tmpl::conc<l, detail::uncompoundify<tmpl::X<m>>>;
      using type = typename simplify<shall_uncompoundify, new_list>::type;
    };

    using type = typename intermediate_simplify<(shall_uncompoundify ? hd::type : detail::MeasureType::Base),
                                                 hd, tl>::type;
  };

  template<bool shall_uncompoundify>
  struct simplify<shall_uncompoundify, tmpl::X<>>
  {
    using type = tmpl::X<>;
  };

  template<class L, bool shall_uncompoundify>
  using simplify_helper_t = tmpl::filter_t<detail::is_exp_null,
                              typename simplify<shall_uncompoundify, tmpl::recursive_union<L>>::type>;

  template<class L, class Default, bool shall_compoundify = false, bool shall_uncompoundify = true>
  using simplify_t = std::conditional_t<std::is_same_v<simplify_helper_t<L, shall_uncompoundify>, tmpl::X<>>,
                                        Default,
                                        compoundifier<simplify_helper_t<L, shall_uncompoundify>, shall_compoundify>>;
}
