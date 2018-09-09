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

#include <tmpl/list.hpp>

namespace unitpp
{
  template<class L>
  struct exists_compound
  {
    static_assert(tmpl::is_list_v<L>, "Expecting a list of compounds");
  };

  template<class... Kind>
  struct exists_compound<tmpl::X<Kind...>> : public std::false_type
  {
    using type = tmpl::X<Kind...>;
  };

  template<class List>
  constexpr bool exists_compound_v = exists_compound<List>::value;

  template<class List>
  using exists_compound_t = typename exists_compound<List>::type;
}
