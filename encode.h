#include <iostream>
#include <vector>
#include <cstdio>
#define ll unsigned long long int
#define MAX_CHAR 256 // Assuming ASCII characters

using namespace std;

ll infile_size=0, outfile_size=0;
long double explen = 0.0;

//structure for storing nodes.
struct Node {
    char character;
    ll frequency;
    Node *left, *right;

    Node(ll frequency) {
        this->character = 0;
        this->frequency = frequency;
        this->left = this->right = nullptr;
    }

    Node(char character, ll frequency) {
        this->character = character;
        this->frequency = frequency;
        this->left = this->right = nullptr;
    }
};


/** Function to build the Huffman tree **/
Node* buildHuffmanTree(const vector<ll>& frequencyArray) {
    vector<Node*> nodes;
	// Create leaf nodes
    for (int i = 0; i < MAX_CHAR; ++i) {
        if (frequencyArray[i] > 0) {
            nodes.push_back(new Node(static_cast<char>(i), frequencyArray[i]));
        }
    }
    // Merge nodes until there's only one node left
    while (nodes.size() > 1) {
        // Find two nodes with the lowest frequencies
        Node* smallest1 = nodes[0];
        Node* smallest2 = nodes[1];
        for (int i = 2; i < nodes.size(); ++i) {
            if (nodes[i]->frequency < smallest1->frequency) {
                smallest2 = smallest1;
                smallest1 = nodes[i];
            } else if (nodes[i]->frequency < smallest2->frequency)
                smallest2 = nodes[i];
        }
        // Create a new node by merging the two smallest nodes
        Node* merged = new Node(smallest1->frequency + smallest2->frequency);
        merged->left = smallest1;
        merged->right = smallest2;
        // Remove the merged nodes from the vector
        vector<Node*> newNodes;
        for (size_t i = 0; i < nodes.size(); ++i) {
            if (nodes[i] != smallest1 && nodes[i] != smallest2)
                newNodes.push_back(nodes[i]);
        }
        newNodes.push_back(merged);
        nodes = newNodes;
    }
    // Return the root of the Huffman tree
    return nodes.front();
}


/*Function to generate and print huffman codes for each character.*/
void Inorder(Node *root, string code, vector<string>& huffmanCodes) {;
    if (root) {
        code.push_back('0');
        Inorder(root->left, code, huffmanCodes);
        code.pop_back();
        if (root->left == nullptr && root->right == nullptr) {
            huffmanCodes[(unsigned char)root->character] = code;
            cout<<"Character: "<< root->character << "  Frequency: "<<root->frequency << "  Huffman Code: " << code << endl;
            explen += ((double)(root->frequency)/infile_size)* code.size();
        }
        code.push_back('1');
        Inorder(root->right, code, huffmanCodes);
        code.pop_back();
    }
}


/** Function to parses the file for character count **/
vector<ll> parse_file(const char* filename) {
	unsigned char character;
	vector<ll> char_count(256,0);
	// Open the binary file in read mode
    FILE *file = fopen( filename , "rb");
    if (file == nullptr) {
        perror("Error opening input file");
        return vector<ll>();
    }
    // Read the file character by character until EOF
    while (fread(&character, sizeof(unsigned char), 1, file) == 1) {
        char_count[character]++;
        infile_size++;
    }
    fclose(file);
    return char_count;
}

/** Determine file size **/



/** Actual compression of a file.**/
void compress(const char* infilename, const char* outfilename, const vector<string>& huffmanCodes) {
	unsigned char ch , fch=0 , counter=7;
	int  i;
	string code;
	// Open the binary file in read mode
    FILE *infile = fopen( infilename , "rb");
    FILE *outfile = fopen( outfilename , "wb");
    if (infile == nullptr) {
        perror("Error opening input file");
        return;
    }  
    // Read the file character by character until EOF
    while (fread(&ch, sizeof(unsigned char), 1, infile) == 1) {
        code = huffmanCodes[ch];
        i=0;
        while(code[i] != '\0') {
            fch = fch | ((code[i] - '0') << counter);
            counter = (counter +7) & 7;  //Decrement from 7 down to zero, and then back again at 7
            if(counter == 7) {
                fputc(fch, outfile);
                outfile_size++;
                fch = 0;
            }
            i++;
        }
    }
    if(fch) {
      	fputc(fch, outfile);
      	outfile_size++;
    }
    fclose(infile);
    fclose(outfile);
}
