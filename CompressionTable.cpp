#include "CompressionTable.h"

CompressionTable::CompressionTable(CSVFile& csv, Mode mode) : 
    m_csv(&csv), m_mode(mode), m_map_binStr(nullptr), 
    m_map_strBin(nullptr)
{
    // Check if we are in Compress or Decompress mode
    if(mode == Compress){
        // Initialize the string to binary map
        m_map_strBin = new std::unordered_map<std::string, std::vector<bool>>();

        // Check if m_csv has been read
        if(m_csv->getData() == nullptr)
            m_csv->read();
    
        // Check if reading was successful
        if(m_csv->getData() == nullptr)
            throw std::runtime_error("Failed to read CSV file in CompressionTable constructor!");

        // Populate the string to binary map
        for(const auto& row : *(m_csv->getData())){

            // Each row should have exactly 2 columns
            if(row.size() != 2)
                throw std::runtime_error("Invalid row in CSV file in CompressionTable constructor! (expected 2 columns, got " + std::to_string(row.size()) + ")");

            // The first column is the string, the second column is the binary sequence (as a string of '0's and '1's)
            const std::string& str = row[0];
            const std::string& binStr = row[1];

            // Convert the binary string to a vector<bool>
            std::vector<bool> bin;
            for(char c : binStr){
                if(c == '0')
                    bin.push_back(false);
                else if(c == '1')
                    bin.push_back(true);
                else
                    throw std::runtime_error("Invalid character in binary string in CSV file in CompressionTable constructor! (expected '0' or '1', got '" + std::string(1, c) + "')");
            }

            // Insert into the map
            m_map_strBin->emplace(str, bin);
        }
    }
    // We are in Decompress mode
    else{
        // Initialize the binary to string map
        m_map_binStr = new std::unordered_map<std::vector<bool>, std::string>();

        // Check if m_csv has been read
        if(m_csv->getData() == nullptr)
            m_csv->read();
        
        // Check if reading was successful
        if(m_csv->getData() == nullptr)
            throw std::runtime_error("Failed to read CSV file in CompressionTable constructor!");

        // Populate the binary to string map
        for(const auto& row : *(m_csv->getData())){

            // Each row should have exactly 2 columns
            if(row.size() != 2)
                throw std::runtime_error("Invalid row in CSV file in CompressionTable constructor! (expected 2 columns, got " + std::to_string(row.size()) + ")");

            // The first column is the string, the second column is the binary sequence (as a string of '0's and '1's)
            const std::string& str = row[0];
            const std::string& binStr = row[1];

            // Convert the binary string to a vector<bool>
            std::vector<bool> bin;
            for(char c : binStr){
                if(c == '0')
                    bin.push_back(false);
                else if(c == '1')
                    bin.push_back(true);
                else
                    throw std::runtime_error("Invalid character in binary string in CSV file in CompressionTable constructor! (expected '0' or '1', got '" + std::string(1, c) + "')");
            }

            // Insert into the map
            m_map_binStr->emplace(bin, str);
        }
    }
}

std::string CompressionTable::mapBinToStr(const std::vector<bool>& bin) const {
    // Make sure we are in Decompress mode
    if(m_mode == Compress)
        throw std::runtime_error("Invalid attempt to map binary to string in compress mode!");

    // Look up the binary sequence in the map
    auto it = m_map_binStr->find(bin);

    // If found, return the corresponding string
    if(it != m_map_binStr->end())
        return it->second;

    // If not found, throw an error
    else {

        // For easier debugging, we can print the binary sequence that was not found
        std::string binStr;
        for(bool b : bin)
            binStr += (b ? '1' : '0');

        throw std::runtime_error("Binary sequence not found in compression table! (length: " + std::to_string(bin.size()) + ", sequence: " + binStr + ")");
    }
    
}

std::vector<bool> CompressionTable::mapStrToBin(const std::string& str) const {
    // Make sure we are in Compress mode
    if(m_mode == Decompress)
        throw std::runtime_error("Invalid attempt to map string to binary in decompress mode!");

    // Look up the string in the map
    auto it = m_map_strBin->find(str);

    // If found, return the corresponding binary sequence
    if(it != m_map_strBin->end())
        return it->second;

    // If not found, throw an error
    else
        throw std::runtime_error("String not found in compression table! (string: " + str + ")");
}