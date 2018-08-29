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

namespace me
{

///// A 'generic' unit
template<typename L, typename T = float>
struct unit
{
  using Units = L;
  using Default = T;

  static_assert(me::is_list_v<L>, "L should be a list of type me::X.");

  constexpr unit() = default;
  constexpr unit(T v) : val(v)
  {  }
private:
  T val;
};


///// 
template<class T>
using simplify_unit_t = std::conditional_t<std::is_same_v<simplify_t<typename T::Units, typename T::Default>, typename T::Default>,
                                           typename T::Default,
                                           unit<simplify_t<typename T::Units, typename T::Default>, typename T::Default>
                                          >;

}

