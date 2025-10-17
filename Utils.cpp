#include "Utils.h"
#include "Ctxt/ctxt.h"
#include "CompressionTable.h"

#include <iostream>
#include <algorithm>
#include <string>

std::string Utils::extractFileName(std::string path){
    // Find the last occurrence of '/' or '\'
    auto pos = path.find_last_of("/\\");

    // If no separator found, return the whole path
    if (pos == std::string::npos) return path;

    // Return the substring after the last separator
    return path.substr(pos + 1);
}

std::string Utils::extractFileExtension(std::string path){
    // Extract the filename from the path
    std::string filename = extractFileName(path);

    // Find the last occurrence of '.'
    auto pos = filename.find_last_of('.');

    // If no dot found or it's the first character, return an empty string
    if (pos == std::string::npos || pos == 0) return "";

    // Return the substring after the last dot
    return filename.substr(pos + 1);
}

std::string Utils::removeFileExtension(std::string filename){
    // Find the last occurrence of '.'
    auto pos = filename.find_last_of('.');

    // If no dot found or it's the first character, return the original filename
    if (pos == std::string::npos || pos == 0) return filename;

    // Return the substring before the last dot
    return filename.substr(0, pos);
}

void Utils::testFileIO(){
    ctxt("-------------------------------", red, false, false, true);
    ctxt("Testing TextFile Writing", cyan, true, false, true);
    ctxt("-------------------------------", red, false, false, true);

    // Create a TextFile object
    TextFile txtFile("./sample/sample.txt");

    // Write some data to its stringstream
    *txtFile.getData() << "Hello, World!\nThis is a test stringstream.";

    ctxt("Wrote to TextFile's stringstream data:", green, false, false, true);

    // Write out the file
    txtFile.write();

    // Display the content written
    ctxt(txtFile.getData()->str(), yellow, false, false, true);

    ctxt("-------------------------------", red, false, false, true);
    ctxt("Testing TextFile Reading", cyan, true, false, true);
    ctxt(std::string("TextFile path: ") + txtFile.getPath(), green, false, false, true);
    ctxt(std::string("Extracted filename: ") + Utils::extractFileName(txtFile.getPath()), green, false, false, true);

    // Read the file
    txtFile.read();

    // Display the content read
    ctxt(std::string("TextFile content:\n") + txtFile.getData()->str(), yellow, false, false, true);
    ctxt("-------------------------------", red, false, false, true);
    ctxt("Testing BinaryFile Writing", cyan, true, false, true);

    // Create a BinaryFile object
    BinaryFile binFile("./sample/sample.bin");

    // Write some data to its vector<bool>
    std::vector<bool> sampleData = {1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,0,0,0,0};
    binFile.setData(&sampleData);

    ctxt("Wrote to BinaryFile's vector<bool> data:", green, false, false, true);

    // Write out the file
    binFile.write();

    // Display the content written
    {
        std::string bits;
        bits.reserve(sampleData.size());
        for (bool bit : sampleData) bits.push_back(bit ? '1' : '0');
        ctxt(bits, yellow, false, false, true);
    }

    ctxt("-------------------------------", red, false, false, true);
    ctxt("Testing BinaryFile Reading", cyan, true, false, true);
    ctxt(std::string("BinaryFile path: ") + binFile.getPath(), green, false, false, true);
    ctxt(std::string("Extracted filename: ") + Utils::extractFileName(binFile.getPath()), green, false, false, true);

    // Read the file
    binFile.read();

    // Display the content read
    {
        std::string bits;
        bits.reserve(binFile.getData()->size());
        for (bool bit : *(binFile.getData())) bits.push_back(bit ? '1' : '0');
        ctxt(std::string("BinaryFile content (as bits): ") + bits, yellow, false, false, true);
    }

    ctxt("-------------------------------", red, false, false, true);
    ctxt("Testing CSVFile Writing", cyan, true, false, true);

    // Create a CSVFile object
    CSVFile csvFile("./sample/sample.csv");

    // Write some data to its vector<vector<string>*>
    std::vector<std::vector<std::string>> sampleCSVData;
    sampleCSVData.push_back({"Name", "Age", "City"});
    sampleCSVData.push_back({"Alice", "30", "New York"});
    sampleCSVData.push_back({"Bob", "25", "Los Angeles"});
    sampleCSVData.push_back({"Charlie", "35", "Chicago"});
    csvFile.setData(&sampleCSVData);

    ctxt("Wrote to CSVFile's vector<vector<string>> data:", green, false, false, true);

    // Write out the file
    csvFile.write();

    // Display the content written
    for (const auto& row : sampleCSVData) {
        std::string line;
        for (const auto& cell : row) {
            line += cell + " ";
        }
        ctxt(line, yellow, false, false, true);
    }

    ctxt("-------------------------------", red, false, false, true);
    ctxt("Testing CSVFile Reading", cyan, true, false, true);
    ctxt(std::string("CSVFile path: ") + csvFile.getPath(), green, false, false, true);
    ctxt(std::string("Extracted filename: ") + Utils::extractFileName(csvFile.getPath()), green, false, false, true);

    // Read the file
    csvFile.read();

    // Display the content read
    for (const auto& row : *(csvFile.getData())) {
        std::string line;
        for (const auto& cell : row) {
            line += cell + " ";
        }
        ctxt(line, yellow, false, false, true);
    }

    ctxt("-------------------------------", red, false, false, true);
}

