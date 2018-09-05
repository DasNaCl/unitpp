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

template<class u>
using result_helper_t = std::remove_reference_t<std::remove_cv_t<u>>;

template<class, class>
struct same_unit;
template<class L1, class T1, class L2, class T2>
struct same_unit<unit<L1, T1>, unit<L2, T2>> : public me::is_same_measure<L1, L2>
{  };
template<class A, class B>
inline constexpr bool same_unit_v = same_unit<result_helper_t<A>, result_helper_t<B>>::value;

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
  constexpr unit(T&& v) : val(v)
  {  }
  constexpr unit(const T& v) : val(v)
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
  constexpr auto operator+=(u2&& b)
    -> std::enable_if_t<me::same_unit_v<unit<me::X<Args...>, T>, u2>, decltype(*this)&>
  {
    val += b.raw();
    return *this;
  }

  template<class u2>
  constexpr auto operator+=(const u2& b)
    -> std::enable_if_t<me::same_unit_v<unit<me::X<Args...>, T>, u2>, decltype(*this)&>
  {
    val += b.raw();
    return *this;
  }

  template<class u2>
  constexpr auto operator-=(u2&& b)
    -> std::enable_if_t<me::same_unit_v<unit<me::X<Args...>, T>, u2>, decltype(*this)&>
  {
    val -= b.raw();
    return *this;
  }

  template<class u2>
  constexpr auto operator-=(const u2& b)
    -> std::enable_if_t<me::same_unit_v<unit<me::X<Args...>, T>, u2>, decltype(*this)&>
  {
    val -= b.raw();
    return *this;
  }
  // scalar operations
  template<class D>
  constexpr auto operator*=(D&& b)
  -> std::enable_if_t<std::is_arithmetic_v<D> || std::is_same_v<me::result_helper_t<D>, T>,
                      decltype(*this)>
  {
    val *= b;
    return *this;
  }
  template<class D>
  constexpr auto operator*=(const D& b)
  -> std::enable_if_t<std::is_arithmetic_v<D> || std::is_same_v<me::result_helper_t<D>, T>,
                      decltype(*this)>
  {
    val *= b;
    return *this;
  }
  template<class D>
  constexpr auto operator/=(D&& b)
  -> std::enable_if_t<std::is_arithmetic_v<D> || std::is_same_v<me::result_helper_t<D>, T>,
                      decltype(*this)>
  {
    val /= b;
    return *this;
  }
  template<class D>
  constexpr auto operator/=(const D& b)
  -> std::enable_if_t<std::is_arithmetic_v<D> || std::is_same_v<me::result_helper_t<D>, T>,
                      decltype(*this)>
  {
    val /= b;
    return *this;
  }
private:
  T val;
};

// conversion
template<class L1, class T1, class L2, class T2>
constexpr auto unit_cast(unit<L2, T2>&& from)
  -> std::enable_if_t<me::same_unit_v<unit<L1, T1>, unit<L2, T2>>, unit<L1, T1>> // <- to
{
  return { from.raw() };
}
template<class L1, class T1, class L2, class T2>
constexpr auto unit_cast(const unit<L2, T2>& from)
  -> std::enable_if_t<me::same_unit_v<unit<L1, T1>, unit<L2, T2>>, unit<L1, T1>> // <- to
{
  return { from.raw() };
}

namespace me::detail
{
  template<class, class>
  struct unit_converter;

  template<class L1, class T1, class L2, class T2>
  struct unit_converter<unit<L1, T1>, unit<L2, T2>>
  {
    template<class, class, class>
    struct helper;
    template<class Garbage, class... Args1, class... Args2>
    struct helper<Garbage, me::X<Args1...>, me::X<Args2...>>
    {
      static_assert(me::length_v<me::X<Args1...>> == me::length_v<me::X<Args2...>>,
                    "Amount of units should be equal");
      using type = me::X<Args2...>; // second list is the one we want to convert to

      constexpr static T1 f(T2 from)
      { return helper<Garbage, tail<me::X<Args1...>>,
                      tail<me::X<Args2...>>>::f(me::convert<me::head<me::X<Args1...>>,
                                                            me::head<me::X<Args2...>>,
                                                            T2>({ static_cast<T1>(from) }).raw()); }
    };
    template<class Garbage>
    struct helper<Garbage, me::X<>, me::X<>>
    {
      constexpr static T1 f(T2 from)
      { return static_cast<T1>(from); }
    };

    using nl1 = me::sort_t<me::detail::base_cmp, L1>;
    using nl2 = me::sort_t<me::detail::base_cmp, L2>;

