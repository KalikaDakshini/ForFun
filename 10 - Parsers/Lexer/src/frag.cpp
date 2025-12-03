#include "frag.hpp"

#include <set>

namespace Kalika::internal
{
  // ======== Frag methods ======== //
  void Frag::add_alphabet(Frag const& frag)
  {
    this->alphabet.insert(frag.alphabet.begin(), frag.alphabet.end());
  }

  void Frag::add_alphabet(char ch)
  {
    if (ch != '\0') {
      this->alphabet.insert(ch);
    }
  }
}  //namespace Kalika::internal
