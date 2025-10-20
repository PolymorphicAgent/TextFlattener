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

        // Normalize the text file in memory (change smart quotes and apostrophes to straight ones, mdashes to -, etc.)
        txtFile.normalizePunctuation();

        // Reset stringstream position to the beginning
        txtFile.getData()->clear();
        txtFile.getData()->seekg(0, std::ios::beg);

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

        std::string currentWord = "";
        std::string::iterator it;
        
        // Get the entire content as a string
        std::string content = txtFile.getData()->str();

        // Read file content character by character
        char c;
        while(txtFile.getData()->get(c)){

            // Check if the selected character is a space or newline
            if(c == ' ' || c == '\n'){

                // Attempt to map the current word to binary
                try {
                    // Append the space/newline char to the end of the mapping
                    std::vector<bool> tmp = table.mapStrToBin(currentWord+std::string(1,c));

                    // Insert the binary to the final boolean vector
                    finalBinary.insert(finalBinary.end(), tmp.begin(), tmp.end());
                    
                } catch (std::exception &e){
                    ctxt(std::string("\nError mapping word to binary: ") + e.what(), red, false, false, true);
                    return 1;
                }

                // Clear the word
                currentWord.clear();
            }
            else {
                
                // Append the character to the built string
                currentWord+=c;
            }
        }

        // handle the last word
        if(!currentWord.empty()){
            // Attempt to map the final word to binary
            try {
                std::vector<bool> tmp = table.mapStrToBin(currentWord);

                // Insert the binary to the final boolean vector
                finalBinary.insert(finalBinary.end(), tmp.begin(), tmp.end());
                
            } catch (std::exception &e){
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

        // Calculate the percent reduction
        double reduction = Utils::genPercentReduction(&txtFile, &binOut);

        ctxt(std::string("Compression reduced file size by ") + std::to_string(reduction) + "%.\n", magenta, false, false, true);

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


    ASCII TEXT ENCODING
    - https://codeshack.io/text-to-ascii-converter/
    - https://dencode.com/en/string/unicode-escape
    - https://www.fileformat.info/info/unicode/char/201c/index.htm
    - https://www.fileformat.info/info/unicode/char/201d/index.htm
    - https://www.fileformat.info/info/unicode/char/2018/index.htm
    - https://www.fileformat.info/info/unicode/char/2019/index.htm
    - https://www.fileformat.info/info/unicode/char/2039/index.htm
    - https://www.fileformat.info/info/unicode/char/203a/index.htm
    - https://www.fileformat.info/info/unicode/char/00e9/index.htm
    - https://www.fileformat.info/info/unicode/char/2014/index.htm
    - https://www.fileformat.info/info/unicode/char/00a0/index.htm
    - https://www.fileformat.info/info/unicode/char/200b/index.htm
    - https://www.fileformat.info/info/unicode/char/2026/index.htm
    - https://www.man7.org/linux//man-pages/man7/cp1252.7.html


    MISC.
    - https://www.geeksforgeeks.org/cpp/how-to-append-a-vector-to-a-vector-in-cpp/

*/