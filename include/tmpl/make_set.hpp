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

#include <tmpl/eliminate.hpp>

namespace unitpp
{
namespace tmpl
{
  template<class H>
  struct make_set
  {
    static_assert(is_list_v<H>, "Can't make a set out of a non-list");
  };

  template<class... H>
  struct make_set<X<H...>>
  {
    using type = cons<head<X<H...>>,
                      typename make_set<eliminate_t<head<X<H...>>, X<H...>>>::type>;
  };

  template<>
  struct make_set<X<>>
  {
    using type = X<>;
  };

  template<class H>
  using make_set_t = typename make_set<H>::type;
}
}
