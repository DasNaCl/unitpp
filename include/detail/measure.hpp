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
#include <detail/is_detected.hpp>

#include <type_traits>
#include <cstdint>

namespace unitpp
{
namespace detail
{

enum class MeasureType
{
  Base,
  Compound,
  Converted,
};

template<class S, class f, class g, class B>
struct scale_info
{
  using Scaled = S;
  inline constexpr static auto StoB = f();
  inline constexpr static auto BtoS = g();
  using Base = B;

  constexpr static std::uint64_t id = S::id;
  constexpr static std::uint64_t base_id = S::base_id;
};

template<MeasureType t, class H, int n>
struct measure
{
  static constexpr MeasureType type = t;
  using kind = H;
  static constexpr int exponent = n;

  template<typename V>
  static constexpr std::uint64_t h()
  {
    if constexpr(tmpl::is_list_v<V>)
    {
      if constexpr(tmpl::is_list_empty_v<V>)
        return 61;
      else
        return h<tmpl::head<V>>() ^ (h<tmpl::tail<V>>() << 11);
    }
    else
      return V::id;
  }
  template<typename V>
  static constexpr std::uint64_t f()
  {
    if constexpr(tmpl::is_list_v<V>)
    {
      if constexpr(tmpl::is_list_empty_v<V>)
        return 61;
      else
        return f<tmpl::head<V>>() ^ (f<tmpl::tail<V>>() << 11);
    }
    else
      return V::base_id;
  }
  static constexpr std::uint64_t id = std::integral_constant<std::uint64_t, h<H>()>::value;
  static constexpr std::uint64_t base_id = std::integral_constant<std::uint64_t, f<H>()>::value;
};

namespace detected
{
template<class T>
using accept_measures_only_type = decltype(std::declval<T>().type);
template<class T>
using accept_measures_only_kind = typename T::kind;
template<class T>
using accept_measures_only_exponent = decltype(std::declval<T>().exponent);
template<class T>
using accept_measures_only_id = decltype(std::declval<T>().id);
template<class T>
using accept_measures_only_base_id = decltype(std::declval<T>().base_id);
}

template<class T>
using is_measure = std::bool_constant<is_detected_v<detected::accept_measures_only_kind, T>
                                   && is_detected_v<detected::accept_measures_only_type, T>
                                   && is_detected_v<detected::accept_measures_only_exponent, T>
                                   && is_detected_v<detected::accept_measures_only_id, T>
                                   && is_detected_v<detected::accept_measures_only_base_id, T>>;
template<class T>
constexpr bool is_measure_v = is_measure<T>::value;

namespace detail
{
  template<class T>
  struct measure_deducer
  { static_assert(is_measure_v<T>, "Can't deduce the MeasureType from a non-measure"); };

  template<MeasureType MT, class K, int n>
  struct measure_deducer<measure<MT, K, n>>
  {
    inline static constexpr MeasureType measure_type = MT;
    using kind = K;
  };
}

template<class T>
inline constexpr bool is_base_measure_v = detail::measure_deducer<T>::measure_type == MeasureType::Base;
template<class T>
inline constexpr bool is_compound_measure_v = detail::measure_deducer<T>::measure_type == MeasureType::Compound;
template<class T>
inline constexpr bool is_converted_measure_v = detail::measure_deducer<T>::measure_type == MeasureType::Converted;

template<class A, class B>
using is_same_measure_type = std::bool_constant<
                              detail::measure_deducer<A>::measure_type ==
                                detail::measure_deducer<B>::measure_type>;

template<class A, class B>
inline constexpr bool is_same_measure_type_v = is_same_measure_type<A, B>::value;

template<class A, class B>
using is_same_kind = std::is_same<typename detail::measure_deducer<A>::kind,
                                  typename detail::measure_deducer<B>::kind>;
template<class A, class B>
inline constexpr bool is_same_kind_v = is_same_kind<A, B>::value;

template<class A, class B>
using is_exact_same_measure = tmpl::same_elements<A, B>;
}
}
