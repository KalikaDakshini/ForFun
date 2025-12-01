#include "processor.hpp"

#include <cctype>
#include <string>
#include <string_view>
#include <vector>

namespace Kalika
{
  std::vector<Token> process(std::string_view input)
  {
    std::vector<Token> token_stack;
    std::vector<Token> op_stack;

    for (auto ch : pre_process(input)) {
      // Add chars to token stack
      if (is_char(ch)) {
        token_stack.emplace_back(ch, TokenKind::CHAR);
        continue;
      }
      // Add operators to operator stack following precedence
      auto op_token = get_token(ch);

      // Handle paranthesis
      if (op_token.is_rparen()) {
        auto top_token = op_stack.back();
        while (!top_token.is_lparen()) {
          token_stack.push_back(top_token);
          op_stack.pop_back();
          top_token = op_stack.back();
        }
        // Remove the left paren
        op_stack.pop_back();
        continue;
      }

      // Examine the top of the stack and check for precedence
      while (!op_stack.empty()) {
        auto top_token = op_stack.back();
        // Move all higher precedende operators to token_stack
        if (op_token.prec() <= top_token.prec()) {
          op_stack.pop_back();
          token_stack.push_back(top_token);
        }
        else {
          break;
        }
      }
      // Add op_token to stack
      op_stack.push_back(op_token);
    }

    // Move all remaining operators to token_stack
    while (!op_stack.empty()) {
      auto token = op_stack.back();
      op_stack.pop_back();
      token_stack.push_back(token);
    }

    return token_stack;
  }

  std::string pre_process(std::string_view input)
  {
    std::string out_str;

    auto can_end = [](char ch) {
      return is_char(ch) || ch == ')' || ch == '*' || ch == '+' ||
             ch == '?';
    };
    auto can_begin = [](char ch) { return is_char(ch) || ch == '('; };

    for (auto i = 0UL; i + 1 < input.length(); ++i) {
      char const a = input[i];
      char const b = input[i + 1];

      out_str.push_back(a);
      if (can_end(a) && can_begin(b)) {
        out_str.push_back('.');
      }
    }

    if (!input.empty()) {
      out_str.push_back(input.back());
    }

    return out_str;
  }

  Token get_token(char ch)
  {
    switch (ch) {
    case '|':
      return Token{.val = ch, .kind = TokenKind::ALTER};
    case concat:
      return Token{.val = ch, .kind = TokenKind::CONCAT};
    case '?':
      return Token{.val = ch, .kind = TokenKind::IF};
    case '+':
      return Token{.val = ch, .kind = TokenKind::PLUS};
    case '*':
      return Token{.val = ch, .kind = TokenKind::KLEENE};
    case '(':
      return Token{.val = ch, .kind = TokenKind::LPAREN};
    case ')':
      return Token{.val = ch, .kind = TokenKind::RPAREN};
    default:
      return Token{.val = ch, .kind = TokenKind::CHAR};
    }
  }

  bool is_char(char ch)
  {
    return static_cast<bool>(std::isalnum(ch));
  }
}  //namespace Kalika
