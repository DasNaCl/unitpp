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

namespace me
{

struct meter
{  };

struct kilogram
{  };

struct second
{  };

//////////////////////////////////////////////////////////////////////
////////////////////////////////////////


//////////////////////////////////////////////
// user defined literals

#define define_literal(kind, literal, T)                                        \
constexpr                                                                       \
unit<me::X<measure<kind, 1>>, T> operator "" _##literal(long double val)        \
{ return unit<me::X<measure<kind, 1>>, T>::from(val); }                         \
constexpr                                                                       \
unit<me::X<measure<kind, 1>>, T> operator "" _##literal(unsigned long long val) \
{ return unit<me::X<measure<kind, 1>>, T>::from(val); }


#define define_quantity(kind, type, literal, T) \
struct yotta##kind : public type {  };          \
struct zetta##kind : public type {  };          \
struct exa##kind   : public type {  };          \
struct peta##kind  : public type {  };          \
struct tera##kind  : public type {  };          \
struct giga##kind  : public type {  };          \
struct mega##kind  : public type {  };          \
struct kilo##kind  : public type {  };          \
struct hecto##kind : public type {  };          \
struct deca##kind  : public type {  };          \
                                                \
struct kind        : public type {  };          \
                                                \
struct deci##kind  : public type {  };          \
struct centi##kind : public type {  };          \
struct milli##kind : public type {  };          \
struct micro##kind : public type {  };          \
struct nano##kind  : public type {  };          \
struct pico##kind  : public type {  };          \
struct femto##kind : public type {  };          \
struct atto##kind  : public type {  };          \
struct zepto##kind : public type {  };          \
struct yocto##kind : public type {  };          \
                                                \
define_literal(yotta##kind, Y##literal, T)      \
define_literal(zetta##kind, Z##literal, T)      \
define_literal(exa##kind,   E##literal, T)      \
define_literal(peta##kind,  P##literal, T)      \
define_literal(tera##kind,  T##literal, T)      \
define_literal(giga##kind,  G##literal, T)      \
define_literal(mega##kind,  M##literal, T)      \
define_literal(kilo##kind,  k##literal, T)      \
define_literal(hecto##kind, h##literal, T)      \
define_literal(deca##kind,  da##literal, T)     \
                                                \
define_literal(deci##kind,  d##literal, T)      \
define_literal(centi##kind, c##literal, T)      \
define_literal(milli##kind, m##literal, T)      \
define_literal(micro##kind, u##literal, T)      \
define_literal(nano##kind,  n##literal, T)      \
define_literal(pico##kind,  p##literal, T)      \
define_literal(femto##kind, f##literal, T)      \
define_literal(atto##kind,  a##literal, T)      \
define_literal(zepto##kind, z##literal, T)      \
define_literal(yocto##kind, y##literal, T)      \
  \
define_conversion(yotta##kind, kind, val, val / 1'000'000'000'000'000'000'000'000.) \
define_conversion(zetta##kind, kind, val, val / 1'000'000'000'000'000'000'000.) \
define_conversion(exa##kind,   kind, val, val / 1'000'000'000'000'000'000.) \
define_conversion(peta##kind,  kind, val, val / 1'000'000'000'000'000.) \
define_conversion(tera##kind,  kind, val, val / 1'000'000'000'000.) \
define_conversion(giga##kind,  kind, val, val / 1'000'000'000.) \
define_conversion(mega##kind,  kind, val, val / 1'000'000.) \
define_conversion(kilo##kind,  kind, val, val / 1'000.) \
define_conversion(hecto##kind, kind, val, val / 100.) \
define_conversion(deca##kind,  kind, val, val / 10.)

//////////////////////////////////////////////
template<class X1, class X2>
static auto test_unit_cast(int)  -> std::is_same<decltype(unit_cast(std::declval<X2>())), X1>;

template<class, class>
static auto test_unit_cast(long) -> std::false_type;

template<class X1, class X2>
struct has_unit_conv : decltype(test_unit_cast<X1, X2>(0))
{  };


template<class B1, class B2>
struct is_convertible : public std::bool_constant<std::is_same_v<B1, B2> || has_unit_conv<B1, B2>::value>
{  };

template<class X1, class X2>
inline constexpr bool is_convertible_v = is_convertible<X1, X2>::value;

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

//////////////////////////////////


}

int main()
{

  static_assert(std::is_same_v<simplify_unit_t<unit<me::X<>, float>>, float>, "Empty list should simplify");
  static_assert(std::is_same_v<simplify_unit_t<unit<me::X<me::X<me::X<>>>, int>>, int>, "Empty packed lists should simplify");
  static_assert(std::is_same_v<simplify_unit_t<unit<me::X<Meter<1>>, double>>,
                                               unit<me::X<Meter<1>>, double>>, "Simplified list should stay the same");
  static_assert(std::is_same_v<simplify_unit_t<unit<me::X<Meter<0>>>>, float>, "Dimensionless unit should simplify");
  static_assert(std::is_same_v<simplify_unit_t<unit<me::X<me::X<>,
                                                          Meter<0>,
                                                          me::X<me::X<Meter<0>>>
                                                         >
                                                   >
                                              >, float>, "Dimensionless unit with packed lists should simplify");
  static_assert(std::is_same_v<simplify_unit_t<unit<me::X<Kilogram<1>, Meter<0>, Kilogram<-1>>>>, float>,
                                                         "Addition should yield dimensionless value");
}
