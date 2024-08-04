# Huffman Encoding and Decoding
This project provides an implementation of Huffman encoding and decoding algorithms. Huffman coding is a popular method of lossless data compression that assigns variable-length codes to input characters based on their frequencies. The most frequent characters get the shortest codes, making the overall encoded data smaller.

## Features
- Build a Huffman Tree based on character frequencies.
- Generate Huffman Codes from the Huffman Tree.
- Encode a given input string using the generated Huffman Codes.
- Decode a given Huffman encoded string back to the original string.
- Simple and efficient algorithms suitable for educational purposes.

## Getting Started
### Prerequisites
`g++:` GNU C++ Compiler

### Installation
Clone the repository to your local machine:
```bash
git clone https://github.com/yourusername/huffman-encoding-decoding.git
cd huffman-encoding-decoding
```
No additional libraries are required for this project.

## Project Structure
- `encode.h`: This header fie contains all the functionality for building Huffman tree and generating Huffman encoded data.
- `decode.h`: This header fie contains all the essentials for decoding Huffman encoded data using the previously built Huffman tree.
- `huffman.cpp`: Encode and decode HuffmanCoding using the functions defined in those header file.

## HuffmanCoding Class
### Methods
- `parse_file(filename)`: Parse the file and return the character frequency stored in an array of 256 long integers.
- `buildHuffmanTree(frequencyArray)`: Takes a frequency array, then build the Huffman tree on it and returns the root of the Huffman Tree.
- `Inorder(root, code, huffmanCodes)`: Takes the root of the Huffman Tree and returns the HuffmanCodes stored in a vector of string.
- `compress(infilename, outfilename, huffmanCodes)`: Takes a input file and returns the encoded data along with the Huffman Tree.
    decode(encoded_data, tree): Takes the encoded data and the Huffman Tree and returns the decoded original string.
    Usage
