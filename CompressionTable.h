#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "File.h"

/// @brief This class represents a compression table (aka dictionary) that will provide conversion
/// @brief functions to and from shortened binary representations.
class CompressionTable {
public:
    /// @brief This enum allows us to easily name what mode the program was run in.
    enum Mode {Compress, Decompress};

    /// @brief This is the ONLY constructor that should be used!
    /// @param csv The CSV file to read the table from.
    /// @param mode Whether the program is in compress or decompress mode.
    explicit CompressionTable(CSVFile& csv, Mode mode);

    /// @brief Gets the current program mode.
    /// @return The current program mode.
    Mode getMode() const { return m_mode; }

    /// @brief Maps from binary sequences to strings.
    /// @brief This should only be used in Decompress mode.
    /// @param bin The binary sequence to map.
    /// @return The string mapped to the given binary sequence.
    std::string mapBinToStr(const std::vector<bool>& bin) const;

    /// @brief Maps from strings to binary sequences.
    /// @brief This should only be used in Compress mode.
    /// @param str The string to map.
    /// @return The binary sequence mapped to the given string.
    /// @note If str is a word, will attempt to find smaller words within it and return the built binary
    std::vector<bool> mapStrToBin(const std::string& str) const;

private:
    /// @brief This member stores the CSV file that contains the compression table.
    /// @note This file is always read-only.
    CSVFile *m_csv;

    /// @brief This member stores the current program mode.
    /// @note This member is always read-only.
    Mode m_mode;

    /// @brief This member stores the mappings from binary sequences to strings.
    /// @note This member is only initialized in Decompress mode.
    std::unordered_map<std::vector<bool>, std::string> *m_map_binStr; 
    
    /// @brief This member stores the mappings from strings to binary sequences.
    /// @note This member is only initialized in Compress mode.
    std::unordered_map<std::string, std::vector<bool>> *m_map_strBin;
};