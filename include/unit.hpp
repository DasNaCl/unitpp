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

#include <tmpl/forall.hpp>
#include <tmpl/find.hpp>
#include <tmpl/sort.hpp>
#include <detail/basify_converted_units.hpp>
#include <detail/inverse_measures.hpp>
#include <detail/uncompoundify.hpp>
#include <detail/compoundify.hpp>
#include <detail/explode.hpp>
#include <system_convert.hpp>
#include <simplify.hpp>
#include <measure.hpp>

template<typename L, typename T = float>
struct unit;

namespace unitpp
{

namespace detail
{
template<class u>
using result_helper_t = std::remove_reference_t<std::remove_cv_t<u>>;
}

template<class U>
struct is_unit : public std::false_type
{  };
template<class L, class T>
struct is_unit<unit<L, T>> : public std::true_type
{  };

template<class U>
inline constexpr bool is_unit_v = is_unit<detail::result_helper_t<U>>::value;

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
using sort_unit_t = unit<tmpl::sort_t<detail::hidden::base_cmp, detail::measure_list_t<u>>, detail::underlying_type_t<u>>;

template<class, class>
struct same_unit : public std::false_type {};
template<class L1, class T1, class L2, class T2>
struct same_unit<unit<L1, T1>, unit<L2, T2>>
{
  using nL1 = detail::measure_list_t<sort_unit_t<unit<L1, T1>>>;
  using nL2 = detail::measure_list_t<sort_unit_t<unit<L2, T2>>>;

  constexpr static bool value = is_exact_same_measure_v<detail::result_helper_t<nL1>, detail::result_helper_t<nL2>>;
};
template<class A, class B>
inline constexpr bool same_unit_v = same_unit<A, B>::value;

// concats the unit list of two units
template<class u1, class u2>
struct mesh_together;
template<class L1, class T1, class L2, class T2>
struct mesh_together<unit<L1, T1>, unit<L2, T2>>
{ using type = unit<tmpl::conc<L1, L2>, T1>; };

template<class u1, class u2>
using mesh_together_t = typename mesh_together<detail::result_helper_t<u1>, detail::result_helper_t<u2>>::type;

// inverses the exponents of a unit
template<class u>
struct inverse_unit;
template<class L, class T>
struct inverse_unit<unit<L, T>>
{
  using type = unit<detail::inverse_measures_t<L>, T>;
};

template<class u>
using inverse_unit_t = typename inverse_unit<detail::result_helper_t<u>>::type;


template<class T, bool shall_compoundify = false, bool shall_uncompoundify = true>
using simplify_unit_t = std::conditional_t<std::is_same_v<unitpp::simplify_t<typename T::Units,
                                                                     typename T::Default,
                                                                     shall_compoundify,
                                                                     shall_uncompoundify>,
                                                          typename T::Default>,
                                           typename T::Default,
                                           unit<unitpp::simplify_t<typename T::Units,
                                                           typename T::Default,
                                                           shall_compoundify,
                                                           shall_uncompoundify>, typename T::Default>
                                          >;
template<class, class>
struct info;
}

template<class... Args, typename T>
struct unit<unitpp::tmpl::X<Args...>, T>
{
  using Units = unitpp::tmpl::X<Args...>;
  using Default = T;

  template<class List, class D>
  friend struct unit;

  template<class, class>
  friend struct unitpp::info;

  constexpr unit() = default;
  constexpr unit(T v) : val(v)
  {  }
  template<class u2>
  constexpr unit(std::enable_if_t<unitpp::is_unit_v<u2>
                      && unitpp::same_unit_v<unit<unitpp::tmpl::X<Args...>, T>, u2>, u2> u) : val(u.val)
  {  }
  template<class u2>
  constexpr unit& operator=(std::enable_if_t<unitpp::is_unit_v<u2>
                                 && unitpp::same_unit_v<unit<unitpp::tmpl::X<Args...>, T>, u2>, u2> u)
  {
    val = u.val;
    return *this;
  }

