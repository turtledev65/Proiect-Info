#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

namespace Chart
{

struct ChartItem {
  explicit ChartItem(size_t value, const std::string &label)
      : value(value), label(std::move(label))
  {
  }

  size_t            value;
  const std::string label;
};

class BaseChart
{
public:
  explicit BaseChart(const std::vector<ChartItem> &items)
      : m_Items(std::move(items))
  {
  }
  virtual ~BaseChart() noexcept = default;

  virtual void Print() = 0;

protected:
  const std::vector<ChartItem> &GetItems() { return m_Items; };
  size_t                        GetTotal();

private:
  const std::vector<ChartItem> m_Items;
};

class BarChart : BaseChart
{
public:
  explicit BarChart(const std::vector<ChartItem> &items) : BaseChart(items) {}
  void Print() override;

private:
  static constexpr size_t BAR_LENGTH     = 20;
  static constexpr char   BAR_EMPTY_CHAR = '-';
  static constexpr char   BAR_FULL_CHAR  = '+';
};

class PieChart : BaseChart
{
public:
  explicit PieChart(const std::vector<ChartItem> &items) : BaseChart(items) {}
  void Print() override;

private:
  static constexpr size_t WIDTH       = 30;
  static constexpr size_t HEIGHT      = 12;
  static constexpr char   FIRST_CHAR  = '*';
  static constexpr char   SECOND_CHAR = '%';
};

} // namespace Chart
