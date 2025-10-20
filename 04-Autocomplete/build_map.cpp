#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace
{
  /**
   * @brief Read the file and update word frequencies
   *
   *
   * @param path Path to input file
   * @param freqs Map holding frequency data
   */

  // Strip punctuation from the word and add it to the map
  void strip_and_add(
    std::unordered_map<std::string, size_t> &word_map, std::string &word
  )
  {
    // Replace hyphens with spaces
    std::ranges::replace_if(word, [](char c) { return c == '-'; }, ' ');
    std::stringstream iss{word};

    // Process word and add it to map
    std::string token;
    while (iss >> token) {
      // Strip punctuation
      std::erase_if(token, [](char c) { return !std::isalpha(c); });
      // Convert to lower case
      std::ranges::transform(token, token.begin(), [](unsigned char c) {
        return std::tolower(c);
      });
      // Add if not empty
      if (!token.empty()) {
        ++word_map[token];
      }
    }
  }

  void process_file(
    const std::string &path, std::unordered_map<std::string, size_t> &word_map
  )
  {
    std::ifstream in_file{path};
    std::string word;

    // Check if file exists
    if (!in_file) {
      std::cerr << "File not found: " << path << '\n';
      return;
    }

    // Add words to frequency map
    while (in_file >> word) {
      strip_and_add(word_map, word);
    }
  }

}  //anonymous namespace

int main(int argc, const char *argv[])
{
  // Get corpus directory from command line
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <corpus_dir>\n";
    return EXIT_FAILURE;
  }
  // Frequency map
  std::unordered_map<std::string, std::size_t> word_map;

  // Process all the files in corpus
  const std::filesystem::path corpus_dir{argv[1]};
  for (const auto &file : std::filesystem::directory_iterator(corpus_dir)) {
    process_file(file.path().string(), word_map);
  }

  // Print word map
  std::ofstream out_file("./word_map.csv");
  for (const auto &[word, freq] : word_map) {
    out_file << word << ";" << freq << '\n';
  }

  return EXIT_SUCCESS;
}