  // operators
  // addition and subtraction
  template<class u2>
  constexpr auto operator+=(u2 b)
    -> std::enable_if_t<unitpp::is_unit_v<u2>
            && unitpp::same_unit_v<unit<unitpp::tmpl::X<Args...>, T>, u2>, decltype(*this)&>
  {
    val += b.val;
    return *this;
  }

  template<class u2>
  constexpr auto operator-=(u2 b)
    -> std::enable_if_t<unitpp::is_unit_v<u2>
            && unitpp::same_unit_v<unit<unitpp::tmpl::X<Args...>, T>, u2>, decltype(*this)&>
  {
    val -= b.val;
    return *this;
  }

  // scalar operations
  template<class D>
  constexpr auto operator*=(D b)
  -> std::enable_if_t<std::is_arithmetic_v<unitpp::detail::result_helper_t<D>>
                   || std::is_same_v<unitpp::detail::result_helper_t<D>, T>,
                      decltype(*this)>
  {
    val *= b;
    return *this;
  }
  template<class D>
  constexpr auto operator/=(D b)
  -> std::enable_if_t<std::is_arithmetic_v<unitpp::detail::result_helper_t<D>>
                   || std::is_same_v<unitpp::detail::result_helper_t<D>, T>,
                      decltype(*this)>
  {
    val /= b;
    return *this;
  }

  //friends
  template<class IS, class M, class D>
  friend IS& operator>>(IS& is, unit<M, D>& u);

  template<class L1, class T1, class L2, class T2>
  friend constexpr auto unit_cast(unit<L2, T2> from)
            -> std::enable_if_t<unitpp::same_unit_v<unit<L1, T1>, unit<L2, T2>>, unit<L1, T1>>;
  template<class U1, class U2>
  friend constexpr auto unit_cast(U2 from)
            -> std::enable_if_t<unitpp::is_unit_v<U1> && unitpp::is_unit_v<U2> && !unitpp::same_unit_v<U1, U2>, U1>;
  template<class u1, class u2>
  friend constexpr auto operator+(u1 a, u2 b)
            -> std::enable_if_t<(unitpp::is_unit_v<u1> && unitpp::is_unit_v<u2>)
                              && unitpp::same_unit_v<u1, u2>, unitpp::detail::result_helper_t<u1>>;
  template<class u1, class u2>
  friend constexpr auto operator-(u1 a, u2 b)
            -> std::enable_if_t<(unitpp::is_unit_v<u1> && unitpp::is_unit_v<u2>)
                              && unitpp::same_unit_v<u1, u2>, unitpp::detail::result_helper_t<u1>>;
  template<class u1, class u2>
  friend constexpr auto operator*(u1 a, u2 b)
            -> std::enable_if_t<unitpp::is_unit_v<u1> && unitpp::is_unit_v<u2>,
                                unitpp::simplify_unit_t<unitpp::mesh_together_t<u1, u2>>>;
  template<class u1, class u2>
  friend constexpr auto operator/(u1 a, u2 b)
            -> std::enable_if_t<unitpp::is_unit_v<u1> && unitpp::is_unit_v<u2>,
                                unitpp::simplify_unit_t<unitpp::mesh_together_t<u1, unitpp::inverse_unit_t<u2>>>>;
  template<class u, class D>
  friend constexpr auto operator*(u a, D b)
            -> std::enable_if_t<std::is_arithmetic_v<unitpp::detail::result_helper_t<D>>
                            || (unitpp::is_unit_v<u> && std::is_same_v<unitpp::detail::result_helper_t<D>,
                                typename u::Default>), unitpp::detail::result_helper_t<u>>;
  template<class u, class D>
  friend constexpr auto operator*(D b, u a)
            -> std::enable_if_t<std::is_arithmetic_v<unitpp::detail::result_helper_t<D>>
                            || (unitpp::is_unit_v<u> && std::is_same_v<unitpp::detail::result_helper_t<D>,
                                typename u::Default>), unitpp::detail::result_helper_t<u>>;
  template<class u, class D>
  friend constexpr auto operator/(u a, D b)
            -> std::enable_if_t<std::is_arithmetic_v<unitpp::detail::result_helper_t<D>>
                            || (unitpp::is_unit_v<u> && std::is_same_v<unitpp::detail::result_helper_t<D>,
                                typename u::Default>), unitpp::detail::result_helper_t<u>>;
  template<class u, class D>
  friend constexpr auto operator/(D b, u a)
            -> std::enable_if_t<std::is_arithmetic_v<unitpp::detail::result_helper_t<D>>
                            || (unitpp::is_unit_v<u> && std::is_same_v<unitpp::detail::result_helper_t<D>,
                                typename u::Default>),
                  unitpp::inverse_unit_t<u>>;
private:
  T val;
};

