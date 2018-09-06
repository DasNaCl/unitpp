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
#include <cstdint>

namespace me
{

namespace detail
{
  constexpr std::uint64_t CRC64(const char* chars, std::uint64_t previous_hash = std::uint64_t(5381))
  {
    return (chars[0] == '\0') ? previous_hash : CRC64(&chars[1], previous_hash * 33 ^ std::uint64_t(chars[0]));
  }
  template<std::size_t Length>
  constexpr std::uint64_t CRC64(const char(&chars)[Length])
  {
    std::uint64_t hash = 5381;

    for(int i = 0; i < Length; ++i)
      hash = hash * 33 ^ std::uint64_t(chars[i]);

    return hash;
  }

  template<class T>
  struct times_10
  {
    constexpr auto operator()(T val) const
    { return val * static_cast<T>(10); }
  };

  template<class T>
  struct by_10
  {
    constexpr auto operator()(T val) const
    { return val / static_cast<T>(10); }
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

template<MeasureType t, class X, int n>
struct measure
{
  static constexpr MeasureType type = t;
  using kind = X;
  static constexpr int exponent = n;

  template<typename V>
  static constexpr std::uint64_t h()
  {
    if constexpr(me::is_list_v<V>)
    {
      if constexpr(me::is_list_empty_v<V>)
        return 61;
      else
        return h<me::head<V>>() ^ (h<me::tail<V>>() << 11);
    }
    else
      return V::id;
  }
  template<typename V>
  static constexpr std::uint64_t f()
  {
    if constexpr(me::is_list_v<V>)
    {
      if constexpr(me::is_list_empty_v<V>)
        return 61;
      else
        return f<me::head<V>>() ^ (f<me::tail<V>>() << 11);
    }
    else
      return V::base_id;
  }
  static constexpr std::uint64_t id = std::integral_constant<std::uint64_t, h<X>()>::value;
  static constexpr std::uint64_t base_id = std::integral_constant<std::uint64_t, f<X>()>::value;
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
template<class T>
using accept_measures_only_id = decltype(std::declval<T>().id);
template<class T>
using accept_measures_only_base_id = decltype(std::declval<T>().base_id);

// check if T is a measure or not
template<class T>
using is_measure = std::bool_constant<is_detected_v<accept_measures_only_kind, T>
                                   && is_detected_v<accept_measures_only_type, T>
                                   && is_detected_v<accept_measures_only_exponent, T>
                                   && is_detected_v<accept_measures_only_id, T>
                                   && is_detected_v<accept_measures_only_base_id, T>>;

template<class T>
constexpr bool is_measure_v = is_measure<T>::value;

// check if T is a compound measure
template<class T>
constexpr bool is_compound_measure_v = T::type == MeasureType::Compound;

// check if T is a converted measure
template<class T>
constexpr bool is_converted_measure_v = T::type == MeasureType::Converted;

// check if T is a base measure
template<class T>
constexpr bool is_base_measure_v = T::type == MeasureType::Base;

// inverses the exponents of given measures
template<class L>
struct inverse_measures;
template<class... Args>
struct inverse_measures<me::X<Args...>>
{
  using m = typename me::head<me::X<Args...>>;
  using tl = me::tail<me::X<Args...>>;
  using type = me::cons<measure<m::type, typename m::kind, -1 * m::exponent>,
                        typename inverse_measures<tl>::type>;
};
template<>
struct inverse_measures<me::X<>>
{
  using type = me::X<>;
};

template<class L>
using inverse_measures_t = typename inverse_measures<L>::type;

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

template<class A, class B>
using is_same_measure = me::same_elements<A, B>;

template<class A, class B>
inline constexpr bool is_same_measure_v = me::same_elements_v<A, B>;

// splits all unit concepts into exponents of magnitude 1
template<class T>
struct explode_single
{
  constexpr static int sign = (T::exponent > 0 ? 1 : -1);
  constexpr static int exp = sign * T::exponent; // <- sneaky way to get the abs
  using M = measure<T::type, typename T::kind, sign>;
  using R = measure<T::type, typename T::kind, sign * (exp - 1)>;

