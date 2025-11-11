#include "circuit.hpp"

int main(int argc, const char **argv)
{
  const Circuit c((argc < 2) ? "./input/circuit-0.csv" : argv[1]);
  c.print();
}
