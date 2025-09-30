#pragma once

#include <string>

/// @brief Static Utility Functions (Don't create an object!)
class Utils {
    public:
        static std::string extractFileName(std::string path);
        static void testFileIO();
};