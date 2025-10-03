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

        /// @brief Extracts the file extension from a given path.
        /// @param path The full file path.
        /// @return The extracted file extension, or an empty string if none exists.
        /// @note The returned extension does not include the leading dot.
        static std::string extractFileExtension(std::string path);

        /// @brief Removes the file extension from a given filename.
        /// @param filename The filename from which to remove the extension.
        /// @return The filename without its extension.
        /// @note If the filename has no extension, it is returned unchanged.
        static std::string removeFileExtension(std::string filename);

        /// @brief Generates the relative character frequencies for the given text file.
        /// @param file The text file to process.
        /// @return A map of characters to their percent frequency / 100.
        /// @note The caller is responsible for deleting the returned map.
        static std::vector<std::pair<char, double>>* genCharFreqs(TextFile*);


        /// @brief Generates the relative word frequencies for the given text file.
        /// @param file The text file to process.
        /// @return A map of words to their percent frequency / 100.
        /// @note The caller is responsible for deleting the returned map.
        static std::vector<std::pair<std::string, double>>* genWordFreqs(TextFile*);

        // ***************** PRINTING UTILITIES *****************

        /// @brief Prints the usage information for the program.
        /// @note This function writes to cout.
        /// @param argv The command line arguments.
        static void printUsage(char* argv[]);

        /// @brief Prints detailed help information for the program.
        /// @note This function writes to cout.
        /// @param argv The command line arguments.
        static void printHelp(char* argv[]);

};