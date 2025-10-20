#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include "bk_tree.hpp"

namespace
{

  /**
   * @brief Compute the Levenshtein distance between two string
   *
   * @return the distance
   */
  std::size_t levenshtein_dist(std::string_view a, std::string_view b)
  {
    const std::size_t m = a.length();
    const std::size_t n = b.length();

    std::vector<std::vector<std::size_t>> dists(n, std::vector<std::size_t>(m));

    // Fill the first row and column
    dists[0][0] = static_cast<std::size_t>(a[0] != b[0]);
    for (size_t i = 1; i < n; i++) {
      dists[i][0] = dists[i - 1][0] + 1;
    }

    for (size_t j = 1; j < m; j++) {
      dists[0][j] = dists[0][j - 1] + 1;
    }

    // Generate the remaining distances
    for (size_t i = 1; i < n; i++) {
      for (size_t j = 1; j < m; j++) {
        auto cur_cost = static_cast<std::size_t>(a[i] != b[j]);
        dists[i][j] = std::min(
          {dists[i - 1][j - 1] + cur_cost,
           dists[i - 1][j] + 1,
           dists[i][j - 1] + 1}
        );
      }
    }

    return dists[n - 1][m - 1];
  }

}  //anonymous namespace

// ====== BkNode Functions ====== //
void BkTree::BkNode::insert(const Word &word)
{
  // Compute the levenshtein distance
  auto dist = levenshtein_dist(this->word_.entry, word.entry);

  auto it = children_.find(dist);

  // Node not found
  if (it == children_.end()) {
    children_[dist] = std::make_unique<BkNode>(word);
  }
  // Node is found, do a recursive call
  else {
    children_[dist]->insert(word);
  }
}

void BkTree::BkNode::dfs()
{
  std::cout << this->word_.entry << '\n';
  for (auto &[dist, child] : this->children_) {
    child.get()->dfs();
  }
}

void BkTree::BkNode::search(
  const std::string &word, std::size_t max_distance, std::vector<Word> &words
)
{
  auto dist = levenshtein_dist(this->word_.entry, word);
  if (dist <= max_distance) {
    words.push_back(this->word_);
  }

  // Check children
  auto child_it = this->children_ |
                  std::views::filter([&max_distance, &dist](const auto &child) {
                    return (dist + child.first >= max_distance) ||
                           (max_distance + child.first >= dist);
                  });

  for (auto &[dist, child] : child_it) {
    child->search(word, max_distance, words);
  }
}

// ====== BkTree Functions ====== //
void BkTree::insert(const Word &word)
{
  // If root exists, add to it
  if (this->root_) {
    this->root_->insert(word);
  }
  // Create a root
  else {
    root_ = std::make_unique<BkNode>(word);
  }
}

void BkTree::dfs()
{
  this->root_->dfs();
}

std::vector<Word> BkTree::search(const std::string &word, std::size_t distance)
{
  std::vector<Word> words;
  this->root_->search(word, distance, words);

  return words;
}
