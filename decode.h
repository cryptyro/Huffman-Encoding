#include "encode.h"
using namespace std;

/** Decode the header and generate the Huffman tree**/
void decode(const char* infilename) {
    Node *root = new Node(0);// Create the root of the Huffman tree
    
    // Open the input file in binary read mode
    ifstream file(infilename, ios::binary);
    if (!file) {
        cerr << "Error opening input file" << endl;
        return;
    }

    int uniqueCharacterCount = file.get();// Read the number of unique characters from the file
    while(uniqueCharacterCount > 0) {
        unsigned char ch = file.get(); // Character
        unsigned char len = file.get(); // Length of Huffman code
        
        Node* traverse = root;
        // Construct the Huffman tree
        while (len > 0) {
            if (file.get() == '0') {
                if (!traverse->left)
                    traverse->left = new Node(0); // Go to the left child
                traverse = traverse->left;
            } else {
                if (!traverse->right) 
                    traverse->right = new Node(0); // Go to the right child
                traverse = traverse->right;
            }
            --len;
        }
        // character (1byte) + length(1byte) + huffmancode(n bytes where n is length of huffmancode)
        traverse->character = ch; // Set the character in the node
        --uniqueCharacterCount;
    }

	// Create the output file name and open it in binary write mode
    string outfilename = string(infilename) + ".og";
    ofstream optr(outfilename, ios::binary);
    if (!optr) {
        cerr << "Error opening output file" << endl;
        file.close();
        return;
    }
    
    // Decompress the encoded file
    unsigned char buffer;
    Node* traverse = root;
    while (file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer))) {
        for (int counter = 7; counter >= 0; --counter) {
            traverse = (buffer & (1 << counter)) ? traverse->right : traverse->left;
            if (!traverse->left && !traverse->right) {
                // Leaf node reached; output the character
                optr.put(traverse->character);
                traverse = root; // Go back to the root of the Huffman tree
            }
        }
    }
    
    // Close the files
    file.close();
    optr.close();
    cout<<endl<<"Voila!! File has been successfully decoded and stored into "<<outfilename<<endl;

	// Clean up memory
	delete root, traverse;
	
	return;
}
