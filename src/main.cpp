#include <iostream>
#include <fstream>

int main()
{
  std::ifstream file("res/Recensamant2021.csv");
  if (file.is_open()) {
    std::cout << "File opened" << std::endl;
  } else {
    std::cout << "File not found!" << std::endl;
  }
  std::cin.get();
}
