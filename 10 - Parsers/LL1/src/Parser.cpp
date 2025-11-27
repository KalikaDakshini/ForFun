#include "Parser.hpp"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <ranges>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

void Parser::load_grammar(std::string const& file_name, bool print)
{
  this->has_grammar_ = true;

  std::ifstream gr_file{file_name};

  if (!gr_file) {
    throw std::runtime_error("Grammar file not found");
  }

  std::string line;
  size_t idx = 0;
  while (std::getline(gr_file, line)) {
    // Ignore empty lines
    line = trim(line);

    // Discard empty lines
    if (line.empty()) {
      continue;
    }
    // Discard comments
    if (line.starts_with("//")) {
      continue;
    }
    // Add non-terminals
    if (line.starts_with("%nterm")) {
      load_terms(line.begin() + 6, line.end(), false, idx);
      continue;
    }
    // Add terminals
    if (line.starts_with("%term")) {
      load_terms(line.begin() + 5, line.end(), true, idx);
      continue;
    }
    // Parse grammar rules
    build_rule(line);
  }

  build_table();

  // Print table if requested
  if (print) {
    print_results();
  }
}

// ======== Private Members ======== //
// Load terms from the grammar file into a table
template<std::forward_iterator It>
void Parser::load_terms(It begin, It end, bool is_term, size_t& idx)
{
  std::string_view const line_view(begin, end);

  // Split at spaces and iterate over words
  for (auto const& word_range : line_view | std::views::split(' ')) {
    // Ignore empty strings
    if (word_range.begin() == word_range.end()) {
      continue;
    }

    // Add symbol to table
    add_term({word_range.begin(), word_range.end()}, is_term, idx++);
  }

  // Add null and EOL to terminals
  // Note: null and EOL are the last two symbols in the list
  if (is_term) {
    add_term(EOL, is_term, idx++);
    add_term(EPSILON, is_term, idx++);
  }

  // Update symbol counts
  if (is_term) {
    this->terms_ = idx - this->nterms_;
  }
  else {
    this->nterms_ = idx;
  }
}

void Parser::add_term(
  std::string const& sym_name, bool is_term, size_t idx
)
{
  this->symbols_.emplace_back(
    Symbol{.name = sym_name, .is_term = is_term}
  );
  this->sym_table_.emplace(sym_name, idx);
}

// Break a rule into symbols
std::vector<size_t> Parser::break_rule(std::string const& rule)
{
  std::vector<size_t> indices;

  if (rule.empty()) {
    indices.push_back(this->sym_index(EPSILON));
    return indices;
  }

  for (auto const& word_range : rule | std::views::split(' ')) {
    indices.push_back(
      this->sym_index({word_range.begin(), word_range.end()})
    );
  }

  return indices;
}

// Build productions from a rule
void Parser::build_rule(std::string const& line)
{
  // Get the non-terminal
  auto rule_view = line | std::views::split(':');
  auto it = rule_view.begin();
  auto lead_idx = this->sym_index(trim({(*it).begin(), (*it).end()}));

  // Left side of the string
  it++;
  std::string rhs{trim({(*it).begin(), (*it).end()})};

  // Get the rules
  for (auto const& word_range : rhs | std::views::split('|')) {
    // Break rule into list of indices
    auto sym_list =
      break_rule(trim({word_range.begin(), word_range.end()}));

    // Add production to rule
    this->grammar_.emplace_back(
      Production{.lhs = lead_idx, .rhs = sym_list}
    );
    this->rule_list_[lead_idx].push_back(this->grammar_.size() - 1);
  }
}

void Parser::build_table()
{
  // Build first and follow sets
  this->build_firsts();
  this->build_follows();

  // Resize table to hold entries
  this->parsing_table_.resize(
    this->nterms_, this->terms_, this->terms_ + this->nterms_
  );

  // Make an iter view of productions
  auto iter_view =
    this->grammar_ |
    std::views::transform([idx = 0](auto const& elem) mutable {
      return std::make_pair(idx++, elem);
    });

  for (auto const& [prod_idx, prod] : iter_view) {
    // Get the first set of the production rule
    auto nterm_idx = prod.lhs;
    auto const& first_set = this->first_[prod.rhs[0]];
    auto const& follow_set = this->follow_[nterm_idx];

    for (auto idx = 0UL; idx < this->terms_; idx++) {
      auto term_idx = this->nterms_ + idx;
      // For [A, a] the production rule A -> w if a is in First(w)
      bool const cond = first_set.contains(term_idx) ||
                        // or null in First(w) and a in Follow(A)
                        (first_set.contains(this->sym_index(EPSILON)) &&
                         follow_set.contains(term_idx));
      if (cond) {
        this->parsing_table_(nterm_idx, idx) = prod_idx;
      }
    }
  }
}

void Parser::build_firsts()
{
  auto null_idx = this->sym_index(EPSILON);

  // Create a set for every terminal
  for (size_t idx = 0; idx < this->terms_; idx++) {
    auto term_idx = this->nterms_ + idx;
    this->first_[term_idx].insert(term_idx);
  }

  // Run till no sets change
  decltype(this->first_) prev;
  do {
    prev = this->first_;
    // Do for every production rule
    for (auto const& prod : this->grammar_) {
      auto left_idx = prod.lhs;
      // Rule 1: If the symbol is a terminal, insert it
      if (is_term(prod.rhs[0])) {
        this->first_[left_idx].insert(prod.rhs[0]);
        continue;
      }

      // Rule 2: Add the first set of the non-terminal
      auto& first_set = this->first_[prod.rhs[0]];
      this->first_[left_idx].insert(first_set.begin(), first_set.end());

      // Rule 3: If the first set contains null, add the first set of the
      // following terminal
      if (auto it = first_set.find(null_idx); it != first_set.end()) {
        // Replace null with the first set of the following symbol
        first_set.erase(it);
        auto next_set = this->first_[prod.rhs[1]];
        first_set.insert(next_set.begin(), next_set.end());
      }
    }
  } while (this->first_ != prev);
}

