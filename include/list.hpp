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

#include <type_traits>

namespace me
{
template<class... T>
struct X;

template<class H, class... T>
struct X<H, T...>;

template<class H>
struct X<H>;

// checks whether the given thing is a X or not
template<typename T>
struct is_list : public std::false_type
{  };

template<typename... Args>
struct is_list<X<Args...>> : public std::true_type
{  };

template<typename T>
inline constexpr bool is_list_v = is_list<T>::value;

// adds an element to a list
template<class H, class T>
struct cons_helper;

template<class H, class... T>
struct cons_helper<H, X<T...>>
{
  using type = X<H, T...>;
};

template<class H, class T>
using cons = typename cons_helper<H, T>::type;


// glues two lists together
template<class H, class T>
struct conc_helper;

template<class... H, class... T>
struct conc_helper<X<H...>, X<T...> >
{
  using type = X<H..., T...>;
};

template<class H, class T>
using conc = typename conc_helper<H, T>::type;


// yields the head of a list
template<class H>
struct head_helper;

template<class H, class... Tail>
struct head_helper<X<H, Tail...>>
{
  using type = H;
};

template<class H>
using head = typename head_helper<H>::type;


// yields the tail of a list
template<class H>
struct tail_helper;

template<class H, class... Tail>
struct tail_helper<X<H, Tail...>>
{
  using type = X<Tail...>;
};

template<class H>
using tail = typename tail_helper<H>::type;

// finds the size of a list
template<class T>
struct length
{
  static constexpr std::size_t value = 1 + length<tail<T>>::value;
};
template<>
struct length<X<>>
{
  static constexpr std::size_t value = 0;
};
template<typename T>
constexpr std::size_t length_v = length<T>::value;

// yields true if list is empty
template<typename T>
using is_list_empty = std::is_same<T, X<>>;

template<typename T>
inline constexpr bool is_list_empty_v = std::is_same_v<T, X<>>;

// yields true if predicate for at least one elem in list is true
template<template<class> class P, class H>
struct find;

template<template<class> class P, class... H>
struct find<P, X<H...>> : public std::conditional_t<P<head<X<H...>>>::value,
                                                    std::true_type,
                                                    typename find<P, tail<X<H...>>>::type>
{  };

template<template<class> class P>
struct find<P, X<>> : public std::false_type
{  };

template<template<class> class P, class H>
inline constexpr bool find_v = find<P, H>::value;

// yields true if predicate is true for all members in the given list
template<template<class> class P, class H>
struct forall;

template<template<class> class P, class... H>
struct forall<P, X<H...>> : public std::conditional_t<!P<head<X<H...>>>::value,
                                                      std::false_type,
                                                      typename forall<P, tail<X<H...>>>::type>
{  };

template<template<class> class P>
struct forall<P, X<>> : public std::true_type
{  };

template<template<class> class P, class H>
inline constexpr bool forall_v = forall<P, H>::value;

// yields true if the class is in the list
template<class E, class H>
struct find_same;

template<class E, class... H>
struct find_same<E, X<H...>> : public std::conditional_t<std::is_same<E, head<X<H...>>>::value,
                                                         std::true_type,
                                                         typename find_same<E, tail<X<H...>>>::type>
{  };

template<class E>
struct find_same<E, X<>> : public std::false_type
{  };

template<class E, class H>
inline constexpr bool find_same_v = find_same<E, H>::value;


// yields true if the list has a list as elem
template<class H>
struct find_list;

template<class... H>
struct find_list<X<H...>> : public std::conditional_t<is_list<head<X<H...>>>::value,
                                                      std::true_type,
                                                      typename find_list<tail<X<H...>>>::type>
{  };

template<>
struct find_list<X<>> : public std::false_type
{  };

template<class H>
inline constexpr bool find_list_v = find_list<H>::value;


// removes an element from a list
template<class E, class H>
struct rm_helper;

template<class E, class... H>
struct rm_helper<E, X<H...>>
{
  using type = std::conditional_t<std::is_same<E, head<X<H...>>>::value,
                                  tail<X<H...>>,
                                  cons<head<X<H...>>,
                                       typename rm_helper<E, tail<X<H...>>>::type>
                                 >;
};

template<class E>
struct rm_helper<E, X<>>
{
  using type = X<>;
};

template<class E, class H>
using rm = typename rm_helper<E, H>::type;


// eliminates an element from a list
template<class E, class H>
struct elim_helper;

template<class E, class... H>
struct elim_helper<E, X<H...>>
{
  using el = typename elim_helper<E, tail<X<H...>>>::type;

