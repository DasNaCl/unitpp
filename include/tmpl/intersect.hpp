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

#include <tmpl/remove.hpp>

namespace unitpp
{
namespace tmpl
{
  template<class H1, class H2>
  struct intersect
  {
    static_assert(is_list_v<H1> && is_list_v<H2>, "Can't intersect with a non-list");
  };

  template<class... H1, class... H2>
  struct intersect<X<H1...>, X<H2...>>
  {
    using type = typename intersect<tail<X<H1...>>, remove_t<head<X<H1...>>, X<H2...>>>::type;
  };

  template<class... H2>
  struct intersect<X<>, X<H2...>>
  {
    using type = X<H2...>;
  };

  template<class H1, class H2>
  using intersect_t = typename intersect<H1, H2>::type;
}
}
