#ifndef PARSER_H
#define PARSER_H

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "Lexer.hpp"

/**
 * @brief LL1 Parser using predict sets and a parsing table
 *
 */
struct Parser {
  explicit Parser(Lexer& lexer) : m_lexer_(lexer) {}

  /**
   * @brief Load grammar from file and build the parsing table.
   */
  void load_grammar(std::string const& file_name, bool print = true);

private:
  using index = std::size_t;

  bool has_grammar_ = false;
  // Lexer to get tokens from
  Lexer& m_lexer_;

  // Keep count of symbols
  size_t nterms_ = 0;
  size_t terms_ = 0;

  // Holds a symbol in grammar
  struct Symbol {
    std::string name;
    bool const is_term;
  };

  // Mapping of symbols to numbers
  size_t start_index_ = 0;
  std::vector<Symbol> symbols_;
  std::unordered_map<std::string, size_t> sym_table_;

  // Grammar rules mapped using symbol indices
  struct Production {
    size_t lhs;
    std::vector<size_t> rhs;
  };

  // Store a list of all production rules
  std::vector<Production> grammar_;
  std::unordered_map<size_t, std::vector<size_t>> rule_list_;

  // Parsing Table used in parsing process
  struct Table {
    std::vector<size_t> elems;
    size_t m_rows;
    size_t m_cols;

    size_t const& operator()(size_t i, size_t j) const
    {
      size_t idx = i * m_cols + j;
      assert(idx < elems.size());
      return elems[idx];
    }

    size_t& operator()(size_t i, size_t j)
    {
      size_t idx = i * m_cols + j;
      assert(idx < elems.size());
      return elems[idx];
    }

    void resize(size_t rows, size_t cols, size_t val)
    {
      this->m_rows = rows;
      this->m_cols = cols;
      this->elems.resize(rows * cols);

      std::ranges::fill(this->elems, val);
    }
  };

  // Parsing table and sets
  std::unordered_map<size_t, std::set<size_t>> first_;
  std::unordered_map<size_t, std::set<size_t>> follow_;
  Table parsing_table_;

  // ====== Helper Members ====== //
  // Symbol Helpers
  template<std::forward_iterator It>
  void load_terms(It begin, It end, bool is_term, size_t& idx);
  void add_term(std::string const& sym_name, bool is_term, size_t idx);

  bool is_term(size_t idx) { return idx >= this->nterms_; }

  // Production helpers
  void build_rule(std::string const& line);
  std::vector<size_t> break_rule(std::string const& rule);

  // Parsing table helpers
  void build_table();
  void build_firsts();
  void build_follows();

  // Display helpers
  void print_results();
  void print_table();

  size_t sym_index(std::string const& name) const
  {
    return this->sym_table_.at(name);
  }

  std::string const& sym_name(size_t idx)
  {
    return this->symbols_[idx].name;
  }

  // ====== Static Members ====== //

  static constexpr char const* EPSILON = "null";
  static constexpr char const* EOL = "$";

  static std::string trim(std::string const& s);
};

#endif
