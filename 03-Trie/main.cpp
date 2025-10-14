#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "trie.hpp"

void normalize_word(std::string& word);

int main(int argc, const char* argv[])
{
	// Open input file for reading
	std::string input_file("input.txt");
	if (argc > 1) {
		input_file = argv[1];
	}
	std::ifstream file_stream = std::ifstream(input_file);

	// Initialize a trie
	Trie prefix_tree;

	// Read the contents of the file
	std::string line, word;
	while (std::getline(file_stream, line)) {
		std::istringstream iss(line);
		while (iss >> word) {
			normalize_word(word);
			if (!word.empty()) {
				prefix_tree.insert(word);
			}
		}
	}

	// Print results
	std::cout << "Under: "
						<< (prefix_tree.search("under") ? "found" : "not found")
						<< std::endl;
	std::cout << "new: " << (prefix_tree.search("new") ? "found" : "not found")
						<< std::endl;

	return 0;
}

void normalize_word(std::string& word)
{
	std::transform(word.begin(), word.end(), word.begin(), [](char ch) {
		return std::isalpha(ch) ? std::tolower(ch) : ' ';
	});
	word.erase(std::remove(word.begin(), word.end(), ' '), word.end());
}