template<class IS, class M, class D>
IS& operator>>(IS& is, unit<M, D>& u)
{ return is >> u.val; }


// conversion
template<class L1, class T1, class L2, class T2>
constexpr auto unit_cast(unit<L2, T2> from)
  -> std::enable_if_t<unitpp::same_unit_v<unit<L1, T1>, unit<L2, T2>>, unit<L1, T1>> // <- to
{
  return { from.val };
}

namespace unitpp
{
namespace detail
{
  template<class>
  struct convert_helper;

  template<class... Args>
  struct convert_helper<unitpp::tmpl::X<Args...>>
  {
    using hd = unitpp::tmpl::head<unitpp::tmpl::X<Args...>>;
    using tl = unitpp::tmpl::tail<unitpp::tmpl::X<Args...>>;

    // convert to base
    template<class T>
    constexpr static T to_base_measure(T t)
    {
      constexpr int e = hd::exponent < 0 ? (-1 * hd::exponent) : hd::exponent;
      T v = t;
      if constexpr(unitpp::is_converted_measure_v<hd>)
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
      if constexpr(unitpp::is_converted_measure_v<hd>)
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
    struct system_convert_helper<Garbage, tmpl::X<A1...>, tmpl::X<B1...>>
    {
      template<class F>
      using no_conv_measure = std::bool_constant<!is_converted_measure_v<F>>;
      static_assert(tmpl::forall_v<no_conv_measure, tmpl::X<A1...>>, "system_convert expects a converted-measure-free list");
      static_assert(tmpl::forall_v<no_conv_measure, tmpl::X<B1...>>, "system_convert expects a converted-measure-free list");
      
      using cur = tmpl::head<tmpl::X<A1...>>;
      
      template<class F>
      using same_measure = std::bool_constant<is_same_measure_type_v<cur, F> && is_same_kind_v<cur, F>>;

      template<class D>
      constexpr static D f(D d)
      {
        using found = tmpl::find<same_measure, tmpl::X<B1...>>;
        constexpr bool cond = found::value;
        if constexpr(cond)
        {
          // everything fine, go on (note that we assume a converted-measure free list)
          return d;
        }
        else
        {
          // unit *must* be converted with a system_conversion
          static_assert(exists_system_convert_v<cur, typename found::type>, "No system conversion existent");

          return system_convert<cur, typename found::type>()(d);
        }
      }
    };

    template<class Garbage>
    struct system_convert_helper<Garbage, tmpl::X<>, tmpl::X<>>
    {
      template<class D>
      constexpr static D f(D d)
      { return d; }
    };

    template<class T>
    constexpr static T system_conversions(T t)
    {
      using A = hd; // first list
      using B = tmpl::head<tl>; // second list

      using Ab = basify_converted_units_t<A>;
      using Bb = basify_converted_units_t<B>;

      return system_convert_helper<void, Ab, Bb>::f(t);
    }
  };
  template<>
  struct convert_helper<tmpl::X<>>
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
}

template<class U1, class U2>
constexpr auto unit_cast(U2 from)
  -> std::enable_if_t<unitpp::is_unit_v<U1> && unitpp::is_unit_v<U2> && !unitpp::same_unit_v<U1, U2>, U1> // <- to
{
  using A = unitpp::detail::explode_t<unitpp::detail::uncompoundify<unitpp::detail::measure_list_t<U1>>>;
  using B = unitpp::detail::explode_t<unitpp::detail::uncompoundify<unitpp::detail::measure_list_t<U2>>>;

  auto tmp = unitpp::detail::convert_helper<B>::to_base_measure(from.val);
  auto sys = unitpp::detail::convert_helper<unitpp::tmpl::X<A, B>>::system_conversions(tmp);
  auto res = unitpp::detail::convert_helper<A>::from_base_measure(sys);

  return { res };
}

template<class u1, class u2>
constexpr auto operator+(u1 a, u2 b)
  -> std::enable_if_t<(unitpp::is_unit_v<u1> && unitpp::is_unit_v<u2>) && unitpp::same_unit_v<u1, u2>,
                      unitpp::detail::result_helper_t<u1>>
{
  return { a.val + b.val };
}

template<class u1, class u2>
constexpr auto operator-(u1 a, u2 b)
  -> std::enable_if_t<unitpp::is_unit_v<u1> && unitpp::is_unit_v<u2> && unitpp::same_unit_v<u1, u2>,
                      unitpp::detail::result_helper_t<u1>>
{
  return { a.val - b.val };
}

// multiplication and division
template<class u1, class u2>
constexpr auto operator*(u1 a, u2 b)
  -> std::enable_if_t<unitpp::is_unit_v<u1> && unitpp::is_unit_v<u2>,
                      unitpp::simplify_unit_t<unitpp::mesh_together_t<u1, u2>>>
{
  return { a.val * b.val };
}

template<class u1, class u2>
constexpr auto operator/(u1 a, u2 b)
  -> std::enable_if_t<unitpp::is_unit_v<u1> && unitpp::is_unit_v<u2>,
                      unitpp::simplify_unit_t<unitpp::mesh_together_t<u1, unitpp::inverse_unit_t<u2>>>>
{
  return { a.val / b.val };
}

// scalar operations
template<class u, class T>
constexpr auto operator*(u a, T b)
  -> std::enable_if_t<std::is_arithmetic_v<unitpp::detail::result_helper_t<T>>
                  || (unitpp::is_unit_v<u> && std::is_same_v<unitpp::detail::result_helper_t<T>, typename u::Default>),
                      unitpp::detail::result_helper_t<u>>
{
  return { a.val * b };
}
template<class u, class D>
constexpr auto operator*(D b, u a)
  -> std::enable_if_t<std::is_arithmetic_v<unitpp::detail::result_helper_t<D>>
                  || (unitpp::is_unit_v<u> && std::is_same_v<unitpp::detail::result_helper_t<D>, typename u::Default>),
                      unitpp::detail::result_helper_t<u>>
{
  return { b * a.val };
}
template<class u, class T>
constexpr auto operator/(u a, T b)
  -> std::enable_if_t<std::is_arithmetic_v<unitpp::detail::result_helper_t<T>>
                  || (unitpp::is_unit_v<u> && std::is_same_v<unitpp::detail::result_helper_t<T>, typename u::Default>),
                      unitpp::detail::result_helper_t<u>>
{
  return { a.val / b };
}
template<class u, class T>
constexpr auto operator/(T b, u a)
  -> std::enable_if_t<std::is_arithmetic_v<unitpp::detail::result_helper_t<T>>
                  || (unitpp::is_unit_v<u> && std::is_same_v<unitpp::detail::result_helper_t<T>, typename u::Default>),
                      unitpp::inverse_unit_t<u>>
{
  return { b / a.val };
}
