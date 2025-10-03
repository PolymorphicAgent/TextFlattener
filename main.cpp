#include<iostream>

#include "File.h"
#include "Utils.h"
#include "CompressionTable.h"

#include "Ctxt/ctxt.h"

#include <fstream>
#include <algorithm>

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
        ctxt("\nError: Input file argument is required for mode '"+mode+"'.", red, false, false, true);
        Utils::printUsage(argv);
        return 1;
    }

    std::string inputFilePath = argv[2];
    std::string inputFileName = Utils::extractFileName(inputFilePath);
    std::string inputFileExt = Utils::extractFileExtension(inputFilePath);
    std::string inputFileNameNoExt = Utils::removeFileExtension(inputFileName);

    // Handle compression mode
    if(mode == "c"){

        // Make sure the input file is a .txt file
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

        // Check if table.csv exists in the current directory
        CSVFile csvCompTable("table.csv");
        try {
            csvCompTable.read();
        } catch (const std::exception &e) {
            ctxt(std::string("\nError: Could not read 'table.csv'. Make sure it exists in the current directory.\n"), red, false, false, true);
            return 1;
        }

        std::string outputFilePath = inputFileNameNoExt + ".bin";

        CompressionTable table(csvCompTable, CompressionTable::Compress);

        // Iterate through the text file word by word
        std::string word;
        std::vector<bool> finalBinary;
        while(*txtFile.getData() >> word){
            
            // Get the pure word (without punctuation, but with apostrophes)
            std::string pureWord = word;
            
            pureWord.erase(std::remove_if(pureWord.begin(), pureWord.end(), [](char i){ return ::ispunct(static_cast<unsigned char>(i)) && i != '\''; }), pureWord.end());

            // Check if the word is part of our table
            std::vector<bool> wordBinRep;
            try {
                wordBinRep = table.mapStrToBin(pureWord);
            } catch (const std::exception &e) {
                ctxt(std::string("\nError mapping to binary: ") + e.what(), red, false, false, true);
                return 1;
            }

            // If the word is part of our table
            if(wordBinRep.size() != 0){
                
                // Determine and build the binary vector for this part (keeping in mind any punctuation)
                for(unsigned int i = 0; i < wordBinRep.size(); i++){

                    // Check if this index is the beginning of our pure word
                    if(word.substr(i) == pureWord)
                        // Append the binary bits for this word to the built vector
                        finalBinary.insert(finalBinary.end(), wordBinRep.begin(), wordBinRep.end());

                    // Otherwise simply append the binary value for the current character
                    else {

                        // Attempt to map the character to a binary value
                        try {
                            std::vector<bool> tmp = table.mapStrToBin(std::string(1, word.at(i)));

                            // Append the binary values to the built vector
                            finalBinary.insert(finalBinary.end(), tmp.begin(), tmp.end());

                        } catch (const std::exception &e) {
                            ctxt(std::string("\nError mapping to binary: ") + e.what(), red, false, false, true);
                            return 1;
                        }
                    }
                }
            }

            // Otherwise, simply iterate through each character and append binary mappings
            else {
                for(char i : word){

                    // Attempt to map the character to a binary value
                    try {
                        std::vector<bool> tmp = table.mapStrToBin(std::string(1, i));

                        // Append the binary values to the built vector
                        finalBinary.insert(finalBinary.end(), tmp.begin(), tmp.end());

                    } catch (const std::exception &e) {
                        ctxt(std::string("\nError mapping to binary: ") + e.what(), red, false, false, true);
                        return 1;
                    }
                }
            }
        }
        
        // Write the binary vector to the file
        BinaryFile binOut(inputFileNameNoExt + ".bin");
        binOut.setData(&finalBinary);

        try {
            binOut.write();
        } catch (const std::exception &e) {
            ctxt(std::string("\nError writing to binary file: ") + e.what(), red, false, false, true);
            return 1;
        }

        ctxt(std::string("\nSuccessfully compressed '") + inputFileName + "' to '" + outputFilePath + "'.\n", green, false, false, true);
    }

    // Handle decompression mode
    else if(mode == "d"){

        // Make sure that the input file is a .bin file
        if(inputFileExt != "bin"){
            ctxt("\nError: Decompression mode requires a .bin input file.\n", red, false, false, true);
            return 1;
        }

        // Create a BinaryFile object for the input file
        BinaryFile binFile(inputFilePath);

        // Read the content of the binary file
        try {
            binFile.read();
        } catch (const std::exception &e) {
            ctxt(std::string("\nError reading input file: ") + e.what() + "\n", red, false, false, true);
            return 1;
        }

        // Check if table.csv exists in the current directory
        CSVFile csvCompTable("table.csv");
        try {
            csvCompTable.read();
        } catch (const std::exception &e) {
            ctxt(std::string("\nError: Could not read 'table.csv'. Make sure it exists in the current directory.\n"), red, false, false, true);
            return 1;
        }

        std::string outputFilePath = inputFileNameNoExt + ".txt";
        //TODO: Implement decompression logic here using compTable
    }

    // Handle generate character frequencies mode
    else if(mode == "gc"){

        // Make sure the input file is a .txt file
        if(inputFileExt != "txt"){
            ctxt("\nError: Generate character frequencies mode requires a .txt input file.\n", red, false, false, true);
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

        // Generate character frequencies
        auto freqs = Utils::genCharFreqs(&txtFile);

        // Write frequencies to a CSV file
        std::string outputFilePath = inputFileNameNoExt + "_char_freqs.csv";
        CSVFile freqFile(outputFilePath);
        std::vector<std::vector<std::string>> freqData;

        // Prepare data for CSV
        freqData.emplace_back(std::vector<std::string>{"Character", "Frequency (%)"});

        // Populate frequency data
        for(const auto& pair : *freqs){
            std::string charStr(1, pair.first);
            if(pair.first == '\n') charStr = "<newline>";
            else if(pair.first == ' ') charStr = "<space>";
            else if(pair.first == ',') charStr = "\",\"";
            freqData.emplace_back(std::vector<std::string>{charStr, std::to_string(pair.second)});
        }

        // Set data
        freqFile.setData(&freqData);

        // Write to CSV file
        try {
            freqFile.write();
        } catch (const std::exception &e) {
            ctxt(std::string("\nError writing frequency file: ") + e.what() + "\n", red, false, false, true);
            delete freqs;
            return 1;
        }

        // Clean up allocated memory
        delete freqs; 

        ctxt(std::string("\nSuccessfully generated character frequencies and wrote to '") + outputFilePath + "'.\n", green, false, false, true);
    }

    // Handle generate word frequencies mode
    else if(mode == "gw"){

        // Make sure the input file is a .txt file
        if(inputFileExt != "txt"){
            ctxt("\nError: Generate word frequencies mode requires a .txt input file.\n", red, false, false, true);
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

        // Generate word frequencies
        auto freqs = Utils::genWordFreqs(&txtFile);

        // Write frequencies to a CSV file
        std::string outputFilePath = inputFileNameNoExt + "_word_freqs.csv";
        CSVFile freqFile(outputFilePath);
        std::vector<std::vector<std::string>> freqData;

        // Prepare data for CSV
        freqData.emplace_back(std::vector<std::string>{"Word", "Frequency (%)"});

        // Populate frequency data
        for(const auto& pair : *freqs){
            std::string wordStr = pair.first;
            if(wordStr.find(',') != std::string::npos){
                wordStr = "\"" + wordStr + "\""; // Escape commas in words
            }
            freqData.emplace_back(std::vector<std::string>{wordStr, std::to_string(pair.second)});
        }

        // Set data
        freqFile.setData(&freqData);

        // Write to CSV file
        try {
            freqFile.write();
        } catch (const std::exception &e) {
            ctxt(std::string("\nError writing frequency file: ") + e.what() + "\n", red, false, false, true);
            delete freqs;
            return 1;
        }

        // Clean up allocated memory
        delete freqs; 

        ctxt(std::string("\nSuccessfully generated word frequencies and wrote to '") + outputFilePath + "'.\n", green, false, false, true);
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

    MISC.
    - https://www.geeksforgeeks.org/cpp/how-to-append-a-vector-to-a-vector-in-cpp/

*/