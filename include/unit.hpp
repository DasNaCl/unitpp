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

template<typename L, typename T = float>
struct unit;

namespace me
{

namespace detail
{
template<class>
struct unit_info;
template<class L, class T>
struct unit_info<unit<L, T>>
{
  using list = L;
  using underlying_type = T;
};
template<class U>
using measure_list_t = typename unit_info<U>::list;
template<class U>
using underlying_type_t = typename unit_info<U>::underlying_type;
}

template<class u>
using result_helper_t = std::remove_reference_t<std::remove_cv_t<u>>;

template<class u>
using sort_unit_t = unit<me::sort_t<me::detail::base_cmp, detail::measure_list_t<u>>, detail::underlying_type_t<u>>;

template<class, class>
struct same_unit;
template<class L1, class T1, class L2, class T2>
struct same_unit<unit<L1, T1>, unit<L2, T2>> : public me::is_same_measure<L1, L2>
{  };
template<class A, class B>
inline constexpr bool same_unit_v = same_unit<result_helper_t<sort_unit_t<A>>,
                                              result_helper_t<sort_unit_t<B>>>::value;

// concats the unit list of two units
template<class u1, class u2>
struct mesh_together;
template<class L1, class T1, class L2, class T2>
struct mesh_together<unit<L1, T1>, unit<L2, T2>>
{ using type = unit<me::conc<L1, L2>, T1>; };

template<class u1, class u2>
using mesh_together_t = typename mesh_together<result_helper_t<u1>, result_helper_t<u2>>::type;

// inverses the exponents of a unit
template<class u>
struct inverse_unit;
template<class L, class T>
struct inverse_unit<unit<L, T>>
{
  using type = unit<inverse_measures_t<L>, T>;
};

template<class u>
using inverse_unit_t = typename inverse_unit<u>::type;

///// A 'generic' unit
}

template<class... Args, typename T>
struct unit<me::X<Args...>, T>
{
  using Units = me::X<Args...>;
  using Default = T;

  constexpr unit() = default;
  constexpr unit(T v) : val(v)
  {  }
  template<class u2>
  constexpr unit(std::enable_if_t<me::same_unit_v<unit<me::X<Args...>, T>, u2>, u2> u) : val(u.raw())
  {  }
  template<class u2>
  constexpr unit& operator=(std::enable_if_t<me::same_unit_v<unit<me::X<Args...>, T>, u2>, u2> u)
  {
    val = u.raw();
    return *this;
  }

  constexpr const T& raw() const
  { return val; }

  // operators
  // addition and subtraction
  template<class u2>
  constexpr auto operator+=(u2 b)
    -> std::enable_if_t<me::same_unit_v<unit<me::X<Args...>, T>, u2>, decltype(*this)&>
  {
    val += b.raw();
    return *this;
  }

  template<class u2>
  constexpr auto operator-=(u2 b)
    -> std::enable_if_t<me::same_unit_v<unit<me::X<Args...>, T>, u2>, decltype(*this)&>
  {
    val -= b.raw();
    return *this;
  }

  // scalar operations
  template<class D>
  constexpr auto operator*=(D b)
  -> std::enable_if_t<std::is_arithmetic_v<D> || std::is_same_v<me::result_helper_t<D>, T>,
                      decltype(*this)>
  {
    val *= b;
    return *this;
  }
  template<class D>
  constexpr auto operator/=(D b)
  -> std::enable_if_t<std::is_arithmetic_v<D> || std::is_same_v<me::result_helper_t<D>, T>,
                      decltype(*this)>
  {
    val /= b;
    return *this;
  }
private:
  T val;
};

namespace me
{

template<class T, bool shall_compoundify = false, bool shall_uncompoundify = true>
using simplify_unit_t = std::conditional_t<std::is_same_v<simplify_t<typename T::Units,
                                                                     typename T::Default,
                                                                     shall_compoundify,
                                                                     shall_uncompoundify>,
                                                          typename T::Default>,
                                           typename T::Default,
                                           unit<simplify_t<typename T::Units,
                                                           typename T::Default,
                                                           shall_compoundify,
                                                           shall_uncompoundify>, typename T::Default>
                                          >;
}

// conversion
template<class L1, class T1, class L2, class T2>
constexpr auto unit_cast(unit<L2, T2> from)
  -> std::enable_if_t<me::same_unit_v<unit<L1, T1>, unit<L2, T2>>, unit<L1, T1>> // <- to
{
  return { from.raw() };
}

namespace me::detail
{
  template<class>
  struct convert_helper;

  template<class... Args>
  struct convert_helper<me::X<Args...>>
  {
    using hd = me::head<me::X<Args...>>;
    using tl = me::tail<me::X<Args...>>;

    // convert to base
    template<class T>
    constexpr static T to_base_measure(T t)
    {
      constexpr int e = hd::exponent < 0 ? (-1 * hd::exponent) : hd::exponent;
      T v = t;
      if constexpr(me::is_converted_measure_v<hd>)
      {
        for(int i = 0; i < e; ++i)
        {
          if constexpr(hd::exponent > 0)
            v = hd::kind::StoB(t);
          else
            v = hd::kind::BtoS(t); // if negative, do the inverse
        }
      }
      return convert_helper<tl>::to_base_measure(v);
    }
    
