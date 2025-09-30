#include "File.h"

#include <fstream>

// ****************** TextFile Implementation ******************

// Constructor for TextFile
TextFile::TextFile(std::string path, std::stringstream* data) {
    if(!data)
        m_data = new std::stringstream();
    else
        m_data = data;
        
    setPath(path);
}

// Destructor for TextFile
TextFile::~TextFile() { if (m_data) delete m_data; }

// Implements reading from a text file
void TextFile::read() {
    std::ifstream file;
    file.open(getPath(), std::ios::in);
    if (file.is_open()) {
        m_data->clear(); // Reset the error state
        m_data->str(""); // Clear the string content
        *m_data << file.rdbuf();
        file.close();
    } else {
        throw std::runtime_error("Could not open text file for reading: " + getPath());
    }
}

// Implements writing to a text file
void TextFile::write() {
    std::ofstream file;
    file.open(getPath(), std::ios::out);
    if (file.is_open()) {
        if (m_data) {
            file << m_data->str();
        }
        file.close();
    } else {
        throw std::runtime_error("Could not open text file for writing: " + getPath());
    }
}

// ****************** BinaryFile Implementation ******************

// Constructor for BinaryFile
BinaryFile::BinaryFile(std::string path, std::vector<bool>* data) {
    if(!data)
        m_data = new std::vector<bool>();
    else
        m_data = data;
        
    setPath(path);
}

// Destructor for BinaryFile
BinaryFile::~BinaryFile() { if (m_data) delete m_data; }

// Implements reading from a binary file
void BinaryFile::read() {
    std::ifstream file;
    file.open(getPath(), std::ios::in | std::ios::binary);
    if (file.is_open()) {
        m_data->clear();
        char byte;
        while (file.get(byte)) {
            for (int i = 7; i >= 0; --i) {
                m_data->push_back((byte >> i) & 1);
            }
        }
        file.close();
    } else {
        throw std::runtime_error("Could not open binary file for reading: " + getPath());
    }
}

// Implement writing to a binary file
void BinaryFile::write() {
    std::ofstream file;
    file.open(getPath(), std::ios::out | std::ios::binary);
    if (file.is_open()) {
        for (size_t i = 0; i < m_data->size(); i += 8) {
            char byte = 0;
            for (size_t j = 0; j < 8 && (i + j) < m_data->size(); ++j) {
                byte = (byte << 1) | (*m_data)[i + j];
            }
            byte <<= (8 - (m_data->size() - i < 8 ? m_data->size() - i : 8)); // Shift if less than 8 bits
            file.put(byte);
        }
        file.close();
    } else {
        throw std::runtime_error("Could not open binary file for writing: " + getPath());
    }
}

// ****************** CSVFile Implementation ******************

// Constructor for CSVFile
CSVFile::CSVFile(std::string path, std::vector<std::vector<std::string>>* data) {
    if(!data)
        m_data = new std::vector<std::vector<std::string>>();
    else
        m_data = data;
        
    setPath(path);
}

// Destructor for CSVFile
CSVFile::~CSVFile() { if (m_data) delete m_data; }

// Implements reading from a CSV file
void CSVFile::read() {
    std::ifstream file;
    file.open(getPath(), std::ios::in);
    if (file.is_open()) {
        m_data->clear();
        std::string line;
        while (std::getline(file, line)) {
            std::vector<std::string> row;
            std::string cell;
            std::stringstream lineStream(line);
            while (std::getline(lineStream, cell, ',')) {
                row.push_back(cell);
            }
            m_data->push_back(row);
        }
        file.close();
    } else {
        throw std::runtime_error("Could not open CSV file for reading: " + getPath());
    }
}

// Implements writing to a CSV file
void CSVFile::write() {
    std::ofstream file;
    file.open(getPath(), std::ios::out);
    if (file.is_open()) {
        for (const auto& row : *m_data) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];
                if (i < row.size() - 1) file << ",";
            }
            file << "\n";
        }
        file.close();
    } else {
        throw std::runtime_error("Could not open CSV file for writing: " + getPath());
    }
}