#include<iostream>

#include "File.h"
#include "Utils.h"
#include "CompressionTable.h"

#include <fstream>

int main(int argc, char *argv[]){
    
    Utils::testFileIO();

    Utils::testCharFreqs();

    return 0;
      
}

/*
    References:

    - https://en.cppreference.com/w/cpp/container/unordered_map.html
    - https://stackoverflow.com/questions/19197799/what-is-the-quickest-way-of-inserting-updating-stdunordered-map-elements-witho
    - https://stackoverflow.com/questions/5056645/sorting-stdmap-using-value
    - https://stackoverflow.com/questions/42933943/how-to-use-lambda-for-stdfind-if

*/