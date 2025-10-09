#include "File.h"

#include <fstream>
#include <cstdint>

// ****************** TextFile Implementation ******************

// Constructor for TextFile
TextFile::TextFile(std::string path, std::stringstream* data) : m_data(data) {

    // If no data provided, create a new stringstream
    if(!data) m_data = new std::stringstream();
    
    // Set the file path
    setPath(path);
}

// Destructor for TextFile
TextFile::~TextFile() = default;

// Implements reading from a text file
void TextFile::read() {

    // Open the file for reading
    std::ifstream file;
    file.open(getPath(), std::ios::in);

    // If the file is open, read its contents into the stringstream
    if (file.is_open()) {
        m_data->clear(); // Reset the error state
        m_data->str(""); // Clear the string content

        // Read the file content into the stringstream
        *m_data << file.rdbuf();

        // Observe the niceties
        file.close();
    }

    // If the file could not be opened, throw an error
    else {
        throw std::runtime_error("Could not open text file for reading: " + getPath());
    }
}

// Implements writing to a text file
void TextFile::write() {

    // Open the file for writing
    std::ofstream file;
    file.open(getPath(), std::ios::out);

    // If the file is open, write the stringstream's contents to the file
    if (file.is_open()) {

        // Write the stringstream content to the file
        if (m_data) file << m_data->str();
        
        // Observe the niceties
        file.close();
    }
    
    // If the file could not be opened, throw an error
    else {
        throw std::runtime_error("Could not open text file for writing: " + getPath());
    }
}

// ****************** BinaryFile Implementation ******************

// Constructor for BinaryFile
BinaryFile::BinaryFile(std::string path, std::vector<bool>* data) : m_data(data) {

    // If no data provided, create a new vector<bool>
    if(!data) m_data = new std::vector<bool>();
        
    // Set the file path
    setPath(path);
}

// Destructor for BinaryFile
BinaryFile::~BinaryFile() = default;

// Implements reading from a binary file
void BinaryFile::read() {

    // Open the file for reading
    std::ifstream file;
    file.open(getPath(), std::ios::in | std::ios::binary);

    // If the file is open, read its contents into the vector<bool>
    if (file.is_open()) {

        // Read the bit count
        uint64_t bit_count;
        file.read(reinterpret_cast<char*>(&bit_count), sizeof(bit_count));

        // Clear existing data
        m_data->clear();
        
        // Reserving space avoids excessive reallocations when reading many bytes.
        m_data->reserve(bit_count);

        // Read the file content byte by byte
        char byte;
        size_t bits_read = 0;
        while (file.get(byte) && bits_read < bit_count) {

            // Convert each byte to its 8 bits (MSB first) and append to m_data.
            //
            // Notes:
            // - We cast `byte` to uint8_t to make sure that right-shifts are logical
            //   and not implementation-defined for a signed `char`.
            // - We append bits in big-endian order (most-significant bit first)
            //   so that the first pushed bit corresponds to bit 7 of the byte.
            uint8_t ubyte = static_cast<uint8_t>(byte);

            // Determine how many bits to read from this byte
            size_t bits_to_read = std::min(size_t(8), bit_count - bits_read);

            // Extract bits one at a time from bit 7 down (MSB first, since most modern machines are little-endian)
            for (size_t i = 0; i < bits_to_read; ++i) {

                // Start from bit 7, go down
                int bit_index = 7 - i;

                // Shift the byte right by `bit_index` places so that the desired
                // bit lands in position 0, then mask with 1 to isolate it.
                // The expression yields 0 or 1 which is implicitly convertible
                // to bool when pushed into std::vector<bool>.
                uint8_t bit = (ubyte >> bit_index) & 0x1u;
                m_data->push_back(static_cast<bool>(bit));
            }

            // Increment bits_read
            bits_read += bits_to_read;
        }

        // Observe the niceties
        file.close();
    }
    
    // If the file could not be opened, throw an error
    else {
        throw std::runtime_error("Could not open binary file for reading: " + getPath());
    }
}

