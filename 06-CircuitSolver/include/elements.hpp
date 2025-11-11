#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <memory>
#include <vector>

class Branch;

/**
 * @brief Graph node denoting a nodal point in a circuit
 */
class Node
{
  double voltage_;
  std::vector<Branch *> branches_;

public:
};

/**
 * @brief Generic branch element denoting an edge in a circuit
 */
class Branch
{
  Node *from_;
  Node *to_;

public:
  Branch(Node *from, Node *to) : from_(from), to_(to) {}

  virtual ~Branch() = default;
  [[nodiscard]] virtual double stamp() const = 0;

  [[nodiscard]] virtual Node *from() const { return this->from_; };

  [[nodiscard]] virtual Node *to() const { return this->to_; };
};

/**
 * @brief Resistor element
 */
class Resistor : public Branch
{
  double resistance_;

public:
  Resistor(Node *from, Node *to, double resistance) :
    Branch(from, to), resistance_(resistance)
  {}

  [[nodiscard]] double stamp() const override { return 1.0 / resistance_; }
};

/**
 * @brief Current Source element
 */
class CurrentSource : public Branch
{
  double current_;

public:
  CurrentSource(Node *from, Node *to, double current) :
    Branch(from, to), current_(current)
  {}

  [[nodiscard]] double stamp() const override { return this->current_; }
};

#endif
