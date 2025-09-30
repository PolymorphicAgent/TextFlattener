#include<iostream>

#include "File.h"
#include "Utils.h"
#include "CompressionTable.h"

#include <fstream>

int main(int argc, char *argv[]){
    
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

    return 0;
      
}