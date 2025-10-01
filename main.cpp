#include<iostream>

#include "File.h"
#include "Utils.h"
#include "CompressionTable.h"

#include <fstream>

int main(int argc, char *argv[]){
    
    // Tests all the file IO functions provided in abstract class File's children
    // Utils::testFileIO();

    // Tests the function that generates relative character frequencies given a file
    // Utils::testCharFreqs();

    // If the program was not run with exactly 2 or 3 arguments, print usage message and exit
    if(argc != 3 && argc != 2){
        std::cerr << "\nUsage: \n\n" << argv[0] << " <mode> <input file>\n\n";
        std::cerr << "  <mode>: \n\n    c ---- Compression mode\n    d ---- Decompression mode\n    gc --- Generate character frequencies mode\n    gw --- Generate word frequencies mode\n    help - Display more detailed information about modes\n\n";
        return 1;
    }

    // If the user requested help, print detailed usage information and exit
    std::string mode = argv[1];

    if(mode == "help"){
        std::cout << "\nUsage: \n\n" << argv[0] << " <mode> <input file>\n\n";

        std::cout << "\nDetailed Mode Information:\n\n";

        std::cout << "  Compression Mode (c):\n";
        std::cout << "    Compresses the input text file using a predefined compression table.\n";
        std::cout << "    Example: " << argv[0] << " c input.txt\n";
        std::cout << "      Output files generated:\n";
        std::cout << "        - input.bin: The compressed binary output file.\n\n";

        std::cout << "  Decompression Mode (d):\n";
        std::cout << "    Decompresses the input binary file using a predefined compression table.\n";
        std::cout << "    Example: " << argv[0] << " d input.bin\n";
        std::cout << "      Output files generated:\n";
        std::cout << "        - input.txt: The decompressed text output file.\n\n";

        std::cout << "  Generate Character Frequencies Mode (gc):\n";
        std::cout << "    Analyzes the input text file and generates a frequency table of characters.\n";
        std::cout << "    Example: " << argv[0] << " gc input.txt\n";
        std::cout << "      Output files generated:\n";
        std::cout << "        - input_char_freqs.csv: A CSV file listing each character and its frequency percentage.\n\n";

        std::cout << "  Generate Word Frequencies Mode (gw):\n";
        std::cout << "    Analyzes the input text file and generates a frequency table of words.\n";
        std::cout << "    Example: " << argv[0] << " gw input.txt\n";
        std::cout << "      Output files generated:\n";
        std::cout << "        - input_word_freqs.csv: A CSV file listing each word and its frequency percentage.\n\n";

        std::cout << "  Help Mode (help):\n";
        std::cout << "    Displays this detailed usage information.\n\n";

        std::cout << "Note:\n";
        std::cout << "  - Make sure that the input files exist and are accessible.\n";
        std::cout << "  - The program will generate output files in the same directory as the input file.\n";
        std::cout << "  - The compression and decompression modes rely on a predefined compression table file 'table.csv'.\n\n";

        return 0;
    }

    return 0;
      
}

/*

    References:


    FOR USING UNORDERED_MAP:
    - https://en.cppreference.com/w/cpp/container/unordered_map.html
    - https://stackoverflow.com/questions/19197799/what-is-the-quickest-way-of-inserting-updating-stdunordered-map-elements-witho
    - https://stackoverflow.com/questions/5056645/sorting-stdmap-using-value
    - https://stackoverflow.com/questions/42933943/how-to-use-lambda-for-stdfind-if


    FOR READING/WRITING BINARY FILES:
    - https://en.cppreference.com/w/cpp/types/integer.html
    - https://stackoverflow.com/questions/11680248/reading-a-binary-file-bit-by-bit
    - https://www.geeksforgeeks.org/dsa/little-and-big-endian-mystery/
    - https://www.w3schools.com/c/c_bitwise_operators.php


*/