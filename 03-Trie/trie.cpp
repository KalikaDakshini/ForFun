#include "trie.hpp"

// Initialize root with sentinel node
Trie::Trie() : root_(Node('\0')) {}

void Trie::insert(const std::string& word)
{
	Node* node = &(this->root_);

	// Push characters into tree
	for (char ch : word) {
		node = node->add_child(ch);
	}

	// Set terminal
	node->set_terminal();
}

bool Trie::search(const std::string& word)
{
	const Node* node = get_prefix(word);
	if (node == nullptr) {
		return false;
	}
	return node->terminal();
}

bool Trie::prefix(const std::string& word)
{
	const Node* node = get_prefix(word);
	return (node != nullptr);
}
