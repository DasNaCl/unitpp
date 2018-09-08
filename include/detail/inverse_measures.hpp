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

#include <detail/measure.hpp>
#include <tmpl/list.hpp>

namespace unitpp
{
namespace detail
{
  template<class L>
  struct inverse_measures;

  template<class... Args>
  struct inverse_measures<tmpl::X<Args...>>
  {
    using hd = typename tmpl::head<tmpl::X<Args...>>;
    using tl = tmpl::tail<tmpl::X<Args...>>;

    static_assert(is_measure_v<hd>, "Can't inverse the exponent of a non-measure");

    using type = tmpl::cons<measure<hd::type, typename hd::kind, -1 * hd::exponent>,
                            typename inverse_measures<tl>::type>;
  };
  template<>
  struct inverse_measures<tmpl::X<>>
  {
    using type = tmpl::X<>;
  };

  template<class L>
  using inverse_measures_t = typename inverse_measures<L>::type;
}
}
