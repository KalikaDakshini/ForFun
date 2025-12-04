#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace Kalika
{

  enum class TokenKind : std::int8_t {
    // Ordered in order of precedence
    CHAR,    // a
    ALTER,   // a|b
    CONCAT,  // a.b
    CLASS,   //a-z
    IF,      // a?
    PLUS,    // a+
    KLEENE,  // a*
    LPAREN,  // (
    RPAREN,  // )
    LBRAC,   // [
    RBRAC,   // ]
    COUNT    // Token Count
  };

  constexpr auto NUM_TOKENS = static_cast<std::size_t>(TokenKind::COUNT);

  //TODO(kalika): Implement Character classes [a-zA-Z]
  //TOOD(kalika): Implement aliases \s \w etc..
  struct Token {
    Token(char ch);

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
      return this->kind == TokenKind::LPAREN ||
             this->kind == TokenKind::LBRAC;
    }

    [[nodiscard]] bool is_rparen() const
    {
      return this->kind == TokenKind::RPAREN ||
             this->kind == TokenKind::RBRAC;
    }

  private:
    static constexpr std::array<std::uint8_t, NUM_TOKENS> prec_id = {
      0, 1, 2, 3, 3, 3, 3, 0, 0, 0, 0
    };
  };

  /**
   * @brief Use shunting-yard algorithm to parse the string
   */
  std::vector<Token> process(std::string_view input);
  // Add concatenation operators to the string
  std::vector<Token> tokenize(std::string_view input);

  // Check if character is not an operator
  bool is_char(char ch);

};  //namespace Kalika

#endif
