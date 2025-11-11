#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace fs = std::filesystem;
namespace view = std::views;

static const std::vector<std::string> STOP_WORDS{
  "a",          "about",  "above",   "after",   "again",  "against",
  "all",        "am",     "an",      "and",     "any",    "are",
  "as",         "at",     "be",      "because", "been",   "before",
  "being",      "below",  "between", "both",    "but",    "by",
  "can",        "did",    "do",      "does",    "doing",  "don",
  "down",       "during", "each",    "few",     "for",    "from",
  "further",    "had",    "has",     "have",    "having", "he",
  "her",        "here",   "hers",    "herself", "him",    "himself",
  "his",        "how",    "i",       "if",      "in",     "into",
  "is",         "it",     "its",     "itself",  "just",   "me",
  "more",       "most",   "my",      "myself",  "no",     "nor",
  "not",        "now",    "of",      "off",     "on",     "once",
  "only",       "or",     "other",   "our",     "ours",   "ourselves",
  "out",        "over",   "own",     "s",       "same",   "she",
  "should",     "so",     "some",    "such",    "t",      "than",
  "that",       "the",    "their",   "theirs",  "them",   "themselves",
  "then",       "there",  "these",   "they",    "this",   "those",
  "through",    "to",     "too",     "under",   "until",  "up",
  "very",       "was",    "we",      "were",    "what",   "when",
  "where",      "which",  "while",   "who",     "whom",   "why",
  "will",       "with",   "you",     "your",    "yours",  "yourself",
  "yourselves",
};

template<typename Derived> struct Vectorizer {
  void fit(std::string_view path)
  {
    auto &d = static_cast<Derived &>(*this);
    d.fit(fs::path{path});
  }

  void words() const
  {
    std::ranges::for_each(this->vocab_, [](const auto &word) {
      std::cout << word << ", ";
    });
    std::cout << '\n';
  }

  void table() const
  {
    for (const auto &hash_table : this->table_) {
      std::ranges::for_each(hash_table, [](const auto &pair) {
        std::cout << pair.first << ": " << pair.second << "; ";
      });
      std::cout << "\n\n";
    }
  }

private:
  std::set<std::string> vocab_;
  std::vector<std::unordered_map<std::string, double>> table_;

  friend Derived;
  Vectorizer() : vocab_(), table_() {};
};

struct Count : Vectorizer<Count> {
  Count() = default;

  void fit(const fs::path &corpus)
  {
    // Pre-process the word before adding
    auto pre_process = [](const std::string &word) {
      std::string stripped;
      // Remove punctuation
      std::ranges::remove_copy_if(
        word, std::back_inserter(stripped), [](const char ch) {
          return std::ispunct(ch);
        }
      );
      //convert to lower
      std::ranges::transform(stripped, stripped.begin(), [](const char ch) {
        return std::tolower(ch);
      });

      // Return the processed word
      return stripped;
    };

    // Accumulate words into the table
    auto accumulate = [this,
                       &pre_process](const fs::path &file, std::size_t idx) {
      std::ifstream in_file{file};

      const std::istream_iterator<std::string> begin(in_file);
      const std::istream_iterator<std::string> end;

      std::for_each(
        begin, end, [this, &idx, &pre_process](const std::string &word) {
          // Strip the word
          const std::string stripped = pre_process(word);

          // Add the word to vocabulary set and update its frequency
          this->vocab_.insert(stripped);
          this->table_[idx][stripped] += 1;
        }
      );
    };

    // Repeat for every document in the directory
    std::for_each(
      fs::directory_iterator(corpus),
      fs::directory_iterator(),
      [&accumulate, idx = 0](const auto &file) mutable {
        accumulate(file.path(), idx);
        idx++;
      }
    );
  }

private:
};

struct TfIdf : Vectorizer<TfIdf> {
  void fit(const fs::path &path)
  {
    // Start of directory contents
    auto begin = fs::directory_iterator(path);
    auto end = fs::directory_iterator{};

    // Allocate space for table
    auto num_files = std::ranges::distance(begin, end);
    this->table_.resize(num_files);

    // Enumerate files and iterate over them
    size_t idx = 0;
    for (const auto &file_path : fs::directory_iterator(path)) {
      build_frequency(file_path.path(), idx);
      idx++;
    }

    // Compute the number of document occurences of a word
    auto inv_doc_freqs = this->build_doc_counts();

    // Compute IDF from doc_count
    auto IDF = [](double freq, double N) { return log(N / (freq + 1)) + 1; };
    std::ranges::for_each(inv_doc_freqs, [&num_files, &IDF](auto &pair) {
      pair.second = IDF(pair.second, num_files);
    });

    // Update table with TF-IDF
    for (auto &hash_table : this->table_) {
      std::ranges::for_each(hash_table, [&inv_doc_freqs](auto &pair) {
        pair.second *= inv_doc_freqs[pair.first];
      });
    }
  }

private:
  /**
   * @brief Calculate the number of times a word appears in a document
   *
   * @param path Path to file
   * @param idx id of the document
   */
  void build_frequency(const std::string &path, size_t idx)
  {
    std::ifstream in_file{path};

    if (!in_file) {
      std::cerr << "File " << path << " not found\n";
      return;
    }

    const std::istream_iterator<std::string> begin(in_file);
    const std::istream_iterator<std::string> end;

    // Add words to the table
    size_t n_total = 0;
    std::for_each(begin, end, [this, &idx, &n_total](const std::string &word) {
      const std::string stripped = strip(word);

      // Ignore stop words
      if (std::ranges::binary_search(STOP_WORDS, stripped)) {
        return;
      }
      // Update the total number of words
      n_total++;
      this->vocab_.insert(stripped);

      // Strip the words of non-essential characters
      this->table_[idx][stripped] += 1;
    });

    auto TF = [](double freq, double N) { return freq / N; };

    // Divide by n-total converting count to frequency
    std::ranges::for_each(this->table_[idx], [&n_total, &TF](auto &pair) {
      pair.second = TF(pair.second, n_total);
    });
  }

  /**
   * @brief Calculate how many documents a word appears in
   */
  std::unordered_map<std::string, double> build_doc_counts()
  {
    std::unordered_map<std::string, double> doc_counts;
    for (const auto &word : this->vocab_) {
      doc_counts[word] =
        std::ranges::count_if(this->table_, [&word](const auto &hash_table) {
          return hash_table.contains(word);
        });
    }

    return doc_counts;
  }

  /**
   * @brief Strip words to their essential characters
   */
  static std::string strip(std::string_view word)
  {
    // Remove punctuation and convert to lower case
    std::string stripped;
    std::ranges::copy(
      word | view::filter([](const char ch) { return !std::ispunct(ch); }) |
        view::transform([](const char ch) { return std::tolower(ch); }),
      std::back_inserter(stripped)
    );
    return stripped;
  }
};
