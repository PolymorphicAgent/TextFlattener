#include<iostream>

#include "File.h"
#include "Utils.h"
#include "CompressionTable.h"

#include "Ctxt/ctxt.h"

#include <fstream>

int main(int argc, char *argv[]){

    // If the program was not run with exactly 2 or 3 arguments, print usage message and exit
    if(argc != 3 && argc != 2){
        Utils::printUsage(argv);
        return 1;
    }

    // If the user requested help, print detailed usage information and exit
    std::string mode = argv[1];

    if(mode == "help"){
        Utils::printHelp(argv);
        return 0;
    }

    // If the user requested to run tests, execute the test functions and exit
    if(mode == "test"){
        Utils::testFileIO();
        Utils::testCharFreqs();
        return 0;
    }

    // Verify that the mode argument is valid
    if(mode != "c" && mode != "d" && mode != "gc" && mode != "gw"){
        ctxt("\nError: Invalid mode '"+mode+"'.\n", dark_red, false, false, false);
        Utils::printUsage(argv);
        return 1;
    }

    // For other modes, make sure an input file argument was provided
    if(argc != 3){
        ctxt("Error: Input file argument is required for mode '"+mode+"'.\n", red, false, false, true);
        Utils::printUsage(argv);
        return 1;
    }

    std::string inputFilePath = argv[2];
    std::string inputFileName = Utils::extractFileName(inputFilePath);
    std::string inputFileExt = Utils::extractFileExtension(inputFilePath);
    std::string inputFileNameNoExt = Utils::removeFileExtension(inputFileName);

    // Handle compression mode
    if(mode == "c"){

        // Ensure the input file is a .txt file
        if(inputFileExt != "txt"){
            ctxt("\nError: Compression mode requires a .txt input file.\n", red, false, false, true);
            return 1;
        }

        // Create a TextFile object for the input file
        TextFile txtFile(inputFilePath);

        // Read the content of the text file
        try {
            txtFile.read();
        } catch (const std::exception &e) {
            ctxt(std::string("\nError reading input file: ") + e.what() + "\n", red, false, false, true);
            return 1;
        }

        // Create a CompressionTable object
        // CompressionTable compTable;

        // Create a BinaryFile object for the output compressed file
        std::string outputFilePath = inputFileNameNoExt + ".bin";
        BinaryFile binFile(outputFilePath);

        // Compress the text data and write to the binary file
        // TODO: Implement compression logic here using compTable

        //ctxt(std::string("\nSuccessfully compressed '") + inputFileName + "' to '" + outputFilePath + "'.\n", green, false, false, true);
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