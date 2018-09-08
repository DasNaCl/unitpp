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

#include <detail/is_detected.hpp>

namespace unitpp
{
namespace detail
{
  namespace detected
  {
    template<class T>
    using has_operator_mult_with_scalar = decltype(std::declval<T>() * std::declval<int>());
    template<class T>
    using has_operator_div_with_scalar = decltype(std::declval<T>() / std::declval<int>());
  }

  template<class T>
  struct times_10
  {
    static_assert(is_detected_v<detected::has_operator_mult_with_scalar, T>,
                  "T shall be multiplicable with an int");
    constexpr auto operator()(T val) const
    { return val * 10; }
  };

  template<class T>
  struct by_10
  {
    static_assert(is_detected_v<detected::has_operator_div_with_scalar, T>,
                  "T shall be divisible with an int");
    constexpr auto operator()(T val) const
    { return val / 10; }
  };

  template<std::size_t n, class T, template<class> class f>
  struct repeat
  {
    constexpr auto operator()(T val) const
    { return (repeat<n-1, T, f>()((f<T>()(val)))); }
  };
  template<class T, template<class> class f>
  struct repeat<1, T, f>
  {
    constexpr auto operator()(T val) const
    { return (f<T>()(val)); }
  };
  template<class T, template<class> class f>
  struct repeat<0, T, f>
  {
    constexpr auto operator()(T val) const
    { return val; }
  };

  template<std::size_t n, class T, template<class> class f>
  inline constexpr repeat<n, T, f> repeat_v;
}
namespace scales
{
  // scales
  template<class T> inline constexpr auto yotta_v = detail::repeat_v<24, T, detail::times_10>;
  template<class T> inline constexpr auto zetta_v = detail::repeat_v<21, T, detail::times_10>;
  template<class T> inline constexpr auto   exa_v = detail::repeat_v<18, T, detail::times_10>;
  template<class T> inline constexpr auto  peta_v = detail::repeat_v<15, T, detail::times_10>;
  template<class T> inline constexpr auto  tera_v = detail::repeat_v<12, T, detail::times_10>;
  template<class T> inline constexpr auto  giga_v = detail::repeat_v< 9, T, detail::times_10>;
  template<class T> inline constexpr auto  mega_v = detail::repeat_v< 6, T, detail::times_10>;
  template<class T> inline constexpr auto  kilo_v = detail::repeat_v< 3, T, detail::times_10>;
  template<class T> inline constexpr auto hecto_v = detail::repeat_v< 2, T, detail::times_10>;
  template<class T> inline constexpr auto  deca_v = detail::repeat_v< 1, T, detail::times_10>;

  template<class T> inline constexpr auto  deci_v = detail::repeat_v< 1, T, detail::by_10>;
  template<class T> inline constexpr auto centi_v = detail::repeat_v< 2, T, detail::by_10>;
  template<class T> inline constexpr auto milli_v = detail::repeat_v< 3, T, detail::by_10>;
  template<class T> inline constexpr auto micro_v = detail::repeat_v< 6, T, detail::by_10>;
  template<class T> inline constexpr auto  nano_v = detail::repeat_v< 9, T, detail::by_10>;
  template<class T> inline constexpr auto  pico_v = detail::repeat_v<12, T, detail::by_10>;
  template<class T> inline constexpr auto femto_v = detail::repeat_v<15, T, detail::by_10>;
  template<class T> inline constexpr auto  atto_v = detail::repeat_v<18, T, detail::by_10>;
  template<class T> inline constexpr auto zepto_v = detail::repeat_v<21, T, detail::by_10>;
  template<class T> inline constexpr auto yocto_v = detail::repeat_v<24, T, detail::by_10>;

  template<class T> using yotta = detail::repeat<24, T, detail::times_10>;
  template<class T> using zetta = detail::repeat<21, T, detail::times_10>;
  template<class T> using   exa = detail::repeat<18, T, detail::times_10>;
  template<class T> using  peta = detail::repeat<15, T, detail::times_10>;
  template<class T> using  tera = detail::repeat<12, T, detail::times_10>;
  template<class T> using  giga = detail::repeat< 9, T, detail::times_10>;
  template<class T> using  mega = detail::repeat< 6, T, detail::times_10>;
  template<class T> using  kilo = detail::repeat< 3, T, detail::times_10>;
  template<class T> using hecto = detail::repeat< 2, T, detail::times_10>;
  template<class T> using  deca = detail::repeat< 1, T, detail::times_10>;

  template<class T> using  deci = detail::repeat< 1, T, detail::by_10>;
  template<class T> using centi = detail::repeat< 2, T, detail::by_10>;
  template<class T> using milli = detail::repeat< 3, T, detail::by_10>;
  template<class T> using micro = detail::repeat< 6, T, detail::by_10>;
  template<class T> using  nano = detail::repeat< 9, T, detail::by_10>;
  template<class T> using  pico = detail::repeat<12, T, detail::by_10>;
  template<class T> using femto = detail::repeat<15, T, detail::by_10>;
  template<class T> using  atto = detail::repeat<18, T, detail::by_10>;
  template<class T> using zepto = detail::repeat<21, T, detail::by_10>;
  template<class T> using yocto = detail::repeat<24, T, detail::by_10>;
}
}
