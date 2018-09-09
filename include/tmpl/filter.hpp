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
  template<template<class> class R, class L>
  struct filter;

  template<template<class> class R, class... Args>
  struct filter<R, X<Args...>>
  {
    // if true, filter out
    using elem = std::conditional_t<!R<head<X<Args...>>>::value, X<head<X<Args...>>>, X<>>;

    using type = conc<elem, typename filter<R, tail<X<Args...>>>::type>;
  };

  template<template<class> class R>
  struct filter<R, X<>>
  {
    using type = X<>;
  };

  template<template<class> class R, class U>
  using filter_t = typename filter<R, U>::type;
}
}
