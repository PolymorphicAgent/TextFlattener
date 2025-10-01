#include "Utils.h"

#include <iostream>
#include <algorithm>

std::string Utils::extractFileName(std::string path){
    
    // simple cross-platform filename extraction
    auto pos = path.find_last_of("/\\");
    if (pos == std::string::npos) return path;
    return path.substr(pos + 1);
}

void Utils::testFileIO(){
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Testing TextFile Writing" << std::endl;
    std::cout << "-------------------------------" << std::endl;
    // Create a TextFile object
    TextFile txtFile("C:/Users/Peter/Documents/ECS 101/Project 1/sample.txt");

    // set some data
    *txtFile.getData() << "Hello, World!\nThis is a test stringstream.";
    std::cout << "Wrote to TextFile's stringstream data:" << std::endl;
    txtFile.write();
    std::cout << txtFile.getData()->str() << std::endl;

    std::cout << "-------------------------------" << std::endl;
    std::cout << "Testing TextFile Reading" << std::endl;

    // read it back
    std::cout << "TextFile path: " << txtFile.getPath() << std::endl;
    std::cout << "Extracted filename: " << Utils::extractFileName(txtFile.getPath()) << std::endl;
    txtFile.read();
    std::cout << "TextFile content:\n" << txtFile.getData()->str() << std::endl;

    std::cout << "-------------------------------" << std::endl;
    std::cout << "Testing BinaryFile Writing" << std::endl;

    // Create a BinaryFile object
    BinaryFile binFile("C:/Users/Peter/Documents/ECS 101/Project 1/sample.bin");

    // set some data
    std::vector<bool> sampleData = {1,0,1,0,1,0,1,0};
    binFile.setData(&sampleData);
    std::cout << "Wrote to BinaryFile's vector<bool> data:" << std::endl;
    binFile.write();
    for (bool bit : sampleData) {
        std::cout << bit;
    }

    std::cout << "\n-------------------------------" << std::endl;
    std::cout << "Testing BinaryFile Reading" << std::endl;

    // read it back
    std::cout << "BinaryFile path: " << binFile.getPath() << std::endl;
    std::cout << "Extracted filename: " << Utils::extractFileName(binFile.getPath()) << std::endl;
    binFile.read();
    std::cout << "BinaryFile content (as bits): ";
    for (bool bit : *(binFile.getData())) {
        std::cout << bit;
    }

    std::cout << "\n-------------------------------" << std::endl;

    std::cout << "Testing CSVFile Writing" << std::endl;
    // Create a CSVFile object
    CSVFile csvFile("C:/Users/Peter/Documents/ECS 101/Project 1/sample.csv");
    // set some data
    std::vector<std::vector<std::string>> sampleCSVData = {
        {"Name", "Age", "City"},
        {"Alice", "30", "New York"},
        {"Bob", "25", "Los Angeles"},
        {"Charlie", "35", "Chicago"}
    };

    csvFile.setData(&sampleCSVData);
    std::cout << "Wrote to CSVFile's vector<vector<string>> data:" << std::endl;
    csvFile.write();
    for (const auto& row : sampleCSVData) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Testing CSVFile Reading" << std::endl;
    // read it back
    std::cout << "CSVFile path: " << csvFile.getPath() << std::endl;
    std::cout << "Extracted filename: " << Utils::extractFileName(csvFile.getPath()) << std::endl;
    csvFile.read();
    std::cout << "CSVFile content:\n";
    for (const auto& row : *(csvFile.getData())) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-------------------------------" << std::endl;
}

void Utils::testCharFreqs(){
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Testing Character Frequency Generation" << std::endl;
    TextFile txtFile("C:/Users/Peter/Documents/ECS 101/Project 1/sample.txt");
    txtFile.read();
    auto freqs = Utils::genCharFreqs(&txtFile);
    std::cout << "Character Frequencies (as percent of total characters):" << std::endl;
    for(const auto& pair : *freqs){
        char tmp[] = {pair.first};
        std::string o(tmp);
        if(pair.first == '\n') o = "<newline>";
        else if(pair.first == ' ') o = "<space>";
        std::cout << "'" << o << "': " << pair.second << "%" << std::endl;
    }
    delete freqs; // Clean up allocated memory
    std::cout << "-------------------------------" << std::endl;
}

/// @brief Generates the relative character frequencies for the given text file.
/// @param file The text file to process.
/// @return A map of characters to their percent frequency / 100
std::vector<std::pair<char, double>>* Utils::genCharFreqs(TextFile* file){
    // Check if the file has been read already
    std::stringstream* fileData = file->getData();
    if(!fileData){
        file->read();
        fileData = file->getData();
    }

    // If there's still no data, throw an error
    if(!fileData){
        throw std::runtime_error("Invalid attempt to calculate character frequencies on an empty file!");
    }

    // Initialize and define our result map
    std::vector<std::pair<char, double>> *result = new std::vector<std::pair<char, double>>();
    
    // Count the occurrence of each character and keep track of total characters
    unsigned int totalChars = 0;
    char extracted;
    while(fileData->get(extracted)){
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

    return result;

}