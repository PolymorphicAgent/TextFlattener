#include "Utils.h"
#include "Ctxt/ctxt.h"

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

void Utils::testFileIO(){
    ctxt("-------------------------------", red, false, false, true);
    ctxt("Testing TextFile Writing", cyan, true, false, true);
    ctxt("-------------------------------", red, false, false, true);

    // Create a TextFile object
    TextFile txtFile("C:/Users/Peter/Documents/ECS 101/Project 1/sample.txt");

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
    BinaryFile binFile("C:/Users/Peter/Documents/ECS 101/Project 1/sample.bin");

    // Write some data to its vector<bool>
    std::vector<bool> sampleData = {1,0,1,0,1,0,1,0};
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
    CSVFile csvFile("C:/Users/Peter/Documents/ECS 101/Project 1/sample.csv");

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
    ctxt("Testing Character Frequency Generation (sample.txt)", cyan, true, false, true);

    // Create and read a TextFile
    TextFile txtFile("C:/Users/Peter/Documents/ECS 101/Project 1/sample.txt");
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