void Utils::testCharFreqs(){
    ctxt("-------------------------------", red, false, false, true);
    ctxt("Testing Character Frequency Generation (./sample/sample.txt)", cyan, true, false, true);

    // Create and read a TextFile
    TextFile txtFile("./sample/sample.txt");
    txtFile.read();

    // Generate character frequencies
    auto freqs = Utils::genCharFreqs(&txtFile);

    // Display the frequencies
    ctxt("Character Frequencies (as percent of total characters):", green, false, false, true);
    for(const auto& pair : *freqs){
        // For better readability, represent newline and space characters with special labels
        std::string o(1, pair.first);
        if(pair.first == '\n') o = "<newline>";
        else if(pair.first == ' ') o = "<space>";

        // Write to console in white
        ctxt(std::string("'") + o + "': " + std::to_string(pair.second) + "%", yellow, false, false, true);
    }

    // Clean up allocated memory
    delete freqs; 

    ctxt("-------------------------------", red, false, false, true);
}

void Utils::testWordFreqs(){
    ctxt("-------------------------------", red, false, false, true);
    ctxt("Testing Word Frequency Generation (./sample/sample.txt)", cyan, true, false, true);

    // Create and read a TextFile
    TextFile txtFile("./sample/sample.txt");
    txtFile.read();

    // Generate word frequencies
    auto freqs = Utils::genWordFreqs(&txtFile);

    // Display the frequencies
    ctxt("Word Frequencies (as percent of total words):", green, false, false, true);
    for(const auto& pair : *freqs){
        std::string wordStr = pair.first;
        if(wordStr.find(',') != std::string::npos){
            wordStr = "\"" + wordStr + "\""; // Escape commas in words
        }
        // Write to console in white
        ctxt(std::string("'") + wordStr + "': " + std::to_string(pair.second) + "%", yellow, false, false, true);
    }

    // Clean up allocated memory
    delete freqs; 

    ctxt("-------------------------------", red, false, false, true);
}

void Utils::testAccuracy(){
    ctxt("-------------------------------", red, false, false, true);
    ctxt("Testing Accuracy Generation (./sample/sample.txt vs. ./sample/sample_copy.txt)", cyan, true, false, true);

    // Create and read the original TextFile
    TextFile original("./sample/sample.txt");
    original.read();

    // Create and read the decompressed TextFile
    TextFile decompressed("./sample/sample_copy.txt");
    decompressed.read();

    // Generate accuracy
    double accuracy = Utils::genAccuracy(&original, &decompressed);

    // Display the accuracy
    ctxt("Accuracy of identical files: " + std::to_string(accuracy) + "%", green, false, false, true);

    ctxt("-------------------------------", red, false, false, true);
}

std::vector<std::pair<char, double>>* Utils::genCharFreqs(TextFile* file){

    // Check if the file has been read
    if(file->getData() == nullptr)
        file->read();

    // Check if reading was successful
    if(file->getData() == nullptr)
        throw std::runtime_error("Invalid attempt to calculate character frequencies on an empty file!");

    // Initialize and define our result map
    std::vector<std::pair<char, double>> *result = new std::vector<std::pair<char, double>>();
    
    // Count the occurrence of each character and keep track of total characters
    unsigned int totalChars = 0;
    char extracted;
    while(file->getData()->get(extracted)){

        // See if we've already encountered this character
        auto it = std::find_if(result->begin(), result->end(), [&extracted](const std::pair<char, double>& l)
            { return l.first == extracted; });

        // If the character isn't already in our list, append it
        if(it == result->end()){
            result->push_back(std::make_pair(extracted, 1.0));
        }

        // Otherwise, increment that occurrence
        else {
            it->second++;
        }

        // Increment total character count
        totalChars++;
    }

    // Calculate the percent makeup of each character
    for(auto & i : *result){
        i.second = (i.second * 100.0) / totalChars;
    }

    // Sort the vector by value
    std::sort(result->begin(), result->end(), 
        [](const std::pair<char, double>& l, const std::pair<char, double>& r)
    {
        return l.second > r.second;
    });

    // Done!
    return result;
}

std::vector<std::pair<std::string, double>>* Utils::genWordFreqs(TextFile* file){

    // Check if the file has been read
    if(file->getData() == nullptr)
        file->read();

    // Check if reading was successful
    if(file->getData() == nullptr)
        throw std::runtime_error("Invalid attempt to calculate word frequencies on an empty file!");

    // Initialize and define our result map
    std::vector<std::pair<std::string, double>> *result = new std::vector<std::pair<std::string, double>>();
    
    // Count the occurrence of each word and keep track of total words
    unsigned int totalWords = 0;
    std::string word;
    while(*file->getData() >> word){

        // make sure it's just the word, no punctuation except '
        word.erase(std::remove_if(word.begin(), word.end(), [](char i){ return ::ispunct(static_cast<unsigned char>(i)) && i != '\''; }), word.end());

        // deal with edge case: "that's", "that'd" strip after apostrophe if it's the last character
        if(word.length() > 2 && word[word.length() - 2] == '\''){

            // if(word == "wouldn't") word = "would";
            if(word.at(word.length()-1) == 's') word = word.substr(0, word.length() - 2);
        }

        // See if we've already encountered this word
        auto it = std::find_if(result->begin(), result->end(), [&word](const std::pair<std::string, double>& l)
            { return l.first == word; });

        // If the word isn't already in our list, append it
        if(it == result->end()){
            result->push_back(std::make_pair(word, 1.0));
        }

        // Otherwise, increment that occurrence
        else {
            it->second++;
        }

        // Increment total word count
        totalWords++;
    }

    // Calculate the percent makeup of each word
    for(auto & i : *result){
        i.second = (i.second * 100.0) / totalWords;
    }

    // Sort the vector by value
    std::sort(result->begin(), result->end(), 
        [](const std::pair<std::string, double>& l, const std::pair<std::string, double>& r)
    {
        return l.second > r.second;
    });

    // Done!
    return result;
}

