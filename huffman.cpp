#include "decode.h"

int main() {
	string str1,  str2 , str3;
	char choice;
	cout<<"Enter 'c' to compress/encode or 'd' to decompress/decode : ";
	cin>>choice;
	
    cout<< "Enter the input file name : ";
    cin>> str1 ;
    const char *inputFilename = str1.c_str();
	
	if( choice == 'c') {
    	// Calculate character frequencies
		vector<ll> frequencyArray = parse_file(inputFilename);

    	// Build Huffman tree
    	Node* root = buildHuffmanTree(frequencyArray);

    	// Generate Huffman codes
    	vector<string> huffmanCodes(MAX_CHAR);
    	Inorder(root, "", huffmanCodes);
    	
    	cout<<endl<<"Expected codelength is = "<<explen << endl;
    	cout<<endl<<endl<<"Input file size :"<<infile_size<<"bytes"<<endl;

    	// Write encoded data to binary file
    	str2= str1+".encoded";
    	const char *outputFilename = str2.c_str();
    	compress(inputFilename , outputFilename, huffmanCodes);
    	
    	cout<<"Output file size :"<<outfile_size<<"bytes"<<endl;
       	cout<<"Compress Ratio : "<<(double)infile_size/outfile_size;
		
		// Write header data to binary file
		write_header(huffmanCodes, "tree.info");
		
		cout<<endl<<"Voila!! File has been successfully compressed and stored into "<<str2<<" with the corresponding huffman tree stored into tree.info."<<endl;
		
    	// Clean up memory
	    delete root;
	}
	
	if( choice == 'd') {
	    cout<< "Enter the auxiliary file name that was created during the encoding (containing the Huffman tree) : ";
    	cin>> str3 ;
    	const char *auxfile = str3.c_str();
    	str2= str1+".og";
    	const char *outputFilename = str2.c_str();

    	// Build Huffman tree
    	Node* root = decode_header(auxfile);

		// Retrieve the original data
		decompress(inputFilename , outputFilename, auxfile);
		
		cout<<endl<<"Voila!! File has been successfully decoded and stored into "<<str2<<endl;

    	// Clean up memory
	    delete root;
	}	
    return 0;
}
