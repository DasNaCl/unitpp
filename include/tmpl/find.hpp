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

#include <detail/is_detected.hpp>
#include <tmpl/list.hpp>

namespace unitpp
{
namespace tmpl
{
  template<template<class> class P, class H>
  struct find
  {
    static_assert(tmpl::is_list_v<H>, "Can't find anything in a non-list");
  };

  namespace detail
  {
    template<template<class> class P, class... H>
    struct find_check
    {
      template<class T>
      using has_value_field = decltype(P<T>::value);

      static_assert(unitpp::detail::is_detected_v<has_value_field, tmpl::head<tmpl::X<H...>>>,
                    "P has no value field");
    };
  }

  template<template<class> class P, class... H>
  struct find<P, X<H...>> : public detail::find_check<P, H...>,
                            public std::conditional_t<P<head<X<H...>>>::value,
                                                      std::true_type,
                                                      typename find<P, tail<X<H...>>>::type>
  {
    using object = std::conditional_t<P<head<X<H...>>>::value,
                                      head<X<H...>>,
                                      typename find<P, tail<X<H...>>>::object>;
  };

  template<template<class> class P>
  struct find<P, X<>> : public std::false_type
  { struct none {}; using object = none; };

  template<template<class> class P, class H>
  inline constexpr bool find_v = find<P, H>::value;

  template<template<class> class P, class H>
  using find_t = typename find<P, H>::object;

  template<class E, class... H>
  struct find_same
  {
    template<class T>
    using is_same_trait = std::is_same<E, T>;

    inline static constexpr bool value = find_v<is_same_trait, X<H...>>;
  };

  template<class E, class... H>
  inline constexpr bool find_same_v = find_same<E, H...>::value;

  template<class H>
  using find_list = find<is_list, H>;

  template<class H>
  inline constexpr bool find_list_v = find_list<H>::value;
}
}
