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
#include <detail/scales.hpp>
#include <detail/crc.hpp>
#include <info.hpp>

#define unitpp_define_quantity_helper(longname, basename)                                    \
struct longname { static constexpr std::uint64_t id = unitpp::detail::CRC64(#longname);          \
                  static constexpr std::uint64_t base_id = unitpp::detail::CRC64(#basename); };

#define unitpp_define_quantity(kind)              \
unitpp_define_quantity_helper(yotta##kind, kind) \
unitpp_define_quantity_helper(zetta##kind, kind) \
unitpp_define_quantity_helper(  exa##kind, kind) \
unitpp_define_quantity_helper( peta##kind, kind) \
unitpp_define_quantity_helper( tera##kind, kind) \
unitpp_define_quantity_helper( giga##kind, kind) \
unitpp_define_quantity_helper( mega##kind, kind) \
unitpp_define_quantity_helper( kilo##kind, kind) \
unitpp_define_quantity_helper(hecto##kind, kind) \
unitpp_define_quantity_helper( deca##kind, kind) \
unitpp_define_quantity_helper(       kind, kind) \
unitpp_define_quantity_helper( deci##kind, kind) \
unitpp_define_quantity_helper(centi##kind, kind) \
unitpp_define_quantity_helper(milli##kind, kind) \
unitpp_define_quantity_helper(micro##kind, kind) \
unitpp_define_quantity_helper( nano##kind, kind) \
unitpp_define_quantity_helper( pico##kind, kind) \
unitpp_define_quantity_helper(femto##kind, kind) \
unitpp_define_quantity_helper( atto##kind, kind) \
unitpp_define_quantity_helper(zepto##kind, kind) \
unitpp_define_quantity_helper(yocto##kind, kind)


#define unitpp_define_quantitiy_unit_literals_helper(longname, name, literal, BtoS, StoB, T) \
constexpr unit<unitpp::tmpl::X<unitpp::measure<unitpp::detail::MeasureType::Converted, \
               unitpp::detail::scale_info<longname, unitpp::scales::BtoS<T>, \
               unitpp::scales::StoB<T>, unitpp::measure<unitpp::detail::MeasureType::Base, name, 1>>, 1>>> literal(1.f);

#define unitpp_define_quantity_unit_literals(name, literal, T) \
unitpp_define_quantitiy_unit_literals_helper(yotta##name, name,  Y##literal, yotta, yocto, T) \
unitpp_define_quantitiy_unit_literals_helper(zetta##name, name,  Z##literal, zetta, zepto, T) \
unitpp_define_quantitiy_unit_literals_helper(exa##name,   name,  E##literal, exa,   atto,  T) \
unitpp_define_quantitiy_unit_literals_helper(peta##name,  name,  P##literal, peta,  femto, T) \
unitpp_define_quantitiy_unit_literals_helper(tera##name,  name,  T##literal, tera,  pico,  T) \
unitpp_define_quantitiy_unit_literals_helper(giga##name,  name,  G##literal, giga,  nano,  T) \
unitpp_define_quantitiy_unit_literals_helper(mega##name,  name,  M##literal, mega,  micro, T) \
unitpp_define_quantitiy_unit_literals_helper(kilo##name,  name,  k##literal, kilo,  milli, T) \
unitpp_define_quantitiy_unit_literals_helper(hecto##name, name,  h##literal, hecto, centi, T) \
unitpp_define_quantitiy_unit_literals_helper(deca##name,  name, da##literal, deca,  deci,  T) \
constexpr unit<unitpp::tmpl::X<unitpp::measure<unitpp::detail::MeasureType::Base, name, 1>>> literal(1.f);              \
unitpp_define_quantitiy_unit_literals_helper(deci##name,  name,  d##literal, deci,  deca,  T) \
unitpp_define_quantitiy_unit_literals_helper(centi##name, name,  c##literal, centi, hecto, T) \
unitpp_define_quantitiy_unit_literals_helper(milli##name, name,  m##literal, milli, kilo,  T) \
unitpp_define_quantitiy_unit_literals_helper(micro##name, name,  u##literal, micro, mega,  T) \
unitpp_define_quantitiy_unit_literals_helper(nano##name,  name,  n##literal, nano,  giga,  T) \
unitpp_define_quantitiy_unit_literals_helper(pico##name,  name,  p##literal, pico,  tera,  T) \
unitpp_define_quantitiy_unit_literals_helper(femto##name, name,  f##literal, femto, peta,  T) \
unitpp_define_quantitiy_unit_literals_helper(atto##name,  name,  a##literal, atto,  exa,   T) \
unitpp_define_quantitiy_unit_literals_helper(zepto##name, name,  z##literal, zepto, zetta, T) \
unitpp_define_quantitiy_unit_literals_helper(yocto##name, name,  y##literal, yocto, yotta, T)

#define unitpp_define_base_unit_literal(literal, name) \
unit<unitpp::tmpl::X<unitpp::measure<unitpp::detail::MeasureType::Base, name, 1>>> literal(1.f);

#define unitpp_define_compound_unit_literal(literal, ...) \
unit<unitpp::tmpl::X<__VA_ARGS__>> literal(1.f);

#define unitpp_define_alias_base(capitalized_name, name)              \
template<int n = 1>                                                   \
using capitalized_name = unitpp::measure<unitpp::detail::MeasureType::Base, name, n>;

#define unitpp_define_alias_converted(capitalized_name, name, BtoS, StoB, Base_) \
template<int n = 1> \
using capitalized_name = unitpp::measure<unitpp::detail::MeasureType::Converted, \
                                     unitpp::detail::scale_info<name, StoB, BtoS, unitpp::measure<unitpp::detail::MeasureType::Base, Base_, 1>>, n>;

#define unitpp_define_alias_compound(capitalized_name, ...)                             \
template<int n = 1>                                                                     \
using capitalized_name = unitpp::measure<unitpp::detail::MeasureType::Compound, unitpp::tmpl::X<__VA_ARGS__>, n>;

#define unitpp_define_compound_conversion(capitalized_name, ...) \
namespace unitpp { template<>                                 \
struct exists_compound<unitpp::tmpl::sort_t<unitpp::detail::hidden::kind_cmp, unitpp::tmpl::X<__VA_ARGS__>>> : std::true_type \
{ using type = unitpp::tmpl::X<capitalized_name<1>>; }; }

#define unitpp_define_quantity_alias(capitalized_name, name) \
unitpp_define_alias_converted(Yotta##name, yotta##name, unitpp::scales::yocto<float>, unitpp::scales::yotta<float>, name) \
unitpp_define_alias_converted(Zetta##name, zetta##name, unitpp::scales::zepto<float>, unitpp::scales::zetta<float>, name) \
unitpp_define_alias_converted(  Exa##name,   exa##name, unitpp::scales::atto<float>,  unitpp::scales::exa<float>,   name) \
unitpp_define_alias_converted( Peta##name,  peta##name, unitpp::scales::femto<float>, unitpp::scales::peta<float>,  name) \
unitpp_define_alias_converted( Tera##name,  tera##name, unitpp::scales::pico<float>,  unitpp::scales::tera<float>,  name) \
unitpp_define_alias_converted( Giga##name,  giga##name, unitpp::scales::nano<float>,  unitpp::scales::giga<float>,  name) \
unitpp_define_alias_converted( Mega##name,  mega##name, unitpp::scales::micro<float>, unitpp::scales::mega<float>,  name) \
unitpp_define_alias_converted( Kilo##name,  kilo##name, unitpp::scales::milli<float>, unitpp::scales::kilo<float>,  name) \
unitpp_define_alias_converted(Hecto##name, hecto##name, unitpp::scales::centi<float>, unitpp::scales::hecto<float>, name) \
unitpp_define_alias_converted( Deca##name,  deca##name, unitpp::scales::deci<float>,  unitpp::scales::deca<float>,  name) \
unitpp_define_alias_base(capitalized_name,   name) \
unitpp_define_alias_converted( Deci##name,  deci##name, unitpp::scales::deca<float>,  unitpp::scales::deci<float>,  name) \
unitpp_define_alias_converted(Centi##name, centi##name, unitpp::scales::hecto<float>, unitpp::scales::centi<float>, name) \
unitpp_define_alias_converted(Milli##name, milli##name, unitpp::scales::kilo<float>,  unitpp::scales::milli<float>, name) \
unitpp_define_alias_converted(Micro##name, micro##name, unitpp::scales::mega<float>,  unitpp::scales::micro<float>, name) \
unitpp_define_alias_converted( Nano##name,  nano##name, unitpp::scales::giga<float>,  unitpp::scales::nano<float>,  name) \
unitpp_define_alias_converted( Pico##name,  pico##name, unitpp::scales::tera<float>,  unitpp::scales::pico<float>,  name) \
unitpp_define_alias_converted(Femto##name, femto##name, unitpp::scales::peta<float>,  unitpp::scales::femto<float>, name) \
unitpp_define_alias_converted( Atto##name,  atto##name, unitpp::scales::exa<float>,   unitpp::scales::atto<float>,  name) \
unitpp_define_alias_converted(Zepto##name, zepto##name, unitpp::scales::zetta<float>, unitpp::scales::zepto<float>, name) \
unitpp_define_alias_converted(Yocto##name, yocto##name, unitpp::scales::yotta<float>, unitpp::scales::yocto<float>, name)


#define unitpp_define_base_quantity(capitalized_name, name, literal) \
unitpp_define_quantity(name)                                         \
unitpp_define_quantity_unit_literals(name, literal, float)           \
unitpp_define_quantity_alias(capitalized_name, name)

namespace si
{

// base
unitpp_define_base_quantity(Gram, gram, g)
unitpp_define_base_quantity(Meter, meter, m)
unitpp_define_base_quantity(Second, second, s)
unitpp_define_base_quantity(Ampere, ampere, A)
unitpp_define_base_quantity(Kelvin, kelvin, K)
unitpp_define_base_quantity(Mole, mole, mol)
unitpp_define_base_quantity(Candela, candela, cd)

// compound
unitpp_define_alias_compound(Radian, Meter<1>, Meter<-1>)
unitpp_define_alias_compound(Steradian, Meter<1>, Meter<-1>)
unitpp_define_alias_compound(Hertz, Second<-1>)
unitpp_define_alias_compound(Newton, Kilogram<1>, Meter<1>, Second<-2>)
unitpp_define_alias_compound(Pascal, Newton<1>, Meter<-2>)
unitpp_define_alias_compound(Joule, Kilogram<1>, Meter<2>, Second<-2>)
unitpp_define_alias_compound(Watt, Joule<1>, Second<-1>)
unitpp_define_alias_compound(Couloumb, Second<1>, Ampere<1>)
unitpp_define_alias_compound(Volt, Watt<1>, Ampere<-1>)
unitpp_define_alias_compound(Farad, Couloumb<1>, Volt<-1>)
unitpp_define_alias_compound(Ohm, Volt<1>, Ampere<-1>)
unitpp_define_alias_compound(Siemens, Ampere<1>, Volt<-1>)
unitpp_define_alias_compound(Weber, Volt<1>, Second<1>)
unitpp_define_alias_compound(Tesla, Weber<1>, Meter<-2>)
unitpp_define_alias_compound(Henry, Weber<1>, Ampere<-1>)
unitpp_define_alias_compound(Celsius, Kelvin<1>)
unitpp_define_alias_compound(Lumen, Candela<1>)
unitpp_define_alias_compound(Lux, Candela<1>, Meter<-2>)
unitpp_define_alias_compound(Becquerel, Second<-1>)
unitpp_define_alias_compound(Gray, Meter<2>, Second<-2>)
unitpp_define_alias_compound(Katal, Mole<1>, Second<-1>)

unitpp_define_compound_unit_literal(rad, Radian<1>)
unitpp_define_compound_unit_literal(sr, Steradian<1>)
unitpp_define_compound_unit_literal(Hz, Hertz<1>)
unitpp_define_compound_unit_literal(N, Newton<1>)
unitpp_define_compound_unit_literal(Pa, Pascal<1>)
unitpp_define_compound_unit_literal(J, Joule<1>)
unitpp_define_compound_unit_literal(W, Watt<1>)
unitpp_define_compound_unit_literal(C, Couloumb<1>)
unitpp_define_compound_unit_literal(V, Volt<1>)
unitpp_define_compound_unit_literal(F, Farad<1>)
unitpp_define_compound_unit_literal(Om, Ohm<1>)
unitpp_define_compound_unit_literal(S, Siemens<1>)
unitpp_define_compound_unit_literal(Wb, Weber<1>)
unitpp_define_compound_unit_literal(T, Tesla<1>)
unitpp_define_compound_unit_literal(H, Henry<1>)
unitpp_define_compound_unit_literal(oC, Celsius<1>)
unitpp_define_compound_unit_literal(lm, Lumen<1>)
unitpp_define_compound_unit_literal(lx, Lux<1>)
unitpp_define_compound_unit_literal(Bq, Becquerel<1>)
unitpp_define_compound_unit_literal(Gy, Gray<1>)
unitpp_define_compound_unit_literal(kat, Katal<1>)
}
unitpp_define_compound_conversion(si::Radian, si::Meter<1>, si::Meter<-1>)

unitpp_define_compound_conversion(si::Steradian, si::Meter<2>, si::Meter<-2>)

unitpp_define_compound_conversion(si::Hertz, si::Second<-1>)

unitpp_define_compound_conversion(si::Newton, si::Kilogram<1>, si::Meter<1>, si::Second<-2>)

unitpp_define_compound_conversion(si::Pascal, si::Newton<1>, si::Meter<-2>)
unitpp_define_compound_conversion(si::Pascal, si::Kilogram<1>, si::Meter<-1>, si::Second<-2>)

unitpp_define_compound_conversion(si::Joule, si::Kilogram<1>, si::Meter<2>, si::Second<-2>)
unitpp_define_compound_conversion(si::Joule, si::Newton<1>, si::Meter<1>)
unitpp_define_compound_conversion(si::Joule, si::Pascal<1>, si::Meter<3>)

unitpp_define_compound_conversion(si::Watt, si::Joule<1>, si::Second<-1>)
unitpp_define_compound_conversion(si::Watt, si::Kilogram<1>, si::Meter<2>, si::Second<-3>)

unitpp_define_compound_conversion(si::Couloumb, si::Second<1>, si::Ampere<1>)

unitpp_define_compound_conversion(si::Volt, si::Watt<1>, si::Ampere<-1>)
unitpp_define_compound_conversion(si::Volt, si::Kilogram<1>, si::Meter<2>, si::Second<-3>, si::Ampere<-1>)

unitpp_define_compound_conversion(si::Farad, si::Couloumb<1>, si::Volt<-1>)
unitpp_define_compound_conversion(si::Farad, si::Kilogram<-1>, si::Meter<-2>, si::Second<4>, si::Ampere<2>)

unitpp_define_compound_conversion(si::Ohm, si::Volt<1>, si::Ampere<-1>)
unitpp_define_compound_conversion(si::Ohm, si::Kilogram<1>, si::Meter<2>, si::Second<-3>, si::Ampere<-2>)

unitpp_define_compound_conversion(si::Siemens, si::Ampere<1>, si::Volt<-1>)
unitpp_define_compound_conversion(si::Siemens, si::Kilogram<-1>, si::Meter<-2>, si::Second<3>, si::Ampere<2>)

unitpp_define_compound_conversion(si::Weber, si::Volt<1>, si::Second<1>)
unitpp_define_compound_conversion(si::Weber, si::Kilogram<1>, si::Meter<2>, si::Second<-2>, si::Ampere<-1>)

unitpp_define_compound_conversion(si::Tesla, si::Weber<1>, si::Meter<-2>)
unitpp_define_compound_conversion(si::Tesla, si::Kilogram<1>, si::Second<-2>, si::Ampere<-1>)

unitpp_define_compound_conversion(si::Henry, si::Weber<1>, si::Ampere<-1>)
unitpp_define_compound_conversion(si::Henry, si::Kilogram<1>, si::Meter<2>, si::Second<-2>, si::Ampere<-2>)

unitpp_define_compound_conversion(si::Celsius, si::Kelvin<1>)

unitpp_define_compound_conversion(si::Lumen, si::Candela<1>)
unitpp_define_compound_conversion(si::Lumen, si::Candela<1>, si::Steradian<1>)

unitpp_define_compound_conversion(si::Lux, si::Candela<1>, si::Meter<-2>)
unitpp_define_compound_conversion(si::Lux, si::Lumen<1>, si::Meter<-2>)

//TODO: unitpp_define_compound_conversion(si::Becquerel, si::Second<-1>)

unitpp_define_compound_conversion(si::Gray, si::Joule<1>, si::Kilogram<-1>)
unitpp_define_compound_conversion(si::Gray, si::Meter<2>, si::Second<-2>)

//TODO: Sievert??

unitpp_define_compound_conversion(si::Katal, si::Mole<1>, si::Second<-1>)




namespace unitpp
{
template<int exp, class StrT>
struct measure_info<si::Meter<exp>, StrT>
{
  constexpr static StrT symbol() { return "m"; }
  constexpr static StrT name()   { return "Meter"; }
};
template<int exp, class StrT>
struct measure_info<si::Kilogram<exp>, StrT>
{
  constexpr static StrT symbol() { return "kg"; }
  constexpr static StrT name()   { return "Kilogram"; }
};
template<int exp, class StrT>
struct measure_info<si::Second<exp>, StrT>
{
  constexpr static StrT symbol() { return "s"; }
  constexpr static StrT name()   { return "Second"; }
};
template<int exp, class StrT>
struct measure_info<si::Joule<exp>, StrT>
{
  constexpr static StrT symbol() { return "J"; }
  constexpr static StrT name()   { return "Joule"; }
};
template<int exp, class StrT>
struct measure_info<si::Watt<exp>, StrT>
{
  constexpr static StrT symbol() { return "W"; }
  constexpr static StrT name()   { return "Watt"; }
};
}