  template<int e, class>
  struct helper
  {
    using type = me::cons<M, typename explode_single<R>::type>;
  };
  template<class Junk>
  struct helper<0, Junk>
  {
    using type = X<>;
  };

  using type = typename helper<exp, void>::type;
};

template<class T>
using explode_single_t = typename explode_single<T>::type;

template<class T>
struct explode;

template<class... Args>
struct explode<X<Args...>>
{
  using type = me::conc<explode_single_t<head<X<Args...>>>,
                        typename explode<tail<X<Args...>>>::type>;
};

template<>
struct explode<X<>>
{
  using type = X<>;
};

template<class T>
using explode_t = typename explode<T>::type;

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
  using type = measure<MeasureType::Base, T, 0>;
};

template<class T, class E>
using add_exp_t = typename add_exp<T, E>::type;

// universify compound measures
template<class T>
struct uncompoundify_helper;

template<typename... D>
struct uncompoundify_helper<X<D...>>
{
  template<class H, int val>
  struct helper
  { using type = H; /*ugly*/ };

  template<class... T, int exp>
  struct helper<me::X<T...>, exp>
  {
    using hd = head<X<T...>>;

    using obj = std::conditional_t<is_compound_measure_v<hd>,
                                   typename helper<typename hd::kind, exp * hd::exponent>::type,
                                   X<measure<hd::type, typename hd::kind, exp * hd::exponent>>>;
    using type = conc<obj, typename helper<tail<X<T...>>, exp>::type>;
  };
  template<int exp>
  struct helper<me::X<>, exp>
  {
    using type = me::X<>;
  };

  using type = typename helper<X<D...>, 1>::type;
};

template<>
struct uncompoundify_helper<X<>>
{
  using type = X<>;
};

template<class T>
using uncompoundify = typename uncompoundify_helper<universify<T>>::type;

// function to find compound measures
template<class>
struct exists_compound;
template<class... Kind>
struct exists_compound<me::X<Kind...>> : public std::false_type
{
  using type = me::X<Kind...>;
};

template<class List>
constexpr bool exists_compound_v = exists_compound<List>::value;

template<class List>
using exists_compound_t = typename exists_compound<List>::type;

// check if given type is a valid measure kind
template<class K>
using measure_kind_type_of_id = decltype(std::declval<K>().id);
template<class M>
using measure_has_valid_kind = std::bool_constant<is_detected_v<measure_kind_type_of_id, M>>;
template<class M>
constexpr bool measure_has_valid_kind_v = measure_has_valid_kind<M>::value;

// compoundify measures
template<bool uncompoundify, class L>
struct simplify; // <- we need to declare it here to be able to use it in compoundify

template<class T>
struct compoundify;

namespace detail
{
  template<class A, class B>
  using kind_cmp = std::bool_constant<(A::id < B::id)>;

  template<class A, class B>
  using base_cmp = std::bool_constant<(A::base_id < B::base_id)>;

  template<class L>
  using compoundify_op = explode_t<sort_t<kind_cmp, L>>;

  // do the compoundify check if some compound for a given thing exists
  template<class T>
  struct check
  {
    using sorted = typename simplify<false, T>::type;
    using new_list = me::sort_t<detail::kind_cmp, sorted>;

    // either this is shortened or the same as before
    using intermediate = exists_compound_t<new_list>;

    constexpr static bool value = exists_compound_v<new_list>;
    using type = compoundify_op<std::conditional_t<value, intermediate, new_list>>;
  };

  template<class List, int chunk>
  struct single_rm_A;
  template<class List, int chunk, std::size_t rm, bool condition>
  struct single_rm_B;

  template<int chunk, class W>
  struct single_rm_E
  {
    using tmp = single_rm_A<W, chunk - 1>;

