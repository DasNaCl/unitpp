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
  template<std::size_t n, class T>
  struct erase
  { static_assert(tmpl::is_list_v<T>, "Can't erase something out of a non-list"); };

  template<std::size_t n, class... T>
  struct erase<n, X<T...>>
  {
    static_assert(n < length_v<X<T...>>, "n should be smaller than the length of the list");

    using type = cons<head<X<T...>>, typename erase<n - 1, tail<X<T...>>>::type>;
  };
  template<class... T>
  struct erase<0, X<T...>>
  {
    using type = tail<X<T...>>;
  };

  template<std::size_t n, typename T>
  using erase_t = typename erase<n, T>::type;
}
}
