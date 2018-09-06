> This readme is still **incomplete**!

# Introduction

unitpp is a easy to use C++ meta-library to add units into the language.
It was written as a hobby project in order to improve metaprogramming-skills.

Besides compile time, there should be no overhead if optimized.

### How-To
#### Quick Start
```
using namespace me;

constexpr auto impulse = 5_m * kg / s;
auto velocity = cm / ms;

std::cout << "Enter impulse (in " << info(impulse) << "): ";
std::cin >> impulse;

std::cout << "\nEnergy"
          << info<Joule<1>>(impulse * velocity) << std::endl;

auto energy = impulse * velocity | (opt::unscale | opt::combine | opt::basify);
static_assert(std::is_same_v<decltype(energy), decltype(kg * m<2> / s<2>)>,
              "Energy simplified correctly");
```

#### Defining own measures

There are several types of so called measures:

 1. `Single` - This represents a base unit, something like Second or Metre
 2. `Compound` - This is a collection of base units. More on that later
 3. `Scaled` - This is basically a scaled `Single` unit

##### Single
##### Compound
##### Scaled

### Dependencies

unitpp has no dependencies at all, besides C++17.

### Installation

Add everything contained in the `include/` directory to your project and you are ready to go!

**Free Software!**