    using type = typename tmp::type;
    constexpr static bool value = tmp::value;
  };
  template<class W>
  struct single_rm_E<0, W>
  {
    using tmp = check<W>;

    using type = typename tmp::type;
    constexpr static bool value = tmp::value;
  };

  template<class List, int chunk, std::size_t rm, class W>
  struct single_rm_D
  {
    using bail_out = single_rm_E<chunk, W>;

    template<class Garbage, bool v>
    struct helper
    {
      struct R
      {
        constexpr static bool value = true;
        using type = compoundify_op<me::cons<me::nth_t<rm, List>, typename bail_out::type>>;
      };
      using type = R;
    };
    template<class Garbage>
    struct helper<Garbage, false>
    {
      using type = single_rm_B<List, chunk, rm + 1, ((rm + 1) < me::length_v<List>)>;
    };

    using tmp = typename helper<void, bail_out::value>::type;

    constexpr static bool value = tmp::value;
    using type = typename tmp::type;
  };

  template<class List, int chunk, std::size_t rm, bool condition>
  struct single_rm_C
  {
    using w = me::erase_t<rm, List>;

    using tmp = single_rm_D<List, chunk, rm, w>;

    using type = typename tmp::type;
    constexpr static bool value = tmp::value;
  };
  template<class List, int chunk, std::size_t rm>
  struct single_rm_C<List, chunk, rm, false>
  {
    using tmp = single_rm_B<compoundify_op<List>, chunk, rm + 1, (rm + 1 < me::length_v<compoundify_op<List>>)>;

    using type = typename tmp::type;
    constexpr static bool value = tmp::value;
  };


  template<class List, int chunk, std::size_t rm, bool condition>
  struct single_rm_B
  {
    using tmp = single_rm_C<compoundify_op<List>, chunk, rm, (me::length_v<compoundify_op<List>> > 1)>;

    using type = typename tmp::type;
    constexpr static bool value = tmp::value;
  };
  template<class List, int chunk, std::size_t rm>
  struct single_rm_B<List, chunk, rm, false>
  {
    using type = List;
    constexpr static bool value = false;
  };

  template<class List, int chunk>
  struct single_rm_A // recurse
  {
    using res_t = single_rm_B<compoundify_op<List>, chunk, 0, (0 < me::length_v<compoundify_op<List>>)>;

    using type = typename res_t::type;
    constexpr static bool value = res_t::value;
  };
  template<class List>
  struct single_rm_A<List, -1> // recursion stop
  {
    using tmp = check<List>;

    struct res
    {
      constexpr static bool value = true;
      using type = typename tmp::type;
    };
    template<class H>
    constexpr static auto f()
      -> std::enable_if_t<H::value, res>;

    template<class H>
    constexpr static auto f()
      -> std::enable_if_t<!H::value, single_rm_A<compoundify_op<List>, 0>>;

    using res_t = decltype(f<tmp>());
    
    using type = typename res_t::type;
    constexpr static bool value = res_t::value;
  };
}

template<typename... T>
struct compoundify<X<T...>>
{
  template<int chunk, bool loop_cond, class List>
  struct helper
  {
    using new_t = detail::single_rm_A<detail::compoundify_op<List>, chunk>;

    constexpr static bool bail_out = new_t::value;
    constexpr static int next_chunk = (bail_out ? -1 : (chunk + 1));

    using new_list = typename new_t::type;
    using type = typename helper<next_chunk,
                                 (static_cast<unsigned int>(next_chunk) < me::length_v<new_list>),
                                 detail::compoundify_op<new_list>>::type;
  };
  template<int chunk, class List>
  struct helper<chunk, false, List>
  {
    using type = me::sort_t<detail::kind_cmp, List>;
  };

  using tmp = typename helper<-1, true, X<T...>>::type;
  using sorted = typename simplify<false, tmp>::type;

