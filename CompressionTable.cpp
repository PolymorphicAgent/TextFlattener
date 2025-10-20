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

            // Insert into the map while making sure spaces and newlines are handled correctly
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

            // Insert into the map while accounting for space and newline
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

    // If not found return a "#"
    else return "#";    
}

std::vector<bool> CompressionTable::mapStrToBin(const std::string& str) const {

    // Make sure we are in Compress mode
    if (m_mode == Decompress)
        throw std::runtime_error("Invalid attempt to map string to binary in decompress mode!");

    // Verify that an empty string was not passed
    if (str.empty())
        throw std::runtime_error("Invalid attempt to map empty string!");

    // Look up the string in the map (exact match)
    auto it = m_map_strBin->find(str);
    
    // If found return the corresponding binary sequence
    if (it != m_map_strBin->end())
        return it->second;

    // If not found
    if (str.length() > 1) {

        // First attempt to find any multi-character table entry that is a substring of str_
        for (const auto& pair : *m_map_strBin) {

            // Take the string from the Compression Table
            const std::string& tableStr = pair.first;

            // Only consider multicharacter table entries here
            if (tableStr.length() <= 1)
                continue;

            // Attempt to find this table entry in the query string
            size_t pos = str.find(tableStr);
            if (pos == std::string::npos)
                continue; // try next table entry

            // Found a matching substring so build a combined boolean vector consisting of binary(before) + binary(matched) + binary(after)
            std::vector<bool> result;

            // Process the part before the matched substring (if any)
            if (pos > 0) {

                // Take a substring of the preceding characters
                std::string before = str.substr(0, pos);

                // Iterate through the string char by char
                for (char c : before) {

                    // Convert the character to a string
                    std::string charStr(1, c);

                    // Find the character in our map
                    auto charIt = m_map_strBin->find(charStr);

                    if (charIt != m_map_strBin->end()) {

                        // If the character is found, append it directly to our binary
                        const std::vector<bool>& charBin = charIt->second;
                        result.insert(result.end(), charBin.begin(), charBin.end());
                    } else {

                        // Otherwise, append '#' binary
                        auto hashIt = m_map_strBin->find("#");

                        if (hashIt != m_map_strBin->end()) {
                            const std::vector<bool>& hashBin = hashIt->second;
                            result.insert(result.end(), hashBin.begin(), hashBin.end());
                        } else {
                            throw std::runtime_error("'#' not found in the compression table " + m_csv->getPath());
                        }
                    }
                }
            }

            // Add the matched substring's binary representation (shorthand)
            const std::vector<bool>& matchedBin = pair.second;
            result.insert(result.end(), matchedBin.begin(), matchedBin.end());

            // Process the part after the matched substring (if any)
            size_t endPos = pos + tableStr.length();
            if (endPos < str.length()) {

                // Take a substring after the shorthand word
                std::string after = str.substr(endPos);

                // Iterate through the string char by char
                for (char c : after) {

                    // Convert the char to string
                    std::string charStr(1, c);

                    // Find the char in our map
                    auto charIt = m_map_strBin->find(charStr);

                    if (charIt != m_map_strBin->end()) {

                        // If the char was found, append it's binary
                        const std::vector<bool>& charBin = charIt->second;
                        result.insert(result.end(), charBin.begin(), charBin.end());
                    } else {

                        // Otherwise, append '#' binary
                        auto hashIt = m_map_strBin->find("#");

                        if (hashIt != m_map_strBin->end()) {
                            const std::vector<bool>& hashBin = hashIt->second;
                            result.insert(result.end(), hashBin.begin(), hashBin.end());
                        } else {
                            throw std::runtime_error("'#' not found in the compression table " + m_csv->getPath());
                        }
                    }
                }
            }

            return result; // A composite result using our table's shorthands (if present)
        }

        // No multi-character shorthand found so fall back to building the token char by char
        std::vector<bool> result;
        for (size_t i = 0; i < str.size(); ++i) {

            // Take the character at the current index
            char c = str[i];

            // Convert the character to a string
            std::string charStr(1, c);

            // Find the character in our map
            auto charIt = m_map_strBin->find(charStr);

            if (charIt != m_map_strBin->end()) {

                // If found, append the binary representation
                const std::vector<bool>& charBin = charIt->second;
                result.insert(result.end(), charBin.begin(), charBin.end());
            } else {

                // Otherwise, fallback to '#'
                auto poundIt = m_map_strBin->find("#");

                // If # was found in our table
                if (poundIt != m_map_strBin->end()) {

                    // Append the '#' binary and continue
                    result.insert(result.end(), poundIt->second.begin(), poundIt->second.end());
                    continue;
                }
                else
                    // If not found, throw an error
                    throw std::runtime_error("'#' not found in the compression table "+m_csv->getPath());

                // Character not found and no fallback (Control should NEVER reach here)
                throw std::runtime_error(std::string("Character '") + charStr + "' not found in the compression table " + m_csv->getPath());
            }
        }

        return result;
    }

    // If our word is of length 1
    else if (str.length() == 1) {

        // Convert the selected character to a string
        std::string charStr(1, str[0]);

        // Attempt to find the character in our table
        auto charIt = m_map_strBin->find(charStr);

        // Return it if found
        if (charIt != m_map_strBin->end())
            return charIt->second;

        // Fallback to '#' for unknown single characters
        auto poundIt = m_map_strBin->find("#");

        // Return # if found
        if (poundIt != m_map_strBin->end())
            return poundIt->second;
        else
            // If not found, throw an error (should not happen)
            throw std::runtime_error("'#' not found in the compression table "+m_csv->getPath());
        
        // Control should NEVER reach this statement
        throw std::runtime_error(std::string("Character '") + charStr + "' not found in the compression table " + m_csv->getPath());
    }
    else {

        // Something has gone VERY wrong if control reaches here!
        throw std::runtime_error("Invalid string length in mapStrToBin!");
    }
}
