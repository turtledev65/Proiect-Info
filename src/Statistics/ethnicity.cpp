#pragma once

#include "ethnicity.hpp"

namespace Statistics
{
std::string Ethnicity::toJSON() const
{
  std::stringstream stream;
  stream << '{';
  stream << "\"type\": " << static_cast<size_t>(type) << ','
         << "\"total\": " << sex.getTotal();
  stream << '}';
  return stream.str();
}
} // namespace Statistics
