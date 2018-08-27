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

#include <unit.hpp>

using namespace me;

namespace si
{

struct meter {  };
struct kilogram {  };
struct second {  };

template<int n = 1>
using Kilogram = measure<MeasureType::Single, kilogram, n>;

template<int n = 1>
using Meter = measure<MeasureType::Single, meter, n>;

template<int n = 1>
using Second = measure<MeasureType::Single, second, n>;

template<int n = 1>
using Newton = measure<MeasureType::Compound,
                       me::X<Kilogram<1>,
                             Meter<1>,
                             Second<-2>>, n>;

template<int n = 1>
using Joule1 = measure<MeasureType::Compound,
                       me::X<Kilogram<1>,
                             Meter<2>,
                             Second<-2>>, n>;

template<int n = 1>
using Joule2 = measure<MeasureType::Compound, me::X<Newton<1>, Meter<1>>, n>;

}

void test_simplify()
{
  using namespace si;

  static_assert(std::is_same_v<simplify_unit_t<
                                                unit<me::X<>>
                                              >,
                               float>,
                "Empty list should simplify");

  static_assert(std::is_same_v<simplify_unit_t<
                                            unit<me::X<
                                                   me::X<
                                                        me::X<>
                                                        >
                                                      >
                                                >
                                              >,
                               float>,
                "Nested empty list should simplify");

  static_assert(std::is_same_v<simplify_unit_t<
                                            unit<me::X<
                                                        Meter<1>
                                                      >, double>
                                              >,
                               unit<me::X<
                                           Meter<1>
                                         >, double>>,
                "Simplified unit should not oversimplify");

  static_assert(std::is_same_v<simplify_unit_t<
                                            unit<me::X<
                                                        Meter<0>
                                                      >
                                                >
                                              >,
                               float>,
                "Dimensionless unit should simplify");

  static_assert(std::is_same_v<simplify_unit_t<unit<me::X<me::X<>,
                                                          Meter<0>,
                                                          me::X<me::X<Meter<0>>>
                                                         >
                                                   >
                                              >,
                               float>,
                "Dimensionless unit with packed lists should simplify");

  static_assert(std::is_same_v<simplify_unit_t<
                                            unit<me::X<
                                                        Kilogram<1>,
                                                        Meter<0>,
                                                        Kilogram<-1>
                                                      >
                                                >
                                              >, float>,
                "Addition should yield dimensionless value");
}

int main(int, const char**)
{
  test_simplify();
}
