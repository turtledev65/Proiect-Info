#pragma once

#include <cstddef>

namespace Statistics
{
struct Sex {
  Sex(size_t m = 0, size_t f = 0) : male(m), female(f) {};

  size_t male   = 0;
  size_t female = 0;

  size_t getTotal() const { return male + female; };
};
} // namespace Statistics
