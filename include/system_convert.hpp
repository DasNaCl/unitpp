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

#include <measure.hpp>

namespace unitpp
{
  template<class From, class To>
  struct system_convert_check
  {
    static_assert(is_measure_v<From>, "From should be a measure");
    static_assert(is_measure_v<To>, "To should be a measure");

    // make sure you only get measures here
  };
  template<class From, class To>
  struct system_convert : public std::false_type
  {
    using type = To;

    template<typename D = std::false_type>
    constexpr void operator()(To)
    { static_assert(D::value, ""); }
  };

  // can be used for e.g. meter to foot
#define unitpp_declare_system_convert(From, To, var)                                           \
  template<>                                                                                     \
  struct system_convert<From, To> : public std::true_type, public system_convert_check<From, To> \
  {                                                                                              \
    using type = To;                                                                             \
    template<class T>                                                                            \
    constexpr T operator()(T) const;                                                             \
  };                                                                                             \
  template<> constexpr To system_convert<From, To>::operator()(To var) const

  template<typename T>
  struct system_convert<T, T> : public std::true_type, public system_convert_check<T, T>
  {
    using type = T;

    template<class D>
    constexpr D operator()(D&& id) const
    { return id; }
  };

  template<class From, class To>
  constexpr bool exists_system_convert_v = system_convert<From, To>::value;
}
