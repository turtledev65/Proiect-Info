#pragma once

#include <cstddef>

namespace Statistics
{
struct Sex {
  explicit Sex(size_t m, size_t f) : male(m), female(f) {};

  size_t male   = 0;
  size_t female = 0;

  size_t GetTotal() const { return male + female; };
};
} // namespace Statistics
