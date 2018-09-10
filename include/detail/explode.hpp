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
namespace detail
{
  template<class T>
  struct explode_single
  {
    static_assert(is_measure_v<T>, "Can't explode a non-unit");

    constexpr static int sign = (T::exponent > 0 ? 1 : -1);
    constexpr static int exp = sign * T::exponent; // <- sneaky way to get the abs
    using M = measure<T::type, typename T::kind, sign>;
    using R = measure<T::type, typename T::kind, sign * (exp - 1)>;

    template<int e, class>
    struct helper
    {
      using type = tmpl::cons<M, typename explode_single<R>::type>;
    };
    template<class Junk>
    struct helper<0, Junk>
    {
      using type = tmpl::X<>;
    };

    using type = typename helper<exp, void>::type;
  };
  template<class T>
  using explode_single_t = typename explode_single<T>::type;

  template<class T>
  struct explode
  {
    static_assert(tmpl::is_list_v<T>, "Can't explode a non-list");
  };

  template<class... Args>
  struct explode<tmpl::X<Args...>>
  {
    using type = tmpl::conc<explode_single_t<tmpl::head<tmpl::X<Args...>>>,
                            typename explode<tmpl::tail<tmpl::X<Args...>>>::type>;
  };

  template<>
  struct explode<tmpl::X<>>
  {
    using type = tmpl::X<>;
  };

  template<class T>
  using explode_t = typename explode<T>::type;
}
}
