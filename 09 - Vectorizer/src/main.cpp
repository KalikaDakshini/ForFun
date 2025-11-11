#include "vectorizer.hpp"

int main()
{
  Vectorizer<TfIdf> vectorizer = TfIdf();
  vectorizer.fit("./input");
  vectorizer.words();
  vectorizer.table();
}