void Parser::build_follows()
{
  // Start symbol has EOL in it
  this->follow_[this->start_index_] = {this->sym_index(EOL)};
  auto null_idx = this->sym_index(EPSILON);

  decltype(this->follow_) prev;
  do {
    prev = this->follow_;

    // Repeat for every production rule
    for (auto const& prod : this->grammar_) {
      auto left_idx = prod.lhs;
      for (auto it = prod.rhs.begin(); it != prod.rhs.end(); it++) {
        auto cur_idx = *it;
        // Skip terminals
        if (is_term(cur_idx)) {
          continue;
        }

        // Rule 1: If the symbol is the last
        if (auto next = it + 1; next == prod.rhs.end()) {
          // Add the follow set of the left index
          auto follow_set = this->follow_[left_idx];
          this->follow_[cur_idx].insert(
            follow_set.begin(), follow_set.end()
          );
          continue;
        }

        auto next_idx = *(it + 1);
        // Rule 2: If the first set contains epsilon, add the follow set
        auto first_set = this->first_[next_idx];
        if (auto null_it = first_set.find(null_idx);
            null_it != first_set.end()) {
          // Add the follow set to it
          first_set.erase(null_it);
          auto follow_set = this->follow_[next_idx];
          this->follow_[cur_idx].insert(
            follow_set.begin(), follow_set.end()
          );
        }
        // Rule 3: Add the first set of the following symbol
        this->follow_[cur_idx].insert(first_set.begin(), first_set.end());
      }
    }

  } while (this->follow_ != prev);
}

void Parser::print_results()
{
  // // Print symbol counts
  // std::cout << "Terminals: " << this->terms_;
  // std::cout << "; Non-Terminals: " << this->nterms_ << '\n';

  // // Print Symbols
  // std::cout << "\nSymbols: \n";
  // for (auto const& [name, idx] : this->sym_table_) {
  //   std::cout << idx << ": " << name << "; ";
  //   std::cout << (this->symbols_[idx].is_term ? "Terminal"
  //                                             : "Non-Terminal")
  //             << '\n';
  // }

  std::cout << "\nGrammar Rules: \n";

  auto iter_view =
    this->grammar_ |
    std::views::transform([idx = 0](auto const& elem) mutable {
      return std::make_pair(idx++, elem);
    });

  // Print production rules
  for (auto const& [idx, prod] : iter_view) {
    std::cout << std::setw(2) << idx << ") " << this->sym_name(prod.lhs)
              << ": ";
    std::ranges::for_each(prod.rhs, [this](auto const& elem) {
      std::cout << this->sym_name(elem) << " ";
    });
    std::cout << '\n';
  }

  // // Print first sets
  // std::cout << "\nFirst Sets: \n";
  // for (auto const& [sym_idx, first_set] : this->first_) {
  //   // Skip printing terminals
  //   if (is_term(sym_idx)) {
  //     continue;
  //   }

  //   std::cout << this->sym_name(sym_idx) << ": ";
  //   std::ranges::for_each(first_set, [this](auto const& elem) {
  //     std::cout << this->sym_name(elem) << " ";
  //   });
  //   std::cout << '\n';
  // }

  // // Print follow sets
  // std::cout << "\nFollow Sets: \n";
  // for (auto const& [sym_idx, follow_set] : this->follow_) {
  //   std::cout << this->sym_name(sym_idx) << ": ";
  //   std::ranges::for_each(follow_set, [this](auto const& elem) {
  //     std::cout << this->sym_name(elem) << " ";
  //   });
  //   std::cout << '\n';
  // }

  std::cout << "\nParsing Table:\n";
  print_table();
}

void Parser::print_table()
{
  // width for IDs and values
  constexpr int IDW = 8;   // room for ≤6 chars + padding
  constexpr int VALW = 6;  // adjusts numeric alignment

  auto const& t = this->parsing_table_;

  // Column header
  std::cout << std::setw(IDW) << " ";
  for (size_t j = 0; j < t.m_cols; ++j) {
    std::cout << std::setw(VALW) << this->sym_name(j + this->nterms_);
  }
  std::cout << '\n';

  // Table body
  for (size_t i = 0; i < t.m_rows; ++i) {
    std::cout << std::setw(IDW) << this->sym_name(i);
    for (size_t j = 0; j < t.m_cols; ++j) {
      if (t(i, j) == this->nterms_ + this->terms_) {
        std::cout << std::setw(VALW) << '-';
      }
      else {
        std::cout << std::setw(VALW) << t(i, j);
      }
    }
    std::cout << '\n';
  }
}

// ======== Static Members ======== //
std::string Parser::trim(std::string const& s)
{
  auto not_space = [](char const ch) { return !std::isspace(ch); };
  auto begin = std::ranges::find_if(s, not_space);
  auto end =
    std::ranges::find_if(s | std::views::reverse, not_space).base();

  return {begin, end};
}
