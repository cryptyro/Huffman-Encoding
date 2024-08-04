#include "encode.h"
using namespace std;

/**
 * Generate a header for the file.
 * Format: 
 * 1. Total Unique Character (1 byte)
 * 2. For each unique character:
 * 2a. Character (1 byte)
 * 2b. Length of code (1 byte)
 * 2c. Huffman code (min: 1 byte, max: 255bytes)
 * 3. Padding
 * Worst case header size: 1 + (1+1)*(1+2+3+4+5+...+255) + 1 ~ 32kb... (only happens when skewed Huffman tree is generated)
 * Best case header size: 1 + 1 + 1 + 1 + 1 = 5bytes (Happens only when a single character exists in an entire file).
 **/
string generate_header(const vector<string>& huffmanCodes) {
    string header = "";
    unsigned char UniqueCharacter = 0;   
    for (int i = 0; i < 256; ++i) {
        if (huffmanCodes[i].size()) {
            header.push_back(i);
            header.push_back(huffmanCodes[i].size());
            header += huffmanCodes[i];
            ++UniqueCharacter;
        }
    }
    return (char)UniqueCharacter+header;
}

/** Store the header information **/
void write_header(const vector<string>& huffmanCodes, const char* filename){
	FILE *file = fopen( filename , "wb");
	string header= generate_header(huffmanCodes);
	for(int i=0; i<header.size() ; ++i){
    	fputc(header[i], file);
	}
	return;
}

/** Decode the header and generate the Huffman tree**/
Node* decode_header(const char* infilename) {
    Node *root = new Node(0);
    FILE *file = fopen( infilename , "rb");
    int uniquecharacter ;
    unsigned char ch, len;
    uniquecharacter = fgetc(file);
    while(uniquecharacter > 0) {
        ch = fgetc(file);
        len = fgetc(file);
        Node *traverse = root;
        while(len > 0) {
            if(fgetc(file) == '0') {
            	if(!traverse->left) {
                	traverse->left = new Node(0);
            	}
            	traverse = traverse->left;
        	} else {
            	if(!traverse->right) {
                	traverse->right = new Node(0);
            	}
            	traverse = traverse->right;
        	}
        	--len;
        }
        // character (1byte) + length(1byte) + huffmancode(n bytes where n is length of huffmancode)
        traverse->character = ch;
        --uniquecharacter;
    }
    return root;
}

/** Decompress the encoded file**/
void decompress(const char* infilename, const char* outfilename, const char* auxfilename ) {
    FILE *iptr = fopen( infilename , "rb");
    FILE *optr = fopen( outfilename , "wb");    
    if (optr == nullptr) {
        perror("Error opening input file");
        return;
    }
    Node *root = decode_header(auxfilename);
    char ch , counter=7;
    Node *traverse = root;
    while (fread(&ch, sizeof(unsigned char), 1, iptr) == 1) {
		while (counter >= 0) {
            traverse = ch & (1 << counter) ? traverse->right : traverse->left;
            ch ^= (1 << counter);
            --counter;
            if (!traverse->left && !traverse->right) {
                fputc(traverse->character, optr);
                traverse = root;
            }
        }
        counter = 7;
    }
    fclose(iptr);
    fclose(optr);
}