// Implement writing to a binary file
void BinaryFile::write() {

    // Open the file for writing
    std::ofstream file;
    file.open(getPath(), std::ios::out | std::ios::binary);

    // If the file is open, write the vector<bool>'s contents to the file
    if (file.is_open()) {

        // Write the bit count first (as a 64-bit value) so we know how many
        // bits are valid when reading back
        uint64_t bit_count = m_data->size();
        file.write(reinterpret_cast<const char*>(&bit_count), sizeof(bit_count));

        // Pack bits from m_data into bytes and write them to the file.
        // We treat bits in m_data as MSB-first within each byte: the bit at
        // index `i` becomes bit 7 of the first output byte, index `i+1` becomes
        // bit 6, and so on. When the remaining bits are fewer than 8, we left-
        // align them so that they occupy the most-significant positions of the
        // final output byte (this mirrors the MSB-first read logic).
        for (size_t i = 0; i < m_data->size(); i += 8) {
            // Use an unsigned 8-bit accumulator to avoid signed-shift UB.
            uint8_t byte_acc = 0;

            // How many bits remain in the final (possibly partial) chunk?
            size_t remaining = m_data->size() - i;
            size_t bits_this_round = remaining < 8 ? remaining : 8;

            // Shift in each bit (MSB-first): shift accumulator left one and OR
            // the next bit into the lowest position. After processing all bits
            // of this chunk the bits will occupy the lowest `bits_this_round`
            // positions of the accumulator, in the same order they appeared in
            // m_data.
            for (size_t j = 0; j < bits_this_round; ++j) {

                // (*m_data)[i + j] yields a proxy/bool; cast to uint8_t for OR.
                byte_acc = static_cast<uint8_t>((byte_acc << 1) |
                                                static_cast<uint8_t>((*m_data)[i + j]));
            }

            // If this was a partial chunk (< 8 bits), shift the assembled
            // bits left so they occupy the most-significant bits of the byte
            // (bit 7 downwards). For a full chunk, (bits_this_round == 8)
            // this shift is zero.
            if (bits_this_round < 8) {
                byte_acc <<= static_cast<uint8_t>(8 - bits_this_round);
            }

            // Write the packed byte to the file. Cast to char for file.put.
            file.put(static_cast<char>(byte_acc));
        }

        // Observe the niceties
        file.close();
    }
    
    // If the file could not be opened, throw an error
    else {
        throw std::runtime_error("Could not open binary file for writing: " + getPath());
    }
}

// ****************** CSVFile Implementation ******************

// Constructor for CSVFile
CSVFile::CSVFile(std::string path, std::vector<std::vector<std::string>>* data) : m_data(data) {

    // If no data provided, create a new vector<vector<string>>
    if(!data) m_data = new std::vector<std::vector<std::string>>();
    
    // Set the file path
    setPath(path);
}

// Destructor for CSVFile
CSVFile::~CSVFile() = default;

// Implements reading from a CSV file
void CSVFile::read() {

    // Open the file for reading
    std::ifstream file;
    file.open(getPath(), std::ios::in);

    // If the file is open, read its contents into the vector<vector<string>>
    if (file.is_open()) {

        // Clear existing data
        m_data->clear();

        // Read the file content line by line
        std::string line;
        while (std::getline(file, line)) {

            // Split the line by commas into cells
            std::vector<std::string> row;
            std::string cell;
            std::stringstream lineStream(line);

            // Extract each cell separated by commas
            while (std::getline(lineStream, cell, ',')) {
                row.push_back(cell);
            }

            // Add the row to the data
            m_data->push_back(row);
        }

        // Observe the niceties
        file.close();
    }
    
    // If the file could not be opened, throw an error
    else {
        throw std::runtime_error("Could not open CSV file for reading: " + getPath());
    }
}

// Implements writing to a CSV file
void CSVFile::write() {

    // Open the file for writing
    std::ofstream file;
    file.open(getPath(), std::ios::out);

    // If the file is open, write the vector<vector<string>>'s contents to the file
    if (file.is_open()) {

        // Write each row as a line, with cells separated by commas
        for (const auto& row : *m_data) {

            // Write each cell in the row, separated by commas
            for (size_t i = 0; i < row.size(); ++i) {

                // Write the cell
                file << row[i];

                // Separate cells with commas
                if (i < row.size() - 1) file << ",";
            }

            // End the line after each row
            file << "\n";
        }

        // Observe the niceties
        file.close();
    }
    
    // If the file could not be opened, throw an error
    else {
        throw std::runtime_error("Could not open CSV file for writing: " + getPath());
    }
}