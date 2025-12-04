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
      size_t pos;
      bool final;

      explicit State(size_t index, bool state_final = false) :
        pos(index), final(state_final)
      {}

      /**
       * @brief Toggle the final status of the state
       */
      void toggle() { this->final = !final; }

      /**
       * @brief Add a transition to the state
       */
      void add_transition(char ch, State* state);

      /**
       * @brief Return transitions corresponding to ch
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

    struct DFAState {
      std::string name;
      bool final;

      explicit DFAState(
        std::string state_name,
        std::set<size_t> indices,
        bool state_final = false
      ) :
        name(std::move(state_name)),
        final(state_final),
        indices_(std::move(indices))
      {}

      /**
       * @brief Add a transition to the state
       */
      void add_transition(char ch, size_t state);

      /**
       * @brief Return transition corresponding to ch
       */
      [[nodiscard]] size_t get_transition(char ch) const;

      std::set<size_t> const& indices() const { return this->indices_; }

      // Helper methods
      bool is_mapped(char ch) { return this->transitions_.contains(ch); }

    private:
      std::unordered_map<char, size_t> transitions_;
      std::set<size_t> indices_;
    };

  }  //namespace internal

  /**
   * @brief Describes an NFA
   */
  struct NFA {
    std::deque<internal::State> storage;
    std::set<char> alphabet;
    size_t start;
    size_t end;

    /**
     * @brief Check if the input string matches the regexp
     */
    [[nodiscard]] bool match(std::string_view input) const;

    /**
     * @brief Return the epsilon closure of the state
     */
    [[nodiscard]] std::set<internal::State*> closure(size_t state_idx);

    /**
     * @brief Print statistics about the machine (For benchmarking)
     */
    void print_stats() const;
  };

  //TODO(kalika): Add DFA Minimization
  /**
   * @brief DFA constructed from an equivalent nFA
   */
  struct DFA {
    explicit DFA(NFA& nfa);

    /**
     * @brief Check if the machine accepts the input
     */
    [[nodiscard]] bool match(std::string_view input) const;

    /**
     * @brief Print the details of the machine (For Debugging)
     */
    void print() const;

    /**
     * @brief Print the stats of the machine (For benchmarking)
     */
    void print_stats() const;

  private:
    std::deque<internal::DFAState> storage_;
    std::unordered_map<std::string, size_t> state_map_;
    std::set<char> alphabet_;
    size_t start_;

    /**
     * @brief If state exists, returns it, else creates it
     */
    std::pair<bool, size_t>
    make_state(std::set<internal::State*> const& states);

    /**
     * @brief Get the state by name
     */
    size_t get_state(std::string& name);
  };

  // Helper methods
  std::string build_name(std::set<internal::State*> const& states);

}  //namespace Kalika

#endif
