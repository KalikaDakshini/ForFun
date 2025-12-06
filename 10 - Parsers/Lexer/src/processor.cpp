#include "processor.hpp"

#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace Kalika
{
  std::vector<Token> process(std::string_view input)
  {
    std::vector<Token> out;
    std::vector<Token> ops;

    for (auto const& tok : tokenize(input)) {
      // Add chars to token stack
      if (tok.kind == TokenKind::CHAR) {
        out.push_back(tok);
        continue;
      }
      // Just push LPAREN on top of the stack
      if (tok.is_lparen()) {
        ops.push_back(tok);
        continue;
      }
      // Handle paranthesis
      if (tok.is_rparen()) {
        while (!ops.back().is_lparen()) {
          out.push_back(ops.back());
          ops.pop_back();
        }
        // Remove the left paren
        ops.pop_back();
        continue;
      }
      // Examine the top of the stack and check for precedence
      while (!ops.empty() && (tok.prec() <= ops.back().prec())) {
        // Move all higher precedende operators to out
        out.push_back(ops.back());
        ops.pop_back();
      }
      // Add op_token to stack
      ops.push_back(tok);
    }

    // Move all remaining operators to out
    while (!ops.empty()) {
      out.push_back(ops.back());
      ops.pop_back();
    }

    return out;
  }

  // Break input string into a set of tokens
  std::vector<Token> tokenize(std::string_view input)
  {
    std::vector<Token> tokens;

    // Recognise end and start characters to demarcate concat
    auto can_end = [](char ch) {
      return is_char(ch) || ch == ')' || ch == ']' || ch == '*' ||
             ch == '+' || ch == '?';
    };
    auto can_begin = [](char ch) {
      return is_char(ch) || ch == '(' || ch == '[';
    };

    for (auto i = 0UL; i < input.length(); i++) {
      char const a = input[i];

      // Make the implicit binary operator explicit
      if (!tokens.empty() &&
          (can_end(tokens.back().val) && can_begin(a))) {
        tokens.emplace_back('.');
      }

      // Handle character classes
      if (a == '[') {
        tokens.emplace_back(make_char_class(input, i));
        continue;
      }

      tokens.emplace_back(a);
    }

    return tokens;
  }

  // Token constructor
  Token::Token(char ch) : val(ch)
  {
    switch (val) {
    case '|':
      kind = TokenKind::ALTER;
      break;
    case '.':
      kind = TokenKind::CONCAT;
      break;
    case '?':
      kind = TokenKind::IF;
      break;
    case '+':
      kind = TokenKind::PLUS;
      break;
    case '*':
      kind = TokenKind::KLEENE;
      break;
    case '(':
      kind = TokenKind::LPAREN;
      break;
    case ')':
      kind = TokenKind::RPAREN;
      break;
    case '[':
      kind = TokenKind::LBRAC;
      break;
    case ']':
      kind = TokenKind::RBRAC;
      break;
    default:
      kind = TokenKind::CHAR;
      break;
    }
  }

  Token::Token(std::vector<CharRange> rs, bool neg) :
    val(']'), ranges(std::move(rs)), negated(neg), kind(TokenKind::RANGE)
  {}

  Token make_char_class(std::string_view input, size_t& i)
  {
    // Precondition: s[i] == '['
    ++i;  //Consume [

    bool neg = false;
    std::vector<CharRange> ranges;

    // Optional negation: [^...]
    if (i < input.length() && input[i] == '^') {
      neg = true;
      ++i;
    }

    char prev = 0;
    bool have_prev = false;

    while (i < input.size()) {
      char const c = input[i++];

      // End of class.
      if (c == ']') {
        --i;
        break;
      }

      // Construct range
      if (c == '-' && have_prev && i < input.size() && input[i] != ']') {
        // Range: prev - nextchar
        char const hi = input[i++];
        ranges.emplace_back(CharRange{.start = prev, .end = hi});
        have_prev = false;
        continue;
      }

      // Literal character
      if (have_prev) {
        // Push previous as single-char range
        ranges.emplace_back(CharRange{.start = prev, .end = prev});
      }
      prev = c;
      have_prev = true;
    }

    if (have_prev) {
      ranges.emplace_back(CharRange{.start = prev, .end = prev});
    }

    return Token(ranges, neg);
  }

  bool is_char(char ch)
  {
    return std::string("[]()+-*|.").find(ch) == std::string::npos;
  }
}  //namespace Kalika
