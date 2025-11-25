#ifndef DRIVER_H
#define DRIVER_H

#include <vector>

#include "parser.hh"
#include "scanner.hpp"

namespace Kalika
{
  struct Driver {
    Driver();

    /**
     * Run parser. Results are stored inside.
     * \returns 0 on success, 1 on failure
     */
    int parse();

    /**
     * @brief Clear the buffer and resume scanning
     */
    void clear();

    /**
     * This is needed so that Scanner and Parser can call some
     * methods that we want to keep hidden from the end user.
     */
    friend class Parser;
    friend class Scanner;

  private:
    // Used internally by Scanner YY_USER_ACTION to update location indicator
    void increase_loc(unsigned int loc);

    // Used to get last Scanner location. Used in error messages.
    unsigned int location() const;

    Scanner m_scanner;
    Parser m_parser;
    unsigned int m_location;
  };

}  //namespace Kalika

#endif  // DRIVER_H
