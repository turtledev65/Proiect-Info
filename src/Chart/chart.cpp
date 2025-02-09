#include "chart.hpp"

namespace Chart
{
size_t BarChart::getTotal()
{
  const std::vector<ChartItem> &items = GetItems();
  size_t                        out   = 0;
  for (const ChartItem &item : items) {
    out += item.value;
  }
  return out;
}

void BarChart::Print()
{
  size_t                        total = getTotal();
  const std::vector<ChartItem> &items = GetItems();

  for (const ChartItem &item : items) {
    float percentage =
        static_cast<float>(item.value) / static_cast<float>(total);
    size_t length = percentage * BAR_LENGTH;

    std::cout << std::left << std::setw(40) << item.label;
    for (size_t j = 0; j < BAR_LENGTH; j++) {
      if (j < length) {
        std::cout << BAR_FULL_CHAR;
      } else {
        std::cout << BAR_EMPTY_CHAR;
      }
    }
    std::cout << ' ' << item.value << '\n';
  }
}

} // namespace Chart
