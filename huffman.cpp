#include "encode.h"
#include "decode.h"

void print_usage(const char *program_name) {
    printf("Usage: %s <mode> <inputfilename>\n", program_name);
    printf("Modes:\n");
    printf("  encode : Generate Huffman Encoding of the input\n");
    printf("  decode : Decode the input with the help of Huffman tree\n");
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
        print_usage(argv[0]);
        return 1;
    }
    const char* mode = argv[1];
    const char* inputFilename = argv[2];
	
	if( mode[0] == 'c') 
    	compress(inputFilename);
	
	if( mode[0] == 'd')
		decode(inputFilename);
    return 0;
}
