#include "driver.hpp"

namespace Kalika
{
  Driver::Driver() : m_scanner(*this), m_parser(m_scanner, *this), m_location(0)
  {}

  int Driver::parse()
  {
    m_location = 0;
    return m_parser.parse();
  }

  void Driver::clear()
  {
    m_location = 0;
  }

  void Driver::increase_loc(unsigned int loc)
  {
    m_location += loc;
  }

  unsigned int Driver::location() const
  {
    return m_location;
  }

}  //namespace Kalika
