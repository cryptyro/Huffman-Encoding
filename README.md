# Huffman Encoding and Decoding
This project provides an implementation of Huffman encoding and decoding algorithms. Huffman coding is a popular method of lossless data compression that assigns variable-length codes to input characters based on their frequencies. The most frequent characters get the shortest codes, making the overall encoded data smaller.

## Features
- Build a Huffman Tree based on character frequencies.
- Generate Huffman Codes from the Huffman Tree.
- Encode a given input file using the generated Huffman Codes.
- Decode a given Huffman encoded file back to the original file.
- Simple and efficient algorithms suitable for educational purposes.

## Getting Started
### Prerequisites
`g++:` GNU C++ Compiler

### Installation
Clone the repository to your local machine:
```bash
git clone https://github.com/yourusername/Huffman-Encoding.git
cd Huffman-Encoding
```
No additional libraries are required for this project.

## Project Structure
- `encode.h`: This header fie contains all the functionality for building Huffman tree and generating Huffman encoded data.
- `decode.h`: This header fie contains all the essentials for decoding Huffman encoded data using the previously built Huffman tree.
- `huffman.cpp`: Encode and decode HuffmanCoding using the functions defined in those header file.

## HuffmanCoding Class
### Methods
- `parse_file(filename)` This function reads a binary file and counts the occurrences of each characterwithin the file. It returns an array where the index corresponds to the character value (0 to 255), and the value at each index represents the count of that character.
- `buildHuffmanTree(frequencyArray)` This function constructs a Huffman tree based on the given frequency array. It uses a priority queue (min-heap) to build the tree by repeatedly merging the two nodes with the smallest frequencies until only one node remains.
- `Inorder(root, code, huffmanCodes)` This function recursively traverses the Huffman tree in an inorder manner to generate Huffman codes.
- `generate_header(huffmanCodes)` Generates a header string containing Huffman coding information for compression.
  *Format:* 
  1. Number of unique characters (1 byte)
  2. For each unique character:
      a. Character (1 byte)
      b. Length of Huffman code (1 byte)
      c. Huffman code (min: 1 byte, max: 255bytes)
  Worst case header size: 1 + (1+1)*(1+2+3+4+5+...+255) + 1 ~ 32kb... (only happens when skewed Huffman tree is generated)
  Best case header size: 1 + 1 + 1 + 1 + 1 = 5bytes (Happens only when a single character exists in an entire file).
- `compress(infilename)` Compresses a file using Huffman coding and writes the compressed data to a new file.
  The function performs the following steps:
   1. Parses the input file to compute character frequencies.
   2. Builds a Huffman tree based on the frequencies.
   3. Generates Huffman codes for each character.
   4. Writes a header containing Huffman coding information to the output file.
   5. Encodes the input file using Huffman codes and writes the encoded data to the output file.
- `decode(encoded_file)` Takes the encoded data and the Huffman Tree and returns the decoded original data.

### Example
Compile
```bash
g++ -o huffman huffman.cpp
```
Encode the file sample.txt
```bash
./huffman c sample.txt
```
