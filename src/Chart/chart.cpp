#include "chart.hpp"

namespace Chart
{

size_t BaseChart::GetTotal()
{
  const std::vector<ChartItem> &items = GetItems();
  size_t                        out   = 0;

  for (size_t i = 0; i < items.size(); i++) {
    out += items[i].value;
  }
  return out;
}

void BarChart::Print()
{
  size_t                        total = GetTotal();
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

void PieChart::Print()
{
  const std::vector<ChartItem> &items = GetItems();
  if (items.size() != 2) {
    return;
  }
  size_t total = GetTotal();

  float firstPercentage =
      static_cast<float>(items[0].value) / static_cast<float>(total);
  float  secondPercentage = 100.0f - firstPercentage;
  size_t firstLength      = firstPercentage * WIDTH;

  std::cout << std::setw(WIDTH - items[1].label.size()) << std::left
            << items[0].label << items[1].label << '\n'
            << std::setw(WIDTH - items[1].label.size()) << std::left
            << items[0].value << items[1].value << '\n';

  for (size_t i = 0; i < HEIGHT; i++) {
    for (size_t j = 0; j < WIDTH; j++) {
      if (j < firstLength) {
        std::cout << FIRST_CHAR;
      } else {
        std::cout << SECOND_CHAR;
      }
    }

    std::cout << '\n';
  }
}

} // namespace Chart