  using type = sorted;
};

template<>
struct compoundify<X<>>
{
  using type = X<>;
};

template<typename L>
using compoundify_t = typename compoundify<detail::compoundify_op<L>>::type;

template<class From, class To>
struct system_convert_check
{
  static_assert(me::is_measure_v<From>, "From should be a measure");
  static_assert(me::is_measure_v<To>, "To should be a measure");

  // make sure you only get measures here
};
template<class From, class To>
struct system_convert : public std::false_type
{
  using type = To;

  template<typename D = std::false_type>
  constexpr void operator()(To)
  { static_assert(D::value, ""); }
};

// can be used for e.g. meter to foot
#define unitpp_declare_system_convert(From, To, var)                                           \
template<>                                                                                     \
struct system_convert<From, To> : public std::true_type, public system_convert_check<From, To> \
{                                                                                              \
  using type = To;                                                                             \
  template<class T>                                                                            \
  constexpr T operator()(T) const;                                                             \
};                                                                                             \
template<> constexpr To system_convert<From, To>::operator()(To var) const

template<typename T>
struct system_convert<T, T> : public std::true_type, public system_convert_check<T, T>
{
  using type = T;

  template<class D>
  constexpr D operator()(D&& id) const
  { return id; }
};

template<class From, class To>
constexpr bool exists_system_convert_v = system_convert<From, To>::value;

// simplify list of unit-kinds
template<bool shall_uncompoundify, class... Args>
struct simplify<shall_uncompoundify, me::X<Args...>>
{
  using hd = me::head<me::X<Args...>>;
  using tl = me::tail<me::X<Args...>>;

  template<class B>
  using my_same_kind = is_same_kind<hd, B>;

  template<MeasureType t, class m, class l>
  struct intermediate_simplify
  {
    using added = add_exp_t<hd, me::X<Args...>>;
    using type = me::cons<added, typename simplify<shall_uncompoundify, me::filter_t<my_same_kind, tl>>::type>;
  };

  template<class m, class l>
  struct intermediate_simplify<MeasureType::Compound, m, l>
  {
    using new_list = me::conc<l, uncompoundify<X<m>>>;
    using type = typename simplify<shall_uncompoundify, new_list>::type;
  };

  using type = typename intermediate_simplify<(shall_uncompoundify ? hd::type : MeasureType::Base),
                                               hd, tl>::type;
};

template<bool shall_uncompoundify>
struct simplify<shall_uncompoundify, me::X<>>
{
  using type = me::X<>;
};

template<class E>
using is_exp_null = std::bool_constant<E::exponent == 0>;

template<class L, bool condition>
using compoundifier = std::conditional_t<condition, compoundify_t<L>, L>;

template<class L, bool shall_uncompoundify>
using simplify_helper_t = me::filter_t<is_exp_null, typename simplify<shall_uncompoundify, me::universify<L>>::type>;

template<class L, class Default, bool shall_compoundify = false, bool shall_uncompoundify = true>
using simplify_t = std::conditional_t<std::is_same_v<simplify_helper_t<L, shall_uncompoundify>, me::X<>>,
                                      Default,
                                      compoundifier<simplify_helper_t<L, shall_uncompoundify>, shall_compoundify>>;

template<class>
struct basify_converted_units;

template<class... Args>
struct basify_converted_units<me::X<Args...>>
{
  using hd = me::head<me::X<Args...>>;

  template<class H>
  struct helper
  { using type = H; };

  template<class H, int n>
  struct helper<measure<MeasureType::Converted, H, n>>
  {
    using type = typename H::Base;
  };

  using type = me::conc<me::X<typename helper<hd>::type>,
                        typename basify_converted_units<me::tail<me::X<Args...>>>::type>;
};

template<>
struct basify_converted_units<me::X<>>
{
  using type = me::X<>;
};

// todo: support converted compound units
template<class L>
using basify_converted_units_t = typename basify_converted_units<me::uncompoundify<L>>::type;

}
