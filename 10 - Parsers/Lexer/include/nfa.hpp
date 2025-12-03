#ifndef NFA_H
#define NFA_H

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
    struct NFAState {
      bool final;
      std::string name;

      explicit NFAState(bool state_final = false) : final(state_final) {}

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
      void add_transition(char ch, NFAState* state_idx);

      /**
       * @brief Check if state accepts input
       */
      bool test(std::string_view input, std::size_t idx) const;

      /**
       * @brief Return all transitions from a state
       */
      [[nodiscard]] std::set<NFAState*> get_transition(char ch) const;

    private:
      umap<char, std::set<NFAState*>> transitions_;
      std::set<size_t> closure_set_;
    };

  }  //namespace internal

  /**
   * @brief Describes a NFA
   */
  struct NFA {
    std::deque<internal::NFAState> storage;
    std::set<char> alphabet;
    size_t start;
    size_t end;

    [[nodiscard]] bool test(std::string_view input) const;
  };
}  //namespace Kalika

#endif
