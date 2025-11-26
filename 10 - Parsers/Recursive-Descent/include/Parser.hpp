#ifndef PARSER_H
#define PARSER_H

#include <cstddef>
#include <optional>
#include <vector>

#include "Lexer.hpp"

struct Parser {
  explicit Parser(Lexer& lexer) : m_lexer_(lexer) {}

  bool parse();

private:
  Lexer& m_lexer_;
  std::vector<Token> tok_list_;
  std::size_t cursor_;

  // Functions defining a right-recursive grammar
  bool expr();
  bool term();
  bool expr1();

  // Helper methods
  void build_tokens();

  bool is_lparen() { return is_paren(TokenKind::LPAREN); }

  bool is_rparen() { return is_paren(TokenKind::RPAREN); }

  bool is_paren(TokenKind paren)
  {
    return tok_list_[cursor_++].kind == paren;
  }

  bool is_op()
  {
    auto& tok_kind = tok_list_[cursor_++].kind;
    return (tok_kind == TokenKind::ADD) ||
           (tok_kind == TokenKind::MUL);
  }

  bool is_num()
  {
    auto& tok_kind = tok_list_[cursor_++].kind;
    return (tok_kind == TokenKind::INT) ||
           (tok_kind == TokenKind::FLOAT);
  }

  TokenKind lookahead() { return this->tok_list_[cursor_].kind; }
};

#endif
