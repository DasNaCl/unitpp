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

#include <tmpl/make_set.hpp>
#include <tmpl/intersect.hpp>

namespace unitpp
{
namespace tmpl
{
  template<class H1, class H2>
  struct same_elements
  {
    static_assert(is_list_v<H1> && is_list_v<H2>, "Can't compare to non-lists depending on their elements");
  };

  template<class... H1, class... H2>
  struct same_elements<X<H1...>, X<H2...>>
  {
    using set1 = make_set_t<X<H1...>>;
    using set2 = make_set_t<X<H2...>>;

    using type1 = intersect_t<set1, set2>;
    using type2 = intersect_t<set2, set1>;

    static constexpr const bool value = std::is_same<type1, type2>::value;
  };

  template<typename A, typename B>
  inline constexpr bool same_elements_v = same_elements<A, B>::value;
}
}
