#include <iostream>
#include <vector>
#include <fstream>
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

/** Function to parses the file for character count **/
vector<ll> parse_file(const char* filename) {
	vector<ll> char_count(MAX_CHAR, 0); // Initialize all counts to 0
	// Open the input file in binary read mode
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error opening input file" << endl;
        exit(EXIT_FAILURE);
    }
	// Read the file character by character until EOF
    unsigned char character;
    while (file.read(reinterpret_cast<char*>(&character), sizeof(character))) {
        char_count[character]++;
        infile_size++; //Get the inputflesize
    }
    file.close();// Close the file
    return char_count; // Return the array of character counts
}

/** Function to build the Huffman tree **/
Node* buildHuffmanTree(const vector<ll>& frequencyArray) {
    vector<Node*> nodes;// Vector to hold leaf nodes based on character frequencies
	// Create leaf nodes for characters with non-zero frequencies
    for (int i = 0; i < MAX_CHAR; ++i) {
        if (frequencyArray[i] > 0)
            nodes.push_back(new Node(static_cast<char>(i), frequencyArray[i]));
    }
    // Merge nodes until there's only one node (the root) remains
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

/**Function to generate and print huffman codes for each character.**/
void Inorder(Node *root, string code, vector<string>& huffmanCodes) {;
    if (root) { // Check if the current node is not null
        // Traverse the left subtree, appending '0' to the current code
        code.push_back('0');
        Inorder(root->left, code, huffmanCodes);
        // Backtrack by removing the last character ('0') from the code
        code.pop_back();
        
         // If the current node is a leaf node (i.e., it has no left or right child)
        if (root->left == nullptr && root->right == nullptr) {
        	// Store the generated Huffman code for the character at this leaf node
            huffmanCodes[(unsigned char)root->character] = code;
            // Print the character, its frequency, and its Huffman code
            cout << "Character: " << root->character
                 << "  Frequency: " << root->frequency
                 << "  Huffman Code: " << code << endl;
            // Calculate the expected length of the encoded file
            explen += ((double)(root->frequency)/infile_size)* code.size();
        }
        // Traverse the right subtree, appending '1' to the current code
        code.push_back('1');
        Inorder(root->right, code, huffmanCodes);
        // Backtrack by removing the last character ('1') from the code
        code.pop_back();
    }
}

 
/**Generates a header string containing Huffman coding information for compression.**/
string generate_header(const vector<string>& huffmanCodes) {
    string header = "";
    // Number of unique characters with non-zero Huffman codes
    unsigned char UniqueCharacter = 0;
    // Iterate over all possible ASCII characters
    for (int i = 0; i < 256; ++i) {
    	// Check if the character has a non-empty Huffman code
        if (huffmanCodes[i].size()) {
            header.push_back(i); // Add character to the header
            header.push_back(huffmanCodes[i].size()); // Add the length of the Huffman code for the character
            header += huffmanCodes[i];// Add the Huffman code itself
            ++UniqueCharacter; // Increment the count of unique characters
        }
    }
    // Prepend the number of unique characters to the header
    return string(1, UniqueCharacter) + header;
}


/**Compresses a file using Huffman coding and writes the compressed data to a new file**/
void compress(const char* infilename) {
	vector<ll> frequencyArray = parse_file(infilename);// Step 1: Calculate character frequencies
    Node* root = buildHuffmanTree(frequencyArray);// Step 2: Build Huffman tree
    vector<string> huffmanCodes(MAX_CHAR);// Vector to store Huffman codes for each character
	Inorder(root, "", huffmanCodes);// Step 3: Generate Huffman codes
	delete root;// Clean up memory used for Huffman tree
		
    cout<<endl<<"Expected codelength is = "<<explen << endl;
    cout<<endl<<endl<<"Input file size :"<<infile_size<<"bytes"<<endl;

    // Create the output file name with ".encoded" suffix and open it in binary write mode
    string outfilename = string(infilename) + ".encode";
    ofstream ofile(outfilename, ios::binary);
    if (!ofile) {
        cerr << "Error opening output file" << endl;
        return;
    }
        
    // Step 4: Write header information to the output file
    string header = generate_header(huffmanCodes);
    ofile.write(header.c_str(), header.size());

	ifstream ifile(infilename, ios::binary);
    if (!ifile) {
        cerr << "Error opening input file" << endl;
        return;
    }

	unsigned char ch, fch=0; // `ch` for reading input characters, `fch` for writing encoded bits
	unsigned char counter = 7;    // Bit position counter (7 for the most significant bit)
	int  i; // Index for iterating through the Huffman code
	string code; // Huffman code corresponding to the current character
	
    // Read the inputfile character by character encode using Huffman codes until EOF
    while (ifile.read(reinterpret_cast<char*>(&ch), sizeof(ch))) {
        code = huffmanCodes[ch];
        i=0;
        while(code[i] != '\0') {
            fch = fch | ((code[i] - '0') << counter);// Append the current bit to `fch`
            counter = (counter +7) & 7;// Decrement counter from 7 to 0, then wrap around
            if(counter == 7) {// When counter resets to 7, write the byte to the output file
                ofile.put(fch);
                outfile_size++;
                fch = 0; // Reset `fch` for the next byte
            }
            i++;
        }
    }
    // Write any remaining bits in `fch` to the output file
    if(fch) {
      	ofile.put(fch);
      	outfile_size++;
    }
    // Close the input and output files
    ifile.close();
    ofile.close();
    // Display the results
    cout<<"Output file size :"<<outfile_size<<"bytes"<<endl;
    cout<<"Compress Ratio : "<<(double)infile_size/outfile_size<<endl;
	cout<<"Voila!! File has been successfully compressed and stored into "<<outfilename<<" with the corresponding huffman tree."<<endl;
}
