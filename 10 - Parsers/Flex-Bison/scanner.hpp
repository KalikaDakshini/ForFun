#ifndef SCANNER_H
#define SCANNER_H

#ifndef yyFlexLexerOnce
#  undef yyFlexLexer
#  define yyFlexLexer KalikaFlexLexer
#  include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL Kalika::Parser::symbol_type Kalika::Scanner::get_next_token()

#include "parser.hh"

namespace Kalika
{

  // Forward declaration of Driver
  struct Driver;

  struct Scanner : public yyFlexLexer {
    Scanner(Driver& drv) : m_driver(drv) {}

    virtual ~Scanner() {}

    virtual Kalika::Parser::symbol_type get_next_token();

  private:
    Driver& m_driver;
  };

}  //namespace Kalika

#endif
