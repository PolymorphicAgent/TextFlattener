#include<iostream>

#include "File.h"
#include "Utils.h"
#include "CompressionTable.h"

#include "Ctxt/ctxt.h"

#include <fstream>

int main(int argc, char *argv[]){
    
    // Tests all the file IO functions provided in abstract class File's children
    // Utils::testFileIO();

    // Tests the function that generates relative character frequencies given a file
    // Utils::testCharFreqs();

    // If the program was not run with exactly 2 or 3 arguments, print usage message and exit
    if(argc != 3 && argc != 2){
        // Print usage in red to indicate an error/incorrect invocation
        ctxt("\nUsage: \n\n", red, false, false, false);
        ctxt(std::string(argv[0]) + " <mode> <input file>\n", green, false, false, true);
        ctxt("  <mode>: \n\n    c ---- Compression mode\n    d ---- Decompression mode\n    gc --- Generate character frequencies mode\n    gw --- Generate word frequencies mode\n    help - Display more detailed information about modes\n    test - Execute test functions", yellow, false, false, true);
        
        return 1;
    }

    // If the user requested help, print detailed usage information and exit
    std::string mode = argv[1];

    if(mode == "help"){
        // Use colored headings and descriptions for readability
        ctxt("\nUsage: \n\n", cyan, true, false, false);
        ctxt(std::string(argv[0]) + " <mode> <input file>\n", green, false, false, true);

        ctxt("Detailed Mode Information:\n\n", red, true, false, false);

        ctxt("  Compression Mode (c):\n", dark_green, false, false, false);
        ctxt("    Compresses the input text file using a predefined compression table.\n", blue, false, false, false);
        ctxt(std::string("    Example: ") + argv[0] + " c input.txt\n", magenta, false, false, false);
        ctxt("      Output files generated:\n", magenta, false, false, false);
        ctxt("        - input.bin: The compressed binary output file.\n\n", yellow, false, false, true);

        ctxt("  Decompression Mode (d):\n", dark_green, false, false, false);
        ctxt("    Decompresses the input binary file using a predefined compression table.\n", blue, false, false, false);
        ctxt(std::string("    Example: ") + argv[0] + " d input.bin\n", magenta, false, false, false);
        ctxt("      Output files generated:\n", magenta, false, false, false);
        ctxt("        - input.txt: The decompressed text output file.\n\n", yellow, false, false, true);

        ctxt("  Generate Character Frequencies Mode (gc):\n", dark_green, false, false, false);
        ctxt("    Analyzes the input text file and generates a frequency table of characters.\n", blue, false, false, false);
        ctxt(std::string("    Example: ") + argv[0] + " gc input.txt\n", magenta, false, false, false);
        ctxt("      Output files generated:\n", magenta, false, false, false);
        ctxt("        - input_char_freqs.csv: A CSV file listing each character and its frequency percentage.\n\n", yellow, false, false, true);

        ctxt("  Generate Word Frequencies Mode (gw):\n", dark_green, false, false, false);
        ctxt("    Analyzes the input text file and generates a frequency table of words.\n", blue, false, false, false);
        ctxt(std::string("    Example: ") + argv[0] + " gw input.txt\n", magenta, false, false, false);
        ctxt("      Output files generated:\n", magenta, false, false, false);
        ctxt("        - input_word_freqs.csv: A CSV file listing each word and its frequency percentage.\n\n", yellow, false, false, true);

        ctxt("  Help Mode (help):\n", dark_green, false, false, false);
        ctxt("    Displays this detailed usage information.\n", magenta, false, false, true);

        ctxt("  Test Mode (test):\n", dark_green, false, false, false);
        ctxt("    Executes built-in test functions to validate file I/O and frequency analysis functionalities.\n", magenta, false, false, false);
        ctxt(std::string("    Example: ") + argv[0] + " test\n", magenta, false, false, true);

        ctxt("Note:\n", dark_red, true, false, false);
        ctxt("  - Make sure that the input files exist and are accessible.\n", yellow, false, false, false);
        ctxt("  - The program will generate output files in the same directory as the input file.\n", yellow, false, false, false);
        ctxt("  - The compression and decompression modes rely on a predefined compression table file 'table.csv'.\n", yellow, false, false, true);

        return 0;
    }

    // If the user requested to run tests, execute the test functions and exit
    if(mode == "test"){
        Utils::testFileIO();
        Utils::testCharFreqs();
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