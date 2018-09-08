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
namespace detail
{
  template<template<class ...> class T, class E, class... Args>
  struct is_detected_helper : public std::false_type
  {  };

  template<template<class ...> class T, class... Args>
  struct is_detected_helper<T, std::void_t<T<Args...>>, Args...> : std::true_type
  {  };

  template<template<class ...> class T, class... Args>
  using is_detected = typename is_detected_helper<T, void, Args...>::type;

  template<template<class ...> class T, class... Args>
  constexpr bool is_detected_v = is_detected<T, Args...>::value;
}
}
