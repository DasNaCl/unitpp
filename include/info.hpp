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

#include <unit.hpp>

#include <string>

namespace unitpp
{

template<class U, class StrT>
struct unit_info;

template<class U, class StrT = std::string>
struct info
{
  info() : val()
  {  }
  template<class D, class = std::enable_if_t<is_unit_v<D>
                 && is_exact_same_measure_v<simplify_t<unitpp::detail::measure_list_t<U>,
                                                       unitpp::detail::underlying_type_t<U>, false, true>,
                     unitpp::simplify_t<unitpp::detail::measure_list_t<D>,
                                        unitpp::detail::underlying_type_t<D>, false, true>>>>
  info(D u) : val(std::to_string(u.raw()))
  {  }
  template<class OS, class D, class StrD>
  friend OS& operator<<(OS& os, const info<D, StrD>& inf);

  StrT value() const
  { return val; }
  StrT symbol() const
  { return unit_info<U, StrT>::symbol(); }
  StrT name() const
  { return unit_info<U, StrT>::name(); }
  StrT text() const
  { return unit_info<U, StrT>::text(); }
private:
  StrT val;
};

template<class OS, class D, class StrD>
OS& operator<<(OS& os, const info<D, StrD>& inf)
{ return os << inf.val << inf.text(); }

template<class, class>
struct measure_info;

// this class is meant for specialization
template<detail::MeasureType type, class kind, int exp, class StrT>
struct measure_info<measure<type, kind, exp>, StrT>
{
  constexpr static StrT symbol()
  { return "?"; }

  constexpr static StrT name()
  { return "{?}"; }
};

template<class... L, class B, class StrT>
struct unit_info<unit<tmpl::X<L...>, B>, StrT>
{
  constexpr static StrT symbol()
  {
    constexpr int exp = tmpl::head<tmpl::X<L...>>::exponent;
    if constexpr(tmpl::length_v<tmpl::X<L...>> > 1)
    {
      if constexpr(exp != 1)
      {
        return measure_info<tmpl::head<tmpl::X<L...>>, StrT>::symbol()
               + "^" + std::to_string(exp) + " "
               + unit_info<unit<tmpl::tail<tmpl::X<L...>>, B>, StrT>::symbol();
      }
      else
      {
        return measure_info<tmpl::head<tmpl::X<L...>>, StrT>::symbol()
               + " "
               + unit_info<unit<tmpl::tail<tmpl::X<L...>>, B>, StrT>::symbol();
      }
    }
    else
    {
      if constexpr(exp != 1)
        return measure_info<tmpl::head<tmpl::X<L...>>, StrT>::symbol()
               + "^" + std::to_string(exp);
      else
        return measure_info<tmpl::head<tmpl::X<L...>>, StrT>::symbol();
    }
  }
  constexpr static StrT name()
  {
    constexpr int exp = tmpl::head<tmpl::X<L...>>::exponent;
    if constexpr(tmpl::length_v<tmpl::X<L...>> > 1)
    {
      if constexpr(exp != 1)
      {
        return measure_info<tmpl::head<tmpl::X<L...>>, StrT>::name()
               + "^" + std::to_string(exp) + " "
               + unit_info<unit<tmpl::tail<tmpl::X<L...>>, B>, StrT>::name();
      }
      else
      {
        return measure_info<tmpl::head<tmpl::X<L...>>, StrT>::name()
               + " "
               + unit_info<unit<tmpl::tail<tmpl::X<L...>>, B>, StrT>::name();
      }
    }
    else
    {
      if constexpr(exp != 1)
        return measure_info<tmpl::head<tmpl::X<L...>>, StrT>::name() + "^" + std::to_string(exp);
      else
        return measure_info<tmpl::head<tmpl::X<L...>>, StrT>::name();
    }
  }
  constexpr static StrT text()
  {
    if constexpr(tmpl::length_v<tmpl::X<L...>> > 1)
      return "(" + symbol() + ")";
    else
      return symbol();
  }
protected:
  unit_info() = default;
  virtual ~unit_info() {}
};

}
