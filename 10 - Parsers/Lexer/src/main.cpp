#include <array>
#include <cstddef>
#include <iostream>

#include "factory.hpp"

int main()
{
  constexpr std::size_t TEST_SIZE = 4;
  std::array<char const*, TEST_SIZE> const inputs = {
    "0", "20", "A", "INVALID"
  };
  std::array<bool, TEST_SIZE> const outputs = {true, true, false, false};

  Kalika::Factory factory;
  auto lexer = factory.get("");

  for (auto idx = 0UL; idx < TEST_SIZE; idx++) {
    std::cout << inputs[idx] << ": "
              << (lexer.test(inputs[idx]) == outputs[idx] ? "Passed"
                                                          : "Failed")
              << '\n';
  }

  return 0;
}
