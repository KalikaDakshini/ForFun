"""Huffman Encoding for bitstreams"""

from collections import Counter
import numpy as np
import matplotlib.pyplot as plt
from huffman import huffman_coding, huffman_encode, HuffmanNode


def generate_data(p: float, n: int) -> np.ndarray:
    """Generate random data"""
    bits = np.random.binomial(1, p, n)
    return bits


def fetch_data(path: str) -> np.ndarray:
    """Read a file into bits"""
    with open(path, "rb") as df:
        data_str = "".join(map(lambda c: f"{c:08b}", df.read()))
        return np.fromiter(data_str, dtype=np.int64)


def reshape_data(bits: np.ndarray, b: int) -> Counter:
    """Reshape data to divide into chunks of length b"""
    p = -len(bits) % b
    if p != 0:
        bits = np.concatenate((bits, np.zeros(p)))
    # Pack data into chunks
    chunks = bits.reshape((-1, b))
    # Treat chunks as numbers for easy encoding
    chunks = (chunks @ np.flip(1 << np.arange(b))).astype(np.int64)
    return Counter(chunks)


def get_longest_code(
    bits: np.ndarray, b: int, code_dict: dict[str, str]
) -> int:
    """Get longest Huffman code for a given probability distribution and bit length"""
    data = reshape_data(bits, b)
    huffman_tree = huffman_coding(data, code_dict)
    return HuffmanNode.longest_path(huffman_tree)


def get_encoded_size(
    bits: np.ndarray, b: int, code_dict: dict[str, str]
) -> int:
    """Return the size of the encoded data"""
    data = reshape_data(bits, b)
    huffman_coding(data, code_dict)
    encoded_length = huffman_encode(data, code_dict)
    return encoded_length


def main():
    """Main function"""
    # Load data
    bits = fetch_data("./input.txt")

    # Run experiments
    bit_min, bit_max = 2, 16
    bit_lengths = range(bit_min, bit_max)
    code_lengths = []
    for bit_length in bit_lengths:
        code_dict: dict[str, str] = {}
        code_lengths.append(get_encoded_size(bits, bit_length, code_dict))

    # Plot results
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))
    fig.suptitle("Huffman Analysis")

    # Plot Size vs Bit length
    ax1.plot(
        bit_lengths,
        code_lengths,
        marker="o",
        linestyle="-",
    )
    ax1.axhline(y=bits.size, linestyle="--")
    ax1.set_xlabel("Bit Length")
    ax1.set_ylabel("Compressed Size")
    ax1.grid(True)

    # Plot Compression rate vs Bit length
    ax2.plot(
        bit_lengths,
        list(map(lambda cl: (bits.size - cl) / bits.size * 100, code_lengths)),
        marker="o",
        linestyle="-",
    )
    ax2.set_xlabel("Bit Length")
    ax2.set_ylabel("Compression %")
    ax2.grid(True)
    plt.savefig("plot.png")


if __name__ == "__main__":
    main()
