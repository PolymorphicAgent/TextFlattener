#include<iostream>

#include "File.h"
#include "Utils.h"
#include "CompressionTable.h"

#include <fstream>

int main(int argc, char *argv[]){
    
    // Tests all the file IO functions provided in abstract class File's children
    Utils::testFileIO();

    // Tests the function that generates relative character frequencies given a file
    Utils::testCharFreqs();

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