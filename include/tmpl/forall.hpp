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
  struct forall
  {
    static_assert(tmpl::is_list_v<H>, "Can't find anything in a non-list");
  };

  namespace detail
  {
    template<template<class> class P, class... H>
    struct forall_check
    {
      template<class T>
      using has_value_field = decltype(P<T>::value);

      static_assert(unitpp::detail::is_detected_v<has_value_field, tmpl::head<tmpl::X<H...>>>,
                    "P has no value field");
    };
  }

  template<template<class> class P, class... H>
  struct forall<P, X<H...>> : public detail::forall_check<P, H...>,
                              public std::conditional_t<!P<head<X<H...>>>::value,
                                                        std::false_type,
                                                        typename forall<P, tail<X<H...>>>::type>
  {  };

  template<template<class> class P>
  struct forall<P, X<>> : public std::true_type
  {  };

  template<template<class> class P, class H>
  inline constexpr bool forall_v = forall<P, H>::value;
}
}
