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

#include <list.hpp>

namespace me
{

enum class MeasureType
{
  Single,
  Compound,
  Scaled,
};

///// This is a measure, basically a triple
template<MeasureType t, class X, int n>
struct measure
{
  static constexpr MeasureType type = t;
  using kind = X;
  static constexpr int exponent = n;
};

namespace detail
{
  template<template<class ...> class T, class E, class... Args>
  struct is_detected : public std::false_type
  {  };

  template<template<class ...> class T, class... Args>
  struct is_detected<T, std::void_t<T<Args...>>, Args...> : std::true_type
  {  };
}

template<template<class ...> class T, class... Args>
using is_detected = typename detail::is_detected<T, void, Args...>::type;

template<template<class ...> class T, class... Args>
constexpr bool is_detected_v = is_detected<T, Args...>::value;

template<class T>
using accept_measures_only_type = decltype(std::declval<T>().type);
template<class T>
using accept_measures_only_kind = typename T::kind;
template<class T>
using accept_measures_only_exponent = decltype(std::declval<T>().exponent);

// check if T is a measure or not
template<class T>
using is_measure = std::bool_constant<is_detected_v<accept_measures_only_kind, T>
                                   && is_detected_v<accept_measures_only_type, T>
                                   && is_detected_v<accept_measures_only_exponent, T>>;

template<class T>
constexpr bool is_measure_v = is_measure<T>::value;

// check if T is a compound measure
template<class T>
constexpr bool is_compound_measure_v = T::type == MeasureType::Compound;

// check if two measures share the same type
template<class A, class B>
using is_same_type = std::bool_constant<A::type == B::type>;

template<class A, class B>
constexpr bool is_same_type_v = is_same_type<A, B>::value;

// check if two measures share the same kind
template<class A, class B>
using is_same_kind = std::bool_constant<std::is_same_v<typename A::kind, typename B::kind>>;

template<class A, class B>
inline constexpr bool is_same_kind_v = std::is_same_v<typename A::kind, typename B::kind>;

// function to find compound measures
namespace detail
{
  template<class... Kind>
  auto exists_compound_func(X<Kind...>&&) -> void;
}
template<class... Kind>
struct exists_compound
{
  using type = decltype(detail::exists_compound_func(std::declval<X<Kind...>>()));
  
  static constexpr bool value = !std::is_same_v<type, void>;
};

template<class... Kind>
constexpr bool exists_compound_v = exists_compound<Kind...>::value;

template<class... Kind>
using exists_compound_t = typename exists_compound<Kind...>::type;

// adds exponents
template<class T, class L>
struct add_exp;

template<class T, class... Args>
struct add_exp<T, me::X<Args...>>
{
  using rec = typename add_exp<T, me::tail<me::X<Args...>>>::type;

  template<class A, class B>
  struct add
  {
    static_assert(is_measure_v<A>, "A should be a measure");
    static_assert(is_measure_v<B>, "B should be a measure");
    static_assert(is_same_type_v<A, B>, "A and B should be of the same type");

    using type = measure<A::type,
                         typename A::kind,
                         A::exponent + B::exponent>;
  };
  template<class A, class B>
  using add_t = typename add<A, B>::type;

  using hd = me::head<me::X<Args...>>;
  using type = std::conditional_t<is_same_kind_v<T, hd>,
                                  add_t<hd, rec>, rec>;
};

template<class T>
struct add_exp<T, me::X<>>
{
  // Fallback to single unit
  using type = measure<MeasureType::Single, T, 0>;
};

template<class T, class E>
using add_exp_t = typename add_exp<T, E>::type;

// universify compound measures
template<class T>
struct uncompoundify_helper
{ using type = T; /*ugly*/ };

template<typename... T>
struct uncompoundify_helper<X<T...>>
{
  using hd = head<X<T...>>;

  using e = std::conditional_t<is_compound_measure_v<hd>,
                               typename uncompoundify_helper<typename hd::kind>::type,
                               X<hd>>;
  using type = conc<e, typename uncompoundify_helper<tail<X<T...>>>::type>;
};

template<>
struct uncompoundify_helper<X<>>
{
  using type = X<>;
};

template<class T>
using uncompoundify = typename uncompoundify_helper<universify<T>>::type;

// simplify list of unit-kinds
template<class L>
struct simplify;

template<class... Args>
struct simplify<me::X<Args...>>
{
  using hd = me::head<me::X<Args...>>;
  using tl = me::tail<me::X<Args...>>;

  template<class B>
  using my_same_kind = is_same_kind<hd, B>;

  template<MeasureType t, class m, class l>
  struct intermediate_simplify
  {
    using added = add_exp_t<hd, me::X<Args...>>;
    using type = me::cons<added, typename simplify<me::filter_t<my_same_kind, tl>>::type>;
  };

  template<class m, class l>
  struct intermediate_simplify<MeasureType::Compound, m, l>
  {
    using new_list = me::conc<l, uncompoundify<X<m>>>;
    using type = typename simplify<new_list>::type;
  };

  using type = typename intermediate_simplify<hd::type, hd, tl>::type;
};

template<>
struct simplify<me::X<>>
{
  using type = me::X<>;
};

template<class E>
using is_exp_null = std::bool_constant<E::exponent == 0>;

template<class L>
using simplify_helper_t = me::filter_t<is_exp_null, typename simplify<me::universify<L>>::type>;

template<class L, class Default>
using simplify_t = std::conditional_t<std::is_same_v<simplify_helper_t<L>, me::X<>>,
                                        Default,
                                        simplify_helper_t<L>>; 

}
