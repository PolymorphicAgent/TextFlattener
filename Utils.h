#pragma once

// ---------------------- Project Includes ----------------------
#include "File.h"

// ---------------------- System Includes ----------------------
#include <string>
#include <sstream>
#include <unordered_map>

/// @brief Static Utility Functions (Don't create an object!)
class Utils {
    public:

        // ***************** TESTING UTILITIES *****************

        /// @brief Test all File I/O functionality.
        /// @note This function writes to cout.
        static void testFileIO();

        /// @brief Test character frequency generation functionality.
        /// @note This function writes to cout.
        static void testCharFreqs();

        /// @brief Test word frequency generation functionality.
        /// @note This function writes to cout.
        static void testWordFreqs();

        /// @brief Test accuracy generation functionality.
        /// @note This function writes to cout.
        static void testAccuracy();

        // ***************** FILE PATH UTILITIES *****************
        
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

        // ***************** OTHER UTILITIES *****************

        /// @brief Compares two vectors for equality.
        /// @tparam T The type of elements in the vectors.
        /// @param a The first vector.
        /// @param b The second vector.
        /// @return True if the vectors are equal, false otherwise.
        /// @note This function assumes that the '==' operator is defined for type T.
        template <typename T>
        static bool equals(const std::vector<T>& l, const std::vector<T>& r) {
            // Check if sizes match
            if(l.size() != r.size()) return false;

            // Compare elements
            for(size_t i = 0; i < l.size(); i++) {
                if(l[i] != r[i]) return false;
            }

            return true;
        }

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

        /// @brief Generates the accuracy of a decompressed file against the original file.
        /// @note This function assumes that both files are text files.
        /// @param original The original text file.
        /// @param decompressed The decompressed text file.
        /// @return The accuracy as a percentage (0.0 to 100.0).
        /// @note If the files are of different lengths, the accuracy will be less than 100.0.
        static double genAccuracy(TextFile*, TextFile*);

        /// @brief Generates the percent reduction in size from the original text file to the compressed binary file.
        /// @param original The original text file.
        /// @param compressed The compressed binary file.
        /// @return The percent reduction in size (0.0 to 100.0).
        /// @note If the compressed file is larger than the original, the percent reduction will be negative.
        /// @note This function assumes that the text file uses 1 byte per character.
        static double genPercentReduction(TextFile*, BinaryFile*);

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