#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <variant>

enum class TokenKind {
  ADD,
  MUL,
  INT,
  FLOAT,
  LPAREN,
  RPAREN,
  EOL,
};

struct Token {
  TokenKind kind;
  using TokenType = std::variant<int, float, char>;

  TokenType val;

  explicit Token(int i) : kind(TokenKind::INT), val(i) {}

  explicit Token(float f) : kind(TokenKind::FLOAT), val(f) {}

  explicit Token(TokenKind tok_kind) : kind(tok_kind) {}

  // Accessor Functions
  bool is_end() const { return this->kind != TokenKind::EOL; }

  int as_int() const { return std::get<int>(val); }

  float as_float() const { return std::get<float>(val); }

  // Bool conversion
  explicit operator bool() const noexcept { return this->is_end(); }

  // Outstream overload
  friend std::ostream& operator<<(std::ostream& out, Token const& t)
  {
    if (t.kind == TokenKind::INT) {
      out << "INT(" << t.as_int() << ")";
    }

    else if (t.kind == TokenKind::FLOAT) {
      out << "FLOAT(" << t.as_float() << ")";
    }

    else if (t.kind == TokenKind::ADD) {
      out << "OP(+)";
    }

    else if (t.kind == TokenKind::MUL) {
      out << "OP(*)";
    }

    else if (t.kind == TokenKind::LPAREN) {
      out << "LPAREN";
    }

    else if (t.kind == TokenKind::RPAREN) {
      out << "RPAREN";
    }

    else if (t.kind == TokenKind::EOL) {
      out << "EOL";
    }

    return out;
  }
};

struct Lexer {
  explicit Lexer(std::istream& in) : stream_(in) {}

  Token next();

private:
  std::istream& stream_;

  void skip_space();
  Token get_number();
  Token get_op();
  Token get_paren();
};

#endif
