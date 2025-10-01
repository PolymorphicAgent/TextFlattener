#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "File.h"

/// @brief This class represents a dynamic compression table (aka dictionary) that will provide conversion
/// @brief functions to and from shortened binary representations.
class CompressionTable {
public:
    enum Mode {Compress, Decompress};

    explicit CompressionTable(File& file, Mode mode);

    File* getFile() const { return m_file; }
    Mode getMode() const { return m_mode; }

    std::vector<bool>* getHeader() const { return m_header; }
    void setHeader(std::vector<bool>&);
    bool genHeader();

    std::string mapBinToStr(const std::vector<bool>& bin) const;
    std::vector<bool> mapStrToBin(const std::string& str) const;

private:
    File* m_file; // Always Read-Only
    Mode m_mode; // Always Read-Only
    std::vector<bool> *m_header; // Read-Only in Compress Mode, Read-Write in Decompress Mode
    std::unordered_map<std::vector<bool>, std::string> *m_map_binStr; // Initialized in Decompress Mode ONLY
    std::unordered_map<std::string, std::vector<bool>> *m_map_strBin; // Initialized in Compress Mode ONLY
};