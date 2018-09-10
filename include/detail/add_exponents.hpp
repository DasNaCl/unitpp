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
  template<class T, class L>
  struct add_exponents
  {
    static_assert(tmpl::is_list_v<L>, "Expecting a list of measures");
  };

  template<class T, class... Args>
  struct add_exponents<T, tmpl::X<Args...>>
  {
    using rec = typename add_exponents<T, tmpl::tail<tmpl::X<Args...>>>::type;

    template<class A, class B>
    struct add
    {
      static_assert(is_measure_v<A>, "A should be a measure");
      static_assert(is_measure_v<B>, "B should be a measure");

      using type = measure<A::type,
                           typename A::kind,
                           A::exponent + B::exponent>;
    };
    template<class A, class B>
    using add_t = typename add<A, B>::type;

    using hd = tmpl::head<tmpl::X<Args...>>;
    using type = std::conditional_t<is_same_kind_v<T, hd>,
                                    add_t<hd, rec>, rec>;
  };

  template<class T>
  struct add_exponents<T, tmpl::X<>>
  {
    // Fallback to single unit
    using type = measure<MeasureType::Base, T, 0>;
  };

  template<class T, class E>
  using add_exponents_t = typename add_exponents<T, E>::type;
}
}
