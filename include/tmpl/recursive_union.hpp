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
namespace tmpl
{
  template<class T>
  struct recursive_union
  { using type = T; };//void; /*ugly*/ };

  template<typename... T>
  struct recursive_union<X<T...>>
  {
    using hd = head<X<T...>>;
    using e = std::conditional_t<is_list<hd>::value,
                                 typename recursive_union<hd>::type, X<hd>>;

    using type = conc<e, typename recursive_union<tail<X<T...>>>::type>;
  };

  template<>
  struct recursive_union<X<>>
  {
    using type = X<>;
  };

  template<class T>
  using recursive_union_t = typename recursive_union<T>::type;
}
}
