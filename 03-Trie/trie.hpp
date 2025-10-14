#ifndef TRIE_HPP
#define TRIE_HPP

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>

class Trie
{
	private:
	class Node
	{
private:
		char ch_;
		bool terminal_;
		std::unordered_map<char, std::unique_ptr<Node>> children_;

public:
		Node() = default;
		explicit Node(char ch) : ch_(ch), terminal_(true) {}

		// public methods
		/**
		 * @brief Get the child object corresponding to the character
		 *
		 * @param c character to insert
		 * @return pointer to Node
		 */
		Node* get_child(char c) const
		{
			auto it = children_.find(c);
			return (it != children_.end() ? it->second.get() : nullptr);
		}

		Node* add_child(char c)
		{
			// Change terminal status to false
			this->set_terminal(false);

			// Return existing child
			Node* child = this->get_child(c);
			if (child != nullptr) {
				return child;
			}

			// Create a new node
			auto node = std::make_unique<Node>(c);
			child = node.get();
			this->children_.emplace(c, std::move(node));

			return child;
		}

		/**
		 * @brief Check if the node is terminal or not
		 */
		bool terminal() const noexcept { return terminal_; }
		void set_terminal(bool status = true) noexcept
		{
			this->terminal_ = status;
		}
	};

	Node root_;

	// private methods
	const Node* get_prefix(const std::string& word) const
	{
		const Node* node = &(this->root_);

		// Convert string to lower case
		std::string new_word{word};
		std::transform(new_word.begin(), new_word.end(), new_word.begin(),
									 [](char ch) { return std::tolower(ch); });

		// Add word to prefix tree
		for (char ch : word) {
			// If char doesn't exist in the path, return false
			if ((node = node->get_child(ch)) == nullptr) {
				return nullptr;
			}
		}

		return node;
	}

	public:
	Trie();

	// public methods
	void insert(const std::string& word);
	bool search(const std::string& word);
	bool prefix(const std::string& word);
};

#endif
