#include <iostream>
#include <string>
#include <map>
#include <algorithm>

int main(int argc, char const *argv[])
{
	std::string word;
	std::map<std::string, size_t> freqs;
	size_t word_count = 0;

	// Read the line and note frequencies
	while(std::cin >> word) {
		// Transform the word to lower
		std::transform(
			word.begin(), word.end(), word.begin(), [](char c) {return tolower(c);}
		);
		freqs[word]++;
		word_count += 1;
	}

	// Print the total number of words
	std::cout << "Word Count: " << word_count << std::endl;

	// Print the word with highest frequency
	auto max_word = std::max_element(
		freqs.begin(), freqs.end(),
		[](const auto& a, const auto& b) {return a.second < b.second;}
		);
	std::cout << "Max Word: " << max_word->first << std::endl;

	// Plot the histogram
	size_t max_length = 24;

	for (auto freq: freqs) {
		size_t length{(freq.second * max_length) / max_word->second};
		std::cout << freq.first << "|" << std::string(length, '=') << '>' << std::endl;
	}

	return 0;
}
