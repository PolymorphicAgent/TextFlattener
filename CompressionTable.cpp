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
        for(const auto& row : *m_csv->getData()){

            // Each row should have exactly 2 columns
            if(row.size() != 2)
                throw std::runtime_error("Invalid row in CSV file in CompressionTable constructor! (expected 2 columns, got " + std::to_string(row.size()) + ") \nRow data: "+[row](){std::string r="";for(auto& i:row)r+=i;return r;}());

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
            std::string insStr=str;
            if(str == "<space>")insStr = " ";
            if(str == "<newline>")insStr = "\n";
            m_map_strBin->emplace(insStr, bin);
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
            std::string insStr=str;
            if(str == "<space>")insStr = " ";
            if(str == "<newline>")insStr = "\n";
            m_map_binStr->emplace(bin, insStr);
        }
    }
}

std::string CompressionTable::mapBinToStr(const std::vector<bool>& bin) const {
    // Make sure we are in Decompress mode
    if(m_mode == Compress)
        throw std::runtime_error("Invalid attempt to map binary to string in compress mode!");

    if(bin.size() == 0)
        throw std::runtime_error("Invalid attempt to map empty binary vector!");

    // Look up the binary sequence in the map
    auto it = m_map_binStr->find(bin);

    // If found, return the corresponding string
    if(it != m_map_binStr->end())
        return it->second;

    // If not found
    else {

        // If unknown, return a "#"
        return "#";

        // For easier debugging, we can print the binary sequence that was not found
        // std::string binStr;
        // for(bool b : bin)
        //     binStr += (b ? '1' : '0');

        // throw std::runtime_error("Binary sequence not found in compression table! (length: " + std::to_string(bin.size()) + ", sequence: " + binStr + ")");
    }
    
}

std::vector<bool> CompressionTable::mapStrToBin(const std::string& str) const {
    std::string str_ = str;
    // Make sure we are in Compress mode
    if(m_mode == Decompress)
        throw std::runtime_error("Invalid attempt to map string to binary in decompress mode!");

    // Verify that an empty string was not passed
    if(str == "")
        throw std::runtime_error("Invalid attempt to map empty string!");

    // Handle tab as 4 spaces
    // else if(str == "\t")str_ = "    ";


    // Look up the string in the map
    auto it = m_map_strBin->find(str_);

    // If found, return the corresponding binary sequence
    if(it != m_map_strBin->end())
        return it->second;

    // If not found
    else {

        // If we are matching a word, return an empty vector to signify that there's no binary representation for that word
        if(str_.length() > 1) return std::vector<bool>();

        // If we are matching a character, attempt to map it to a '#'
        else if(str_.length() == 1) {

            // Lookup the binary for the '#' character and return it
            auto it1 = m_map_strBin->find("#");

            // If found, return the corresponding binary sequence
            if(it1 != m_map_strBin->end())
                return it1->second;
            
            // If not found, throw an error
            throw std::runtime_error("'#' not found in the compression table "+m_csv->getPath());
        }
        else {
            throw std::runtime_error("Invalid string length in mapStrToBin!");
        }
    }
}