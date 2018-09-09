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
  struct nth
  {
    static_assert(tmpl::is_list_v<T>, "Can't get the nth position out of a non-list");
    static_assert(n < tmpl::length_v<T>, "Can't yield a position behind list's size");
  };

  template<std::size_t n, class... T>
  struct nth<n, X<T...>>
  {
    using type = typename nth<n - 1, tail<X<T...>>>::type;
  };
  template<class... T>
  struct nth<0, X<T...>>
  {
    using type = head<X<T...>>;
  };

  template<int n, typename T>
  using nth_t = typename nth<n, T>::type;
}
}
