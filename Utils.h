#pragma once

#include "File.h"

#include <string>
#include <sstream>
#include <unordered_map>

/// @brief Static Utility Functions (Don't create an object!)
class Utils {
    public:
        static void testFileIO();
        static void testCharFreqs();
        
        static std::string extractFileName(std::string path);
        static std::vector<std::pair<char, double>>* genCharFreqs(TextFile*);

};