double Utils::genAccuracy(TextFile* original, TextFile* decompressed){

    // Check if the files have been read
    if(original->getData() == nullptr)
        original->read();
    if(decompressed->getData() == nullptr)
        decompressed->read();

    // Check if reading was successful
    if(original->getData() == nullptr || decompressed->getData() == nullptr)
        throw std::runtime_error("Invalid attempt to calculate accuracy on an empty file!");

    // Reset stringstream positions to the beginning
    original->getData()->clear();
    original->getData()->seekg(0, std::ios::beg);
    decompressed->getData()->clear();
    decompressed->getData()->seekg(0, std::ios::beg);

    // Compare character by character
    unsigned int totalChars = 0;
    unsigned int matchingChars = 0;
    char origChar, decompChar;
    while(original->getData()->get(origChar) && decompressed->getData()->get(decompChar)){
        totalChars++;
        if(decompChar == '#' || origChar == decompChar){
            matchingChars++;
        }
    }

    // If files are of different lengths, account for that in totalChars
    while(original->getData()->get(origChar)){
        totalChars++;
    }
    while(decompressed->getData()->get(decompChar)){
        totalChars++;
    }

    // Calculate accuracy as a percentage
    if(totalChars == 0) return 100.0; // Both files are empty, consider them perfectly accurate
    return (static_cast<double>(matchingChars) / static_cast<double>(totalChars)) * 100.0;
}

double Utils::genPercentReduction(TextFile* original, BinaryFile* compressed){

    // Check if the files have been read
    if(original->getData() == nullptr)
        original->read();
    if(compressed->getData() == nullptr)
        compressed->read();

    // Check if reading was successful
    if(original->getData() == nullptr || compressed->getData() == nullptr)
        throw std::runtime_error("Invalid attempt to calculate percent reduction on an empty file(s)!");

    // Reset stringstream position to the beginning
    original->getData()->clear();
    original->getData()->seekg(0, std::ios::beg);

    // Calculate number of original characters
    unsigned int numOriginalChars = 0;
    char originalChar;
    while(original->getData()->get(originalChar)){
        numOriginalChars++;
    }

    // Get the number of bits in compressed file
    unsigned int numCompressedBits = compressed->getData()->size();

    // Use the provided formula to calculate percent reduction
    // 100*(8*(numOriginalChars - numCompressedBits)/(8*numOriginalChars))
    if(numOriginalChars == 0) return 0.0; // Avoid division by zero, no reduction possible on empty original
    return 100.0 * (8.0 * (static_cast<double>(numOriginalChars) - static_cast<double>(numCompressedBits)) / (8.0 * static_cast<double>(numOriginalChars)));

}

// ****************** PRINTING UTILITIES *****************

void Utils::printUsage(char* argv[]){
    // Print usage in red to indicate an error/incorrect invocation
    ctxt("\nUsage: \n\n", red, false, false, false);
    ctxt(std::string(argv[0]) + " <mode> <input file(s)>\n", green, false, false, true);
    ctxt("  <mode>: \n\n    c ---- Compression mode\n    d ---- Decompression mode\n    gc --- Generate character frequencies mode\n    gw --- Generate word frequencies mode\n    help - Display more detailed information about modes\n    test - Execute test functions\n    acc -- Compare two text files' accuracy\n", yellow, false, false, true);
}

void Utils::printHelp(char* argv[]){
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

    ctxt("  Accuracy Comparison Mode (acc):\n", dark_green, false, false, false);
    ctxt("    Compares two text files and calculates the accuracy of the decompressed file against the original.\n", blue, false, false, false);
    ctxt(std::string("    Example: ") + argv[0] + " acc original.txt decompressed.txt\n", magenta, false, false, false);
    ctxt("      Output:\n", magenta, false, false, false);
    ctxt("        - Displays the accuracy percentage in the console.\n\n", yellow, false, false, true);

    ctxt("Note:\n", dark_red, true, false, false);
    ctxt("  - Make sure that the input files exist and are accessible.\n", yellow, false, false, false);
    ctxt("  - The program will generate output files in the same directory as the input file.\n", yellow, false, false, false);
    ctxt("  - The compression and decompression modes rely on a predefined compression table file 'table.csv'.\n", yellow, false, false, true);

}