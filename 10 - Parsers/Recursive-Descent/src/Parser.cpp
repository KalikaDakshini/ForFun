#include "Parser.hpp"
#include "Lexer.hpp"

void Parser::build_tokens()
{
  // Reset token list on every parse
  this->tok_list_.clear();
  this->cursor_ = 0;

  // Gather tokens up to EOL
  while (true) {
    auto tok = this->m_lexer_.next();
    this->tok_list_.push_back(tok);

    if (!tok) {
      break;
    }
  }
}

bool Parser::parse()
{
  // Get all tokens in the stream before hand
  this->build_tokens();
  return this->expr() &&
         (this->cursor_ == this->tok_list_.size() - 1);
}

bool Parser::expr()
{
  return this->term() && this->expr1();
}

bool Parser::expr1()
{
  auto tok = lookahead();
  if (tok == TokenKind::ADD || tok == TokenKind::MUL) {
    return this->is_op() && this->expr();
  }

  // null string case
  return true;
}

bool Parser::term()
{
  if (lookahead() == TokenKind::LPAREN) {
    return (is_lparen() && expr() && is_rparen());
  }
  return this->is_num();
}
