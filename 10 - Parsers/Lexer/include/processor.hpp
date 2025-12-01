#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace Kalika
{
  char const concat = '.';

  enum class TokenKind : std::int8_t {
    // Ordered in order of precedence
    CHAR,    // a
    ALTER,   // a|b
    CONCAT,  // a-b
    IF,      // a?
    PLUS,    // a+
    KLEENE,  // a*
    LPAREN,  // (
    RPAREN,  // )
    COUNT    // Token Count
  };

  constexpr auto NUM_TOKENS = static_cast<std::size_t>(TokenKind::COUNT);

  //TODO(kalika): Implement Character classes
  struct Token {
    char val;
    TokenKind kind;

    [[nodiscard]] std::uint8_t prec() const
    {
      return prec_id[static_cast<std::uint8_t>(kind)];
    }

    [[nodiscard]] bool is_paren() const
    {
      return is_lparen() || is_rparen();
    }

    [[nodiscard]] bool is_lparen() const
    {
      return this->kind == TokenKind::LPAREN;
    }

    [[nodiscard]] bool is_rparen() const
    {
      return this->kind == TokenKind::RPAREN;
    }

  private:
    static constexpr std::array<std::uint8_t, NUM_TOKENS> prec_id = {
      0, 1, 2, 3, 3, 3, 0, 0
    };
  };

  /**
   * @brief Use shunting-yard algorithm to parse the string
   */
  std::vector<Token> process(std::string_view input);
  // Add concatenation operators to the string
  std::string pre_process(std::string_view input);
  // Return an operator token
  Token get_token(char ch);

  // Check if character is not an operator
  bool is_char(char ch);

};  //namespace Kalika

#endif
