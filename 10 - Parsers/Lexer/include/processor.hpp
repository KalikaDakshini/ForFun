#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <array>
#include <cstdint>
#include <string_view>
#include <vector>

namespace Kalika
{

  enum class TokenKind : std::uint8_t {
    // Ordered in order of precedence
    CHAR,    // a
    ALTER,   // a|b
    CONCAT,  // a.b
    RANGE,   // [a-z]
    IF,      // a?
    PLUS,    // a+
    KLEENE,  // a*
    LPAREN,  // (
    RPAREN,  // )
    LBRAC,   // [
    RBRAC,   // ]
    COUNT    // Token Count
  };

  constexpr auto NUM_TOKENS = static_cast<size_t>(TokenKind::COUNT);

  struct CharRange {
    char start;
    char end;
  };

  //TODO(kalika): Implement ranges(a-z) for Character classes [a-zA-Z]
  //TODO(kalika): Implement aliases \s \w etc..
  struct Token {
    explicit Token(char ch);
    explicit Token(std::vector<CharRange> rs, bool neg = false);

    char val;
    std::vector<CharRange> ranges;
    bool negated;
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
  // Create a token with character class
  Token make_char_class(std::string_view input, size_t& i);

  // Check if character is not an operator
  bool is_char(char ch);

};  //namespace Kalika

#endif
