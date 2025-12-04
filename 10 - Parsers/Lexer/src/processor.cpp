#include "processor.hpp"

#include <string>
#include <string_view>
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

    auto i = 0UL;
    while (i++ < input.length() - 1) {
      char const a = input[i - 1];
      char const b = input[i];

      tokens.emplace_back(a);
      // Make the implicit concatenation operator, explicit
      if (can_end(a) && can_begin(b)) {
        tokens.emplace_back('.');
      }
    }

    // Push the last remaining token
    if (!input.empty()) {
      tokens.emplace_back(input.back());
    }

    return tokens;
  }

  // Token constructor
  Token::Token(char ch) : val(ch)
  {
    switch (ch) {
    case '|':
      kind = TokenKind::ALTER;
      break;
    case '.':
      kind = TokenKind::CONCAT;
      break;
    case '-':
      kind = TokenKind::CLASS;
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

  bool is_char(char ch)
  {
    return std::string("[]()+-*|.").find(ch) == std::string::npos;
  }
}  //namespace Kalika
