"""Huffman Coding Algorithm"""

import sys
import heapq
from collections import Counter
from typing import Optional
from binarytree import Node


class HuffmanNode(Node):
    """Tree node for huffman encoding"""

    def __init__(
        self,
        symbol: str,
        freq: int,
        left: Optional["HuffmanNode"],
        right: Optional["HuffmanNode"],
    ):
        super().__init__(f"{symbol}:{freq}")
        self.symbol = symbol
        self.freq = freq
        self.left = left
        self.right = right
        self._height = 0
        self.code = ""

    @staticmethod
    def longest_path(tree: Optional["HuffmanNode"]) -> int:
        """Return the longest path in the graph"""
        if not tree:
            return 0
        return (
            max(
                HuffmanNode.longest_path(tree.left),
                HuffmanNode.longest_path(tree.right),
            )
            + 1
        )

    @staticmethod
    def tree_size(tree: Optional["HuffmanNode"]) -> int:
        """Return the longest path in the graph"""
        if not tree:
            return 0
        return (
            HuffmanNode.tree_size(tree.left)
            + HuffmanNode.tree_size(tree.right)
            + sys.getsizeof(tree)
        )

    def __lt__(self, other):
        return self.freq < other.freq


def build_codes(
    tree: Optional[HuffmanNode], code_dict: dict[str, str], code: str = ""
):
    """Build lengths of codewords"""
    if not tree:
        return
    tree.code = code
    tree.value += f":{tree.code}"
    # Update dict
    if not tree.left and not tree.right:
        code_dict[tree.symbol] = tree.code
    # Recursively build lengths
    build_codes(tree.left, code_dict, f"{tree.code}0")
    build_codes(tree.right, code_dict, f"{tree.code}1")


def translate(data: Counter) -> list[HuffmanNode]:
    """Translate data into a manageable format"""
    node_list = []
    for key, freq in data.items():
        node_list.append(HuffmanNode(key, freq, None, None))

    return node_list


def huffman_coding(data: Counter, code_dict: dict[str, str]) -> HuffmanNode:
    """Perform Huffman Coding"""
    heap = translate(data)
    heapq.heapify(heap)
    # Repeat till heap is empty
    while len(heap) > 1:
        n1 = heapq.heappop(heap)
        n2 = heapq.heappop(heap)
        new_node = HuffmanNode("O", n1.freq + n2.freq, n1, n2)
        heapq.heappush(heap, new_node)

    # Return root node
    root = heap[0]
    build_codes(root, code_dict)
    return root


def huffman_encode(data: Counter, code_dict: dict[str, str]) -> int:
    """Encode the data using Huffman encoding"""
    total_length = sys.getsizeof(code_dict)
    for key, freq in data.items():
        total_length += freq * len(code_dict[key])

    return total_length


if __name__ == "__main__":
    pass
