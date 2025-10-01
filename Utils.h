#pragma once

#include "File.h"

#include <string>
#include <sstream>
#include <unordered_map>

/// @brief Static Utility Functions (Don't create an object!)
class Utils {
    public:
        /// @brief Test all File I/O functionality.
        /// @note This function writes to cout.
        static void testFileIO();

        /// @brief Test character frequency generation functionality.
        /// @note This function writes to cout.
        static void testCharFreqs();
        
        /// @brief Extracts the filename from a given path.
        /// @param path The full file path.
        /// @return The extracted filename.
        static std::string extractFileName(std::string path);

        /// @brief Generates the relative character frequencies for the given text file.
        /// @param file The text file to process.
        /// @return A map of characters to their percent frequency / 100.
        /// @note The caller is responsible for deleting the returned map.
        static std::vector<std::pair<char, double>>* genCharFreqs(TextFile*);

};