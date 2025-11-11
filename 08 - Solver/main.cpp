#include <cmath>
#include <iostream>

#include "solver.hpp"

int main()
{
  const IntegralSolver<Trapezoidal> ts;
  std::cout << "Trapezoidal: " << ts.solve(sin, 0, M_PI, 100) << '\n';

  const IntegralSolver<Simpsons> ss;
  std::cout << "Simpsons: " << ss.solve<>(sin, 0, M_PI, 100) << '\n';
}
