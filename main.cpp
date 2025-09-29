#include<iostream>

#include "File.h"
#include "Utils.h"

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

    return 0;
      
}