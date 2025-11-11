#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <iostream>
#include <memory>
#include <string_view>
#include <vector>

#include "elements.hpp"

class Circuit
{
  std::vector<std::unique_ptr<Node>> nodes_;
  std::vector<std::unique_ptr<Branch>> branches_;

public:
  explicit Circuit(std::string_view file) : nodes_(), branches_()
  {
    std::cout << file << '\n';
  };

  // public methods
  void print() const { std::cout << "Printing Circuit\n"; };
};

#endif