  using type = std::conditional_t<std::is_same<head<X<H...>>, E>::value,
                                  el, cons<head<X<H...>>, el>
                                 >;

};

template<class E>
struct elim_helper<E, X<>>
{
  using type = X<>;
};

template<class E, class H>
using elim = typename elim_helper<E, H>::type;


// makes a set out of the type list
template<class H>
struct make_set_helper;

template<class... H>
struct make_set_helper<X<H...>>
{
  using type = cons<head<X<H...>>,
                    typename make_set_helper<elim<head<X<H...>>, X<H...>>>::type>;
};

template<>
struct make_set_helper<X<>>
{
  using type = X<>;
};

template<class H>
using make_set = typename make_set_helper<H>::type;


// computes the diff of two lists : a in A -> not a in inter
template<class H1, class H2>
struct inter_helper;

template<class... H1, class... H2>
struct inter_helper<X<H1...>, X<H2...>>
{
  using type = typename inter_helper<tail<X<H1...>>, rm<head<X<H1...>>, X<H2...>>>::type;
};

template<class... H2>
struct inter_helper<X<>, X<H2...>>
{
  using type = X<H2...>;
};

template<class H1, class H2>
using inter = typename inter_helper<H1, H2>::type;


// checks whether two lists share the same elements
template<class H1, class H2>
struct same_elements;

template<class... H1, class... H2>
struct same_elements<X<H1...>, X<H2...>>
{
  using set1 = make_set<X<H1...>>;
  using set2 = make_set<X<H2...>>;

  using type1 = inter<set1, set2>;
  using type2 = inter<set2, set1>;

  static constexpr const bool value = std::is_same<type1, type2>::value;
};

template<typename A, typename B>
inline constexpr bool same_elements_v = same_elements<A, B>::value;

/// Takes a list possibly containing lists and makes a single list out of them
template<class T>
struct universify_helper
{ using type = T; };//void; /*ugly*/ };

template<typename... T>
struct universify_helper<X<T...>>
{
  using hd = head<X<T...>>;
  using e = std::conditional_t<is_list<hd>::value,
                               typename universify_helper<hd>::type, X<hd>>;

  using type = conc<e, typename universify_helper<tail<X<T...>>>::type>;
};

template<>
struct universify_helper<X<>>
{
  using type = X<>;
};

template<class T>
using universify = typename universify_helper<T>::type;

/// filter
template<template<class> class R, class L>
struct filter;

template<template<class> class R, class... Args>
struct filter<R, X<Args...>>
{
  // if true, filter out
  using elem = std::conditional_t<!R<head<X<Args...>>>::value, X<head<X<Args...>>>, X<>>;

  using type = conc<elem, typename filter<R, tail<X<Args...>>>::type>;
};

template<template<class> class R>
struct filter<R, X<>>
{
  using type = X<>;
};

template<template<class> class R, class U>
using filter_t = typename filter<R, U>::type;

/// sorts the elements in a list by a predicate
template<template<class, class> class R, class L>
struct sort;

template<template<class, class> class R, class... Args>
struct sort<R, X<Args...>>
{
  using pivot = head<X<Args...>>;

  template<class U>
  using is_smaller = R<U, pivot>;

  template<class U>
  using isnt_smaller = std::integral_constant<bool, !R<U, pivot>::value>;

  // determine left and right list
  using left = filter_t<isnt_smaller, tail<X<Args...>>>;
  using right = filter_t<is_smaller, tail<X<Args...>>>;

  using type = conc<typename sort<R, left>::type,
                conc<X<pivot>,
                      typename sort<R, right>::type>>;
};

template<template<class, class> class R>
struct sort<R, X<>>
{
  using type = X<>;
};

template<template<class, class> class R, class L>
using sort_t = typename sort<R, L>::type;

}
