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

#include <cstdint>

namespace unitpp
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
}
}
