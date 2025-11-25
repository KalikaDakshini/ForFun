#include "Lexer.hpp"

#include <cctype>
#include <string>

Token Lexer::next()
{
  int const val = this->stream_.peek();
  char const ch = static_cast<char>(val);

  // Endline or end of file
  if (val == '\n' || val == std::istream::traits_type::eof()) {
    this->stream_.get();
    return Token{TokenKind::EOL};
  }

  // Whitespace
  if (std::isspace(val) != 0) {
    skip_space();
    return next();
  }
  // Operators
  if (std::string("*+").find(ch) != std::string::npos) {
    return get_op();
  }

  // Numbers
  if (std::isdigit(val) != 0 || ch == '.') {
    return get_number();
  }

  // Discard any unprocessed character
  this->stream_.get();

  // If no criterion matches, continue with the scanning
  return next();
}

void Lexer::skip_space()
{
  while (std::isspace(this->stream_.peek()) != 0) {
    this->stream_.get();
  }
}

Token Lexer::get_op()
{
  int const val = this->stream_.get();
  char const ch = static_cast<char>(val);

  if (ch == '+') {
    return Token{TokenKind::ADD};
  }
  return Token{TokenKind::MUL};
}

Token Lexer::get_number()
{
  bool is_float = false;
  std::string num_str;

  while (true) {
    int const val = this->stream_.peek();
    char const ch = static_cast<char>(val);
    // Break on non-numeric character
    if (std::isdigit(val) == 0 && ch != '.') {
      break;
    }
    // Break if a second '.' is encountered signifiying the end of a
    // number
    if (is_float && ch == '.') {
      break;
    }
    // Set number to floating point
    if (ch == '.') {
      is_float = true;
    }

    // Add the character and advance the scanner
    num_str.push_back(ch);
    this->stream_.get();
  }

  // Return appropriate token
  if (is_float) {
    return Token(std::stof(num_str));
  }
  return Token(std::stoi(num_str));
}