    constexpr static unit<L1, T1> f(unit<L2, T2>&& t2)
    { return { helper<void, nl1, nl2>::f(t2.raw()) }; }
    constexpr static unit<L1, T1> f(const unit<L2, T2>& t2)
    { return { helper<void, nl1, nl2>::f(t2.raw()) }; }
  };

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

template<class U, class L2, class T2>
constexpr auto unit_cast(unit<L2, T2>&& from)
  -> std::enable_if_t<!me::same_unit_v<U, unit<L2, T2>>, U> // <- to
{
  using A = unit<me::detail::measure_list_t<U>, me::detail::underlying_type_t<U>>;
  using B = unit<L2, T2>;

  return { me::detail::unit_converter<A, B>::f(from) };
}
template<class U, class L2, class T2>
constexpr auto unit_cast(const unit<L2, T2>& from)
  -> std::enable_if_t<!me::same_unit_v<U, unit<L2, T2>>, U> // <- to
{
  using A = unit<me::detail::measure_list_t<U>, me::detail::underlying_type_t<U>>;
  using B = unit<L2, T2>;

  return { me::detail::unit_converter<A, B>::f(from) };
}
/////
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

//// operators
// addition and subtraction
template<class u1, class u2>
constexpr auto operator+(u1&& a, u2&& b)
  -> std::enable_if_t<same_unit_v<u1, u2>, result_helper_t<u1>>
{
  return { a.raw() + b.raw() };
}

template<class u1, class u2>
constexpr auto operator+(const u1& a, const u2& b)
  -> std::enable_if_t<same_unit_v<u1, u2>, result_helper_t<u1>>
{
  return { a.raw() + b.raw() };
}

template<class u1, class u2>
constexpr auto operator-(u1&& a, u2&& b)
  -> std::enable_if_t<same_unit_v<u1, u2>, result_helper_t<u1>>
{
  return { a.raw() - b.raw() };
}

template<class u1, class u2>
constexpr auto operator-(const u1& a, const u2& b)
  -> std::enable_if_t<same_unit_v<u1, u2>, result_helper_t<u1>>
{
  return { a.raw() - b.raw() };
}

// multiplication and division
template<class u1, class u2>
constexpr auto operator*(u1&& a, u2&& b)
  -> simplify_unit_t<mesh_together_t<u1, u2>>
{
  return { a.raw() * b.raw() };
}

template<class u1, class u2>
constexpr auto operator*(const u1& a, const u2& b)
  -> simplify_unit_t<mesh_together_t<u1, u2>>
{
  return { a.raw() * b.raw() };
}

template<class u1, class u2>
constexpr auto operator/(u1&& a, u2&& b)
  -> simplify_unit_t<mesh_together_t<u1, inverse_unit_t<u2>>>
{
  return { a.raw() / b.raw() };
}

template<class u1, class u2>
constexpr auto operator/(const u1& a, const u2& b)
  -> simplify_unit_t<mesh_together_t<u1, inverse_unit_t<u2>>>
{
  return { a.raw() / b.raw() };
}
// scalar operations
template<class u, class T>
constexpr auto operator*(u&& a, T&& b)
  -> std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<result_helper_t<T>, typename u::Default>,
                      result_helper_t<u>>
{
  return { a.raw() * b };
}
template<class u, class T>
constexpr auto operator*(T&& b, u&& a)
  -> std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<result_helper_t<T>, typename u::Default>,
                      result_helper_t<u>>
{
  return { b * a.raw() };
}
template<class u, class T>
constexpr auto operator/(u&& a, T&& b)
  -> std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<result_helper_t<T>, typename u::Default>,
                      result_helper_t<u>>
{
  return { a.raw() / b };
}
template<class u, class T>
constexpr auto operator/(T&& b, u&& a)
  -> std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<result_helper_t<T>, typename u::Default>,
                      inverse_unit_t<result_helper_t<u>>>
{
  return { b / a.raw() };
}

template<class u, class T>
constexpr auto operator*(const u& a, const T& b)
  -> std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<result_helper_t<T>, typename u::Default>,
                      result_helper_t<u>>
{
  return { a.raw() * b };
}
template<class u, class T>
constexpr auto operator*(const T& b, const u& a)
  -> std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<result_helper_t<T>, typename u::Default>,
                      result_helper_t<u>>
{
  return { b * a.raw() };
}
template<class u, class T>
constexpr auto operator/(const u& a, const T& b)
  -> std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<result_helper_t<T>, typename u::Default>,
                      result_helper_t<u>>
{
  return { a.raw() / b };
}
template<class u, class T>
constexpr auto operator/(const T& b, const u& a)
  -> std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<result_helper_t<T>, typename u::Default>,
                      inverse_unit_t<result_helper_t<u>>>
{
  return { b / a.raw() };
}

}
