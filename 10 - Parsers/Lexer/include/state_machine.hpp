#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <deque>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace Kalika
{

  template<typename T, typename U> using umap = std::unordered_map<T, U>;
  using std::size_t;

  static constexpr char const EPS = '\0';

  namespace internal
  {

    /**
     * @brief Describes a single state of the NFA
     */
    struct State {
      bool final;
      std::string name;

      explicit State(bool state_final = false) : final(state_final) {}

      /**
       * @brief Toggle the final status of the state
       */
      void toggle() { this->final = !final; }

      /**
       * @brief Set the name object
       */
      void set_name(std::string state_name)
      {
        this->name = std::move(state_name);
      }

      /**
       * @brief Add a transition to the state
       */
      void add_transition(char ch, State* state_idx);

      /**
       * @brief Return all transitions from a state
       */
      [[nodiscard]] std::set<State*> get_transition(char ch) const;

      /**
       * @brief Check if state accepts input
       */
      bool match(std::string_view input, std::size_t idx) const;

      /**
       * @brief Return the closure set of the current state
       */
      std::set<State*> closure();

    private:
      umap<char, std::set<State*>> transitions_;
      std::set<State*> closure_set_;
    };

  }  //namespace internal

  //TODO(kalika): Add DFA Minimization
  /**
   * @brief Describes an NFA
   */
  struct StateMachine {
    std::deque<internal::State> storage;
    std::set<char> alphabet;
    size_t start;
    size_t end;

    [[nodiscard]] bool match(std::string_view input) const;
  };

}  //namespace Kalika

#endif
