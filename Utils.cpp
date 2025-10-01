#include "Utils.h"

#include <iostream>
#include <algorithm>

std::string Utils::extractFileName(std::string path){
    // Find the last occurrence of '/' or '\'
    auto pos = path.find_last_of("/\\");

    // If no separator found, return the whole path
    if (pos == std::string::npos) return path;

    // Return the substring after the last separator
    return path.substr(pos + 1);
}

void Utils::testFileIO(){
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Testing TextFile Writing" << std::endl;
    std::cout << "-------------------------------" << std::endl;

    // Create a TextFile object
    TextFile txtFile("C:/Users/Peter/Documents/ECS 101/Project 1/sample.txt");

    // Write some data to its stringstream
    *txtFile.getData() << "Hello, World!\nThis is a test stringstream.";

    std::cout << "Wrote to TextFile's stringstream data:" << std::endl;

    // Write out the file
    txtFile.write();

    // Display the content written
    std::cout << txtFile.getData()->str() << std::endl;

    std::cout << "-------------------------------" << std::endl;
    std::cout << "Testing TextFile Reading" << std::endl;
    std::cout << "TextFile path: " << txtFile.getPath() << std::endl;
    std::cout << "Extracted filename: " << Utils::extractFileName(txtFile.getPath()) << std::endl;

    // Read the file
    txtFile.read();

    // Display the content read
    std::cout << "TextFile content:\n" << txtFile.getData()->str() << std::endl;
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Testing BinaryFile Writing" << std::endl;

    // Create a BinaryFile object
    BinaryFile binFile("C:/Users/Peter/Documents/ECS 101/Project 1/sample.bin");

    // Write some data to its vector<bool>
    std::vector<bool> sampleData = {1,0,1,0,1,0,1,0};
    binFile.setData(&sampleData);

    std::cout << "Wrote to BinaryFile's vector<bool> data:" << std::endl;

    // Write out the file
    binFile.write();

    // Display the content written
    for (bool bit : sampleData) {
        std::cout << bit;
    }

    std::cout << "\n-------------------------------" << std::endl;
    std::cout << "Testing BinaryFile Reading" << std::endl;
    std::cout << "BinaryFile path: " << binFile.getPath() << std::endl;
    std::cout << "Extracted filename: " << Utils::extractFileName(binFile.getPath()) << std::endl;

    // Read the file
    binFile.read();

    // Display the content read
    std::cout << "BinaryFile content (as bits): ";
    for (bool bit : *(binFile.getData())) {
        std::cout << bit;
    }

    std::cout << "\n-------------------------------" << std::endl;
    std::cout << "Testing CSVFile Writing" << std::endl;

    // Create a CSVFile object
    CSVFile csvFile("C:/Users/Peter/Documents/ECS 101/Project 1/sample.csv");

    // Write some data to its vector<vector<string>*>
    std::vector<std::vector<std::string>> sampleCSVData;
    sampleCSVData.push_back({"Name", "Age", "City"});
    sampleCSVData.push_back({"Alice", "30", "New York"});
    sampleCSVData.push_back({"Bob", "25", "Los Angeles"});
    sampleCSVData.push_back({"Charlie", "35", "Chicago"});
    csvFile.setData(&sampleCSVData);

    std::cout << "Wrote to CSVFile's vector<vector<string>> data:" << std::endl;

    // Write out the file
    csvFile.write();

    // Display the content written
    for (const auto& row : sampleCSVData) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "-------------------------------" << std::endl;
    std::cout << "Testing CSVFile Reading" << std::endl;
    std::cout << "CSVFile path: " << csvFile.getPath() << std::endl;
    std::cout << "Extracted filename: " << Utils::extractFileName(csvFile.getPath()) << std::endl;

    // Read the file
    csvFile.read();

    // Display the content read
    std::cout << "CSVFile content:\n";
    for (const auto& row : *(csvFile.getData())) {
        for (const auto cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "-------------------------------" << std::endl;
}

void Utils::testCharFreqs(){
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Testing Character Frequency Generation" << std::endl;

    // Create and read a TextFile
    TextFile txtFile("C:/Users/Peter/Documents/ECS 101/Project 1/sample.txt");
    txtFile.read();

    // Generate character frequencies
    auto freqs = Utils::genCharFreqs(&txtFile);

    // Display the frequencies
    std::cout << "Character Frequencies (as percent of total characters):" << std::endl;
    for(const auto& pair : *freqs){

        // For better readability, represent newline and space characters with special labels
        char tmp[] = {pair.first, '\0'};
        std::string o(tmp);
        if(pair.first == '\n') o = "<newline>";
        else if(pair.first == ' ') o = "<space>";

        // Write to cout
        std::cout << "'" << o << "': " << pair.second << "%" << std::endl;
    }

    // Clean up allocated memory
    delete freqs; 

    std::cout << "-------------------------------" << std::endl;
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