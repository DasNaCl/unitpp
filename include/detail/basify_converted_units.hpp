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

#include <tmpl/recursive_union.hpp>
#include <measure.hpp>

namespace unitpp
{
namespace detail
{
  template<class>
  struct basify_converted_units;

  template<class... Args>
  struct basify_converted_units<tmpl::X<Args...>>
  {
    using hd = tmpl::head<tmpl::X<Args...>>;

    template<class H>
    struct helper
    { using type = H; };

    template<class H, int n>
    struct helper<measure<MeasureType::Converted, H, n>>
    {
      using type = typename H::Base;
    };

    using type = tmpl::conc<tmpl::X<typename helper<hd>::type>,
                            typename basify_converted_units<tmpl::tail<tmpl::X<Args...>>>::type>;
  };

  template<>
  struct basify_converted_units<tmpl::X<>>
  {
    using type = tmpl::X<>;
  };

  // todo: support converted compound units
  template<class L>
  using basify_converted_units_t = typename basify_converted_units<tmpl::recursive_union_t<L>>::type;
}
}
