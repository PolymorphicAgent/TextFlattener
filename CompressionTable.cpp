#include "CompressionTable.h"

CompressionTable::CompressionTable(File& file, Mode mode) : 
    m_file(&file), m_mode(mode), m_map_binStr(nullptr), 
    m_map_strBin(nullptr), m_header(nullptr)
{
    if(mode == Compress)
        m_map_strBin = new std::unordered_map<std::string, std::vector<bool>>();
    else
        m_map_binStr = new std::unordered_map<std::vector<bool>, std::string>();
}

void CompressionTable::setHeader(std::vector<bool>& header){
    if(m_mode == Compress)
        throw std::runtime_error("Invalid attempt to set header in compress mode!");
    else {
        delete m_header;
        m_header = &header;
    }
    return;
}

bool CompressionTable::genHeader(){//TODO: implement
    // Assume mappings have been generated
    return true;
}

std::string CompressionTable::mapBinToStr(const std::vector<bool>& bin) const {
    if(m_mode == Compress)
        throw std::runtime_error("Invalid attempt to map binary to string in compress mode!");

    auto it = m_map_binStr->find(bin);
    if(it != m_map_binStr->end())
        return it->second;
    else //TODO: This failure case is for easier debugging. In the future, we should map unknown characters to '#'
        throw std::runtime_error("Binary sequence not found in compression table!");
}

std::vector<bool> CompressionTable::mapStrToBin(const std::string& str) const {
    if(m_mode == Decompress)
        throw std::runtime_error("Invalid attempt to map string to binary in decompress mode!");

    auto it = m_map_strBin->find(str);
    if(it != m_map_strBin->end())
        return it->second;
    else //TODO: This failure case is for easier debugging. In the future, we should map unknown characters to a special binary sequence
        throw std::runtime_error("String not found in compression table!");
}