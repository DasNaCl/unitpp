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

#include <tmpl/filter.hpp>

namespace unitpp
{
namespace tmpl
{
  template<template<class, class> class R, class L>
  struct sort
  {
    static_assert(is_list_v<L>, "Can't sort a non-list");
  };

  template<template<class, class> class R, class... Args>
  struct sort<R, X<Args...>>
  {
    using pivot = head<X<Args...>>;

    template<class U>
    using is_smaller = R<U, pivot>;

    template<class U>
    using isnt_smaller = std::integral_constant<bool, !R<U, pivot>::value>;

    // determine left and right list
    using left = filter_t<isnt_smaller, tail<X<Args...>>>;
    using right = filter_t<is_smaller, tail<X<Args...>>>;

    using type = conc<typename sort<R, left>::type,
                  conc<X<pivot>,
                        typename sort<R, right>::type>>;
  };

  template<template<class, class> class R>
  struct sort<R, X<>>
  {
    using type = X<>;
  };

  template<template<class, class> class R, class L>
  using sort_t = typename sort<R, L>::type;
}
}
