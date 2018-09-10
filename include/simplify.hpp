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

#include <detail/compoundify.hpp>
#include <detail/simplify.hpp>

namespace unitpp
{

  template<class L, bool shall_uncompoundify>
  using simplify_helper_t = tmpl::filter_t<is_exp_null,
                              typename detail::simplify<shall_uncompoundify, tmpl::recursive_union_t<L>>::type>;

  template<class L, class Default, bool shall_compoundify = false, bool shall_uncompoundify = true>
  using simplify_t = std::conditional_t<std::is_same_v<simplify_helper_t<L, shall_uncompoundify>, tmpl::X<>>,
                                        Default,
                                        detail::compoundifier<simplify_helper_t<L, shall_uncompoundify>, shall_compoundify>>;
}
