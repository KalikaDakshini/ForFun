#include <array>
#include <cstddef>
#include <iostream>

#include "factory.hpp"

int main(int argc, char const* argv[])
{
  // Implement reading text from a stream
  constexpr std::size_t TEST_SIZE = 4;
  std::array<char const*, TEST_SIZE> const inputs = {
    "aaaac",
    "bbbbc",
    "ababc",
    "abc",
  };
  std::array<bool, TEST_SIZE> const outputs = {false, false, true, true};

  Kalika::Factory factory;
  auto lexer = factory.get((argc < 2) ? "(ab)+c" : argv[1]);

  for (auto idx = 0UL; idx < TEST_SIZE; idx++) {
    std::cout << inputs[idx] << ": "
              << (lexer.test(inputs[idx]) == outputs[idx] ? "Passed"
                                                          : "Failed")
              << '\n';
  }

  return 0;
}
