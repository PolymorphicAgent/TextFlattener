#include<iostream>

#include "File.h"
#include "Utils.h"
#include "CompressionTable.h"

#include "Ctxt/ctxt.h"

#include <fstream>
#include <algorithm>
#include <iomanip>

int main(int argc, char *argv[]){

    // std::cout<<"’, ”, “\n";return 0;

    // BinaryFile test("Articles.bin");
    // test.read();

    // // int i = 0;
    // for(auto v:*test.getData()){
    //     std::cout<<v;
    //     // if(i == 50)break;
    //     // i++;
    // }
    // return 0;
    // CSVFile test("table.csv");
    // test.read();
    
    // for(auto row: *test.getData()){
    //     for(auto column: row){
    //         std::cout<<column<<", ";
    //     }
    //     std::cout<<"\n";
    // }

    // argc = 3;argv[1] = "c";argv[2] = "sample.txt";

    // If the program was not run with exactly 2, 3, or 4 arguments, print usage message and exit
    if(argc != 4 && argc != 3 && argc != 2){
        Utils::printUsage(argv);
        return 1;
    }

    // If the user requested help, print detailed usage information and exit
    std::string mode = argv[1];

    if(mode == "help"){
        Utils::printHelp(argv);

        // If extraneous arguments were provided, warn the user
        if(argc != 2){
            ctxt("\nWarning: Extraneous arguments provided with 'help' mode. They have been ignored.\n", yellow, false, false, true);
        }
        return 0;
    }

    // If the user requested to run tests, execute the test functions and exit
    if(mode == "test"){
        Utils::testFileIO();
        Utils::testCharFreqs();
        Utils::testWordFreqs();
        Utils::testAccuracy();

        // If extraneous arguments were provided, warn the user
        if(argc != 2){
            ctxt("\nWarning: Extraneous arguments provided with 'test' mode. They have been ignored.\n", yellow, false, false, true);
        }

        return 0;
    }

    // Verify that the mode argument is valid
    if(mode != "c" && mode != "d" && mode != "gc" && mode != "gw" && mode != "acc"){
        ctxt("\nError: Invalid mode '"+mode+"'.\n", dark_red, false, false, false);
        Utils::printUsage(argv);

        return 1;
    }

    // For other modes, make sure an input file argument was provided
    if(argc < 3){
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

        // If extraneous arguments were provided, warn the user
        if(argc > 4){
            ctxt("\nWarning: Extraneous arguments provided with 'c' mode. They have been ignored.\n", yellow, false, false, true);
        }

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

        // Check if table.csv exists in the csv directory
        CSVFile csvCompTable("./csv/table.csv");
        try {
            csvCompTable.read();
        } catch (const std::exception &e) {
            ctxt(std::string("\nError: Could not read './csv/table.csv'. Make sure it exists in the csv directory. (")+e.what()+")\n", red, false, false, true);
            return 1;
        }

        std::string outputFilePath;
        if(argc == 4){
            outputFilePath = argv[3];
        }
        else outputFilePath = inputFileNameNoExt + ".bin";

        CompressionTable table(csvCompTable, CompressionTable::Compress);

        std::vector<bool> finalBinary;

        std::string currentWord;
        std::string::iterator it;
        
        // Get the entire content as a string
        std::string content = txtFile.getData()->str();
        
        // Normalize em-dash (U+2014) to ASCII hyphen-minus '-' so it maps to '-' in table.csv
        const std::string emdash_utf8 = "\xE2\x80\x94"; // U+2014
        const std::string emdash_cp = "\x97"; // CP1252 0x97
        size_t p = 0;
        while((p = content.find(emdash_utf8, p)) != std::string::npos){ content.replace(p, emdash_utf8.size(), "-"); p += 1; }
        p = 0;
        while((p = content.find(emdash_cp, p)) != std::string::npos){ content.replace(p, emdash_cp.size(), "-"); p += 1; }

        // Normalize é to e
        const std::string e_acute_utf8 = "\xC3\xA9"; // U+00E9
        const std::string e_acute_cp = "\xE9"; // CP1252 0xE9
        p = 0;
        while((p = content.find(e_acute_utf8, p)) != std::string::npos){ content.replace(p, e_acute_utf8.size(), "e"); p += 1; }
        p = 0;
        while((p = content.find(e_acute_cp, p)) != std::string::npos){ content.replace(p, e_acute_cp.size(), "e"); p += 1; }

        // Iterate through the content decoding UTF-8 code points so multi-byte characters
        // (like smart quotes) are treated as single string tokens and passed intact to mapStrToBin
        for(size_t idx = 0; idx < content.size(); ){
            unsigned char lead = static_cast<unsigned char>(content[idx]);

            // ********** GITHUB COPILOT USED FOR THIS SECTION **********
            size_t charLen = 1;
            if((lead & 0x80) == 0) charLen = 1;            // ASCII
            else if((lead & 0xE0) == 0xC0) charLen = 2;   // 2-byte UTF-8
            else if((lead & 0xF0) == 0xE0) charLen = 3;   // 3-byte UTF-8
            else if((lead & 0xF8) == 0xF0) charLen = 4;   // 4-byte UTF-8

            // Safeguard: don't run past the content
            if(idx + charLen > content.size()) charLen = 1;

            std::string ch = content.substr(idx, charLen);
            // ********** END GITHUB COPILOT SECTION **********

            // Decide whether ch is whitespace or punctuation (note: isspace/ispunct operate on single-byte chars)
            bool is_space = (charLen == 1 && isspace(static_cast<unsigned char>(ch[0])));
            bool is_punct = false;
            if(charLen == 1) is_punct = ispunct(static_cast<unsigned char>(ch[0]));

            // Treat ASCII apostrophe specially (keep inside words)
            if(is_space || (is_punct && ch != "'")){
                if(!currentWord.empty()){
                    try{
                        std::vector<bool> wordBinRep = table.mapStrToBin(currentWord);
                        if(wordBinRep.size() != 0 && currentWord.length() > 1){
                            finalBinary.insert(finalBinary.end(), wordBinRep.begin(), wordBinRep.end());
                            currentWord.clear();
                        } else {
                            for(size_t k = 0; k < currentWord.size(); ){
                                // ********** GITHUB COPILOT USED FOR THIS SECTION **********
                                // extract next UTF-8 char from currentWord
                                unsigned char l = static_cast<unsigned char>(currentWord[k]);
                                size_t lLen = 1;
                                if((l & 0x80) == 0) lLen = 1;
                                else if((l & 0xE0) == 0xC0) lLen = 2;
                                else if((l & 0xF0) == 0xE0) lLen = 3;
                                else if((l & 0xF8) == 0xF0) lLen = 4;
                                std::string cc = currentWord.substr(k, lLen);
                                // ********** END GITHUB COPILOT SECTION ********** 

                                std::vector<bool> charBinRep = table.mapStrToBin(cc);
                                finalBinary.insert(finalBinary.end(), charBinRep.begin(), charBinRep.end());
                                k += lLen;
                            }
                            currentWord.clear();
                        }
                    } catch (const std::exception &e){
                        ctxt(std::string("\nError mapping word to binary: ") + e.what(), red, false, false, true);
                        return 1;
                    }
                }

                // map the space or punctuation itself
                try{
                    std::vector<bool> charBinRep = table.mapStrToBin(ch);
                    finalBinary.insert(finalBinary.end(), charBinRep.begin(), charBinRep.end());
                } catch (const std::exception &e){
                    ctxt(std::string("\nError mapping character to binary: ") + e.what(), red, false, false, true);
                    return 1;
                }
            } else {
                // append this UTF-8 char to currentWord
                currentWord += ch;
            }

            idx += charLen;
        }
        
        // Process any remaining word at the end
        if(!currentWord.empty()) {
            try {
                std::vector<bool> wordBinRep = table.mapStrToBin(currentWord);
                if(wordBinRep.size() != 0 && currentWord.length() > 1) {
                    finalBinary.insert(finalBinary.end(), wordBinRep.begin(), wordBinRep.end());
                } else {
                    for(char c : currentWord) {
                        std::vector<bool> charBinRep = table.mapStrToBin(std::string(1, c));
                        finalBinary.insert(finalBinary.end(), charBinRep.begin(), charBinRep.end());
                    }
                }
            } catch (const std::exception &e) {
                ctxt(std::string("\nError mapping final word to binary: ") + e.what(), red, false, false, true);
                return 1;
            }
        }
        
        // Write the binary vector to the file
        BinaryFile binOut(outputFilePath);
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

        // If extraneous arguments were provided, warn the user
        if(argc > 4){
            ctxt("\nWarning: Extraneous arguments provided with 'd' mode. They have been ignored.\n", yellow, false, false, true);
        }

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
        CSVFile csvCompTable("./csv/table.csv");
        try {
            csvCompTable.read();
        } catch (const std::exception &e) {
            ctxt(std::string("\nError: Could not read './csv/table.csv'. Make sure it exists in the csv directory.\n"), red, false, false, true);
            return 1;
        }

        std::string outputFilePath;
        if(argc == 4)
            outputFilePath = argv[3];
        else outputFilePath = inputFileNameNoExt + ".txt";

        CompressionTable table(csvCompTable, CompressionTable::Decompress);

        TextFile outFile(outputFilePath);

        // Iterate through the binary bits
        bool first = true;
        unsigned int count = 0;
        std::vector<bool> tmp;
        size_t i = 0;
        for(bool b : *binFile.getData()){

            // If starting a new representation, clear tmp and record the leading bit and expected remaining bits
            if(first){
                tmp.clear();
                tmp.push_back(b);
                first = false;
                count = b ? 3 : 6; // short (1 + 3) or long (1 + 6)
            }
            // Otherwise we are continuing the current representation
            else {
                tmp.push_back(b);
                if(count > 0) count--; // consume one of the remaining bits

                // If we've read the last required bit for this representation, map and output it now
                if(count == 0){
                    first = true;

                    // Stream our temporary representation to the file
                    std::string tmpString;
                    try {
                        tmpString = table.mapBinToStr(tmp);
                    }
                    catch (const std::exception &e){
                        ctxt(std::string("Error mapping Binary to String: ")+e.what(), red, true, false, true);
                        return 1;
                    }

                    *outFile.getData() << tmpString;

                    // //debug
                    // if(tmpString == " ")std::cout<<"<s>";
                    // else if(tmpString == "\n")std::cout<<"<n>";
                    // else std::cout<<tmpString;
                    
                    tmp.clear();
                }
            }

            ++i;
        }

        // Write out the file
        try {
            outFile.write();
        }
        catch (const std::exception &e) {
            ctxt(std::string("Error Writing File: ")+e.what(), red, true, false, true);
            return 1;
        }

        // Success!
        ctxt(std::string("\nSuccessfully decompressed '") + inputFileName + "' to '" + outputFilePath + "'.\n", green, false, false, true);
    }

    // Handle generate character frequencies mode
    else if(mode == "gc"){

        // If extraneous arguments were provided, warn the user
        if(argc != 3){
            ctxt("\nWarning: Extraneous arguments provided with 'gc' mode. They have been ignored.\n", yellow, false, false, true);
        }

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

        // If extraneous arguments were provided, warn the user
        if(argc != 3){
            ctxt("\nWarning: Extraneous arguments provided with 'gw' mode. They have been ignored.\n", yellow, false, false, true);
        }

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
    else if(mode == "acc"){

        // If not enough arguments provided, error
        if(argc < 4){
            ctxt("\nError: Accuracy comparison mode requires two .txt input files.\n", red, false, false, true);
            return 1;
        }

        // If extraneous arguments were provided, warn the user
        if(argc > 4){
            ctxt("\nWarning: Extraneous arguments provided with 'acc' mode. They have been ignored.\n", yellow, false, false, true);
        }

        // Make sure that the input files are .txt files
        std::string inputFilePath2 = argv[3];
        std::string inputFileName2 = Utils::extractFileName(inputFilePath2);
        std::string inputFileExt2 = Utils::extractFileExtension(inputFilePath2);

        if(inputFileExt != "txt" || inputFileExt2 != "txt"){
            ctxt("\nError: Accuracy comparison mode requires two .txt input files.\n", red, false, false, true);
            return 1;
        }

        // Create TextFile objects for both input files
        TextFile originalFile(inputFilePath);
        TextFile decompressedFile(inputFilePath2);

        // Read the content of the original text file
        try {
            originalFile.read();
        } catch (const std::exception &e) {
            ctxt(std::string("\nError reading original file: ") + e.what() + "\n", red, false, false, true);
            return 1;
        }

        // Read the content of the decompressed text file
        try {
            decompressedFile.read();
        } catch (const std::exception &e) {
            ctxt(std::string("\nError reading copied file: ") + e.what() + "\n", red, false, false, true);
            return 1;
        }

        // Generate accuracy
        double accuracy = Utils::genAccuracy(&originalFile, &decompressedFile);

        ctxt(std::string("\nThe accuracy of '") + inputFileName2 + "' against '" + inputFileName + "' is " + std::to_string(accuracy) + "%.\n", green, false, false, true);
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
    - https://codeshack.io/text-to-ascii-converter/

    GITHUB COPILOT was used on 15 lines of code in two sections due to my infamiliarity with text encoding. These lines can be found surrounded by the following comments:
    
    // ********** GITHUB COPILOT USED FOR THIS SECTION **********
    // ********** END GITHUB COPILOT SECTION ********** 

*/