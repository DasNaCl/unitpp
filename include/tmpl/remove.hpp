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
  template<class E, class H>
  struct remove
  {
    static_assert(is_list_v<H>, "Can't remove something from a non-list");
  };

  template<class E, class... H>
  struct remove<E, X<H...>>
  {
    using type = std::conditional_t<std::is_same<E, head<X<H...>>>::value,
                                    tail<X<H...>>,
                                    cons<head<X<H...>>,
                                         typename remove<E, tail<X<H...>>>::type>
                                   >;
  };

  template<class E>
  struct remove<E, X<>>
  {
    using type = X<>;
  };

  template<class E, class H>
  using remove_t = typename remove<E, H>::type;
}
}
