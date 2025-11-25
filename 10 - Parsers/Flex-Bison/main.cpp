#include "driver.hpp"
#include "parser.hh"
#include "scanner.hpp"

#include <iostream>

int main()
{
  Kalika::Driver drv;
  int const res = drv.parse();
  std::cout << "Parse complete. Result = " << res << '\n';
  return 0;
}