    // convert from base
    template<class T>
    constexpr static T from_base_measure(T t)
    {
      constexpr int e = hd::exponent < 0 ? (-1 * hd::exponent) : hd::exponent;
      T v = t;
      if constexpr(me::is_converted_measure_v<hd>)
      {
        for(int i = 0; i < e; ++i)
        {
          if constexpr(hd::exponent > 0)
            v = hd::kind::BtoS(t);
          else
            v = hd::kind::StoB(t); // if negative, do the inverse
        }
      }
      return convert_helper<tl>::from_base_measure(v);  
    }

    // possibly convert from different systems
    template<class, class, class>
    struct system_convert_helper;

    template<class Garbage, class... A1, class... B1>
    struct system_convert_helper<Garbage, me::X<A1...>, me::X<B1...>>
    {
      template<class F>
      using no_conv_measure = std::bool_constant<!me::is_converted_measure_v<F>>;
      static_assert(me::forall_v<no_conv_measure, me::X<A1...>>, "system_convert expects a converted-measure-free list");
      static_assert(me::forall_v<no_conv_measure, me::X<B1...>>, "system_convert expects a converted-measure-free list");
      
      using cur = me::head<me::X<A1...>>;
      
      template<class F>
      using same_measure = std::bool_constant<me::is_same_type_v<cur, F> && me::is_same_kind_v<cur, F>>;

      template<class D>
      constexpr static D f(D d)
      {
        using found = me::find<same_measure, me::X<B1...>>;
        constexpr bool cond = found::value;
        if constexpr(cond)
        {
          // everything fine, go on (note that we assume a converted-measure free list)
          return d;
        }
        else
        {
          // unit *must* be converted with a system_conversion
          static_assert(me::exists_system_convert_v<cur, typename found::type>, "No system conversion existent");

          return me::system_convert<cur, typename found::type>()(d);
        }
      }
    };

    template<class Garbage>
    struct system_convert_helper<Garbage, me::X<>, me::X<>>
    {
      template<class D>
      constexpr static D f(D d)
      { return d; }
    };

    template<class T>
    constexpr static T system_conversions(T t)
    {
      using A = hd; // first list
      using B = me::head<tl>; // second list

      using Ab = me::basify_converted_units_t<A>;
      using Bb = me::basify_converted_units_t<B>;

      return system_convert_helper<void, Ab, Bb>::f(t);
    }
  };
  template<>
  struct convert_helper<me::X<>>
  {
    template<class T>
    constexpr static T to_base_measure(T t)
    { return t; }

    template<class T>
    constexpr static T system_conversions(T t)
    { return t; }

    template<class T>
    constexpr static T from_base_measure(T t)
    { return t; }
  };
}

template<class U1, class U2>
constexpr auto unit_cast(U2 from)
  -> std::enable_if_t<!me::same_unit_v<U1, U2>, U1> // <- to
{
  using A = me::explode_t<me::uncompoundify<me::detail::measure_list_t<U1>>>;
  using B = me::explode_t<me::uncompoundify<me::detail::measure_list_t<U2>>>;

  auto tmp = me::detail::convert_helper<B>::to_base_measure(from.raw());
  auto sys = me::detail::convert_helper<me::X<A, B>>::system_conversions(tmp);
  auto res = me::detail::convert_helper<A>::from_base_measure(sys);

  return { res };
}
/////
namespace me
{


//// operators
// addition and subtraction
template<class u1, class u2>
constexpr auto operator+(u1 a, u2 b)
  -> std::enable_if_t<same_unit_v<u1, u2>, result_helper_t<u1>>
{
  return { a.raw() + b.raw() };
}

template<class u1, class u2>
constexpr auto operator-(u1 a, u2 b)
  -> std::enable_if_t<same_unit_v<u1, u2>, result_helper_t<u1>>
{
  return { a.raw() - b.raw() };
}

// multiplication and division
template<class u1, class u2>
constexpr auto operator*(u1 a, u2 b)
  -> simplify_unit_t<mesh_together_t<u1, u2>>
{
  return { a.raw() * b.raw() };
}

template<class u1, class u2>
constexpr auto operator/(u1 a, u2 b)
  -> simplify_unit_t<mesh_together_t<u1, inverse_unit_t<u2>>>
{
  return { a.raw() / b.raw() };
}

// scalar operations
template<class u, class T>
constexpr auto operator*(u a, T b)
  -> std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<result_helper_t<T>, typename u::Default>,
                      result_helper_t<u>>
{
  return { a.raw() * b };
}
template<class u, class T>
constexpr auto operator*(T b, u a)
  -> std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<result_helper_t<T>, typename u::Default>,
                      result_helper_t<u>>
{
  return { b * a.raw() };
}
template<class u, class T>
constexpr auto operator/(u a, T b)
  -> std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<result_helper_t<T>, typename u::Default>,
                      result_helper_t<u>>
{
  return { a.raw() / b };
}
template<class u, class T>
constexpr auto operator/(T b, u a)
  -> std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<result_helper_t<T>, typename u::Default>,
                      inverse_unit_t<result_helper_t<u>>>
{
  return { b / a.raw() };
}

}
