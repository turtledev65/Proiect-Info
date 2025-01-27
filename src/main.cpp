#include <iostream>
#include <fstream>
#include "Csv/parser.hpp"

int main()
{
  std::ifstream file("res/tes.csv");
  Csv::Parser parser(file);
  Csv::Field field = parser.nextField();
  std::cout << field.data << std::endl;
}
