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

#include <type_traits>

namespace unitpp
{
namespace tmpl
{
  template<class... T>
  struct X;

namespace detail
{
  template<class H>
  struct head_tail_helper;
  
  template<class H, class... Tail>
  struct head_tail_helper<X<H, Tail...>>
  { using head = H; using tail = X<Tail...>; };
}
  template<class H>
  using head = typename detail::head_tail_helper<H>::head;
  template<class H>
  using tail = typename detail::head_tail_helper<H>::tail;

namespace detail
{
  template<class H, class T>
  struct cons_helper;

  template<class H, class... Args>
  struct cons_helper<H, X<Args...>>
  { using type = X<H, Args...>; };

  template<class H, class T>
  struct conc_helper;

  template<class... Args1, class... Args2>
  struct conc_helper<X<Args1...>, X<Args2...>>
  { using type = X<Args1..., Args2...>; };
}
  template<class H, class L>
  using cons = typename detail::cons_helper<H, L>::type;
  
  template<class A, class B>
  using conc = typename detail::conc_helper<A, B>::type;

  template<class T>
  struct is_list : public std::false_type
  {  };

  template<class... Args>
  struct is_list<X<Args...>> : public std::true_type
  {  };
  
  template<class T>
  inline constexpr bool is_list_v = is_list<T>::value;

  template<class T>
  struct length
  { static constexpr std::size_t value = 1 + length<tail<T>>::value; };

  template<>
  struct length<X<>>
  { static constexpr std::size_t value = 0; };

  template<class T>
  inline constexpr std::size_t length_v = length<T>::value;

  template<class T>
  inline constexpr std::size_t is_list_empty_v = (length_v<T> == 0);
}
}
