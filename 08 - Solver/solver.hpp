#ifndef SOLVER_H
#define SOLVER_H

#include <cstddef>
#include <utility>

// Method used to solve equations
template<typename T> struct Method {
  virtual ~Method() = default;

  template<typename F>
  double solve(F &&f, double a, double b, std::size_t divisions) const
  {
    auto derived = static_cast<const T &>(*this);
    return derived.solve(std::forward<F>(f), a, b, divisions);
  }

private:
  Method() = default;
  friend T;
};

// Solve integrals
template<typename T> struct IntegralSolver {
  IntegralSolver() = default;

  template<typename F>
  double solve(F &&f, double a, double b, std::size_t divisions = 100) const
  {
    return method_.solve(std::forward<F>(f), a, b, divisions);
  }

private:
  T method_;
};

struct Trapezoidal : Method<Trapezoidal> {
  template<typename F>
  double solve(F &&f, double a, double b, std::size_t divisions) const;
};

template<typename F>
double
Trapezoidal::solve(F &&f, double a, double b, std::size_t divisions) const
{
  const double step = (b - a) / static_cast<double>(divisions);

  double result = 0;
  for (double start = a; start <= (b - step); start += step) {
    result += step * (f(start) + f(start + step)) / 2;
  }

  return result;
}

struct Simpsons : Method<Simpsons> {
  template<typename F>
  double solve(F &&f, double a, double b, std::size_t divisions) const;
};

template<typename F>
double Simpsons::solve(F &&f, double a, double b, std::size_t divisions) const
{
  const double step = (b - a) / static_cast<double>(divisions);

  double result = 0;
  for (double start = a; start <= (b - step); start += step) {
    result +=
      step * (f(start) + f(start + step) + 4 * f(start + (step / 2))) / 6;
  }

  return result;
}

#endif
