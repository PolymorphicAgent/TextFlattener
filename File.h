#pragma once

#include <string>
#include <sstream>
#include <vector>

/// @brief This is an abstract base class for different types of files.
class File {
public:

    // Virtual functions
    virtual void read() = 0;
    virtual void write() = 0;

    // Shared functions
    std::string getPath() const { return m_filePath; }
    void setPath(std::string path) { m_filePath = path; }

private:
    std::string m_filePath;

};

/// @brief This class represents a text file.
/// @note The data is stored in a stringstream for ease of manipulation.
class TextFile : public File {
public:
    
    /// @brief The constructor for TextFile.
    /// @param path The file path.
    /// @param data The stringstream to use as data storage. If nullptr, a new stringstream will be created.
    /// @note If a stringstream is provided, the caller is responsible for managing its memory.
    TextFile(std::string path = "", std::stringstream* data = nullptr);

    /// @brief The destructor for TextFile.
    ~TextFile();

    /// @brief Reads the file from disk into the stringstream.
    void read() override;

    /// @brief Writes the contents of the stringstream to disk.
    void write() override;

    /// @brief Gets the stringstream data.
    /// @note The caller is responsible for managing the memory of the returned stringstream if it was provided in the constructor.
    /// @return The stringstream data.
    std::stringstream* getData() const { return m_data; }

    /// @brief Sets the stringstream data.
    /// @param data The new stringstream to use as data storage.
    /// @note The caller is responsible for managing the memory of the provided stringstream.
    void setData(std::stringstream* data) { m_data = data; }

    /// @brief Normalizes punctuation in the text data by converting smart quotes and apostrophes to straight quotes and apostrophes. Also converts accented e to regular e.
    /// @note This modifies the content of the stringstream in place.
    void normalizePunctuation();
private:

    /// @brief The stringstream that holds the file's data.
    /// @note If this was provided in the constructor, the caller is responsible for managing its memory.
    std::stringstream *m_data;
};

/// @brief This class represents a binary file.
/// @note The data is stored in a vector<bool> for ease of manipulation.
class BinaryFile : public File {
public:

    /// @brief The constructor for BinaryFile.
    /// @param path The file path.
    /// @param data The vector<bool> to use as data storage. If nullptr, a new vector<bool> will be created.
    /// @note If a vector<bool> is provided, the caller is responsible for managing its memory.
    BinaryFile(std::string path = "", std::vector<bool>* data = nullptr);

    /// @brief The destructor for BinaryFile.
    ~BinaryFile();

    /// @brief Reads the file from disk into the vector<bool>.
    void read() override;

    /// @brief Writes the contents of the vector<bool> to disk.
    void write() override;

    /// @brief Gets the vector<bool> data.
    /// @note The caller is responsible for managing the memory of the returned vector<bool> if it was provided in the constructor.
    /// @return The vector<bool> data.
    std::vector<bool> *getData() { return m_data; }

    /// @brief Sets the vector<bool> data.
    /// @param data The new vector<bool> to use as data storage.
    /// @note The caller is responsible for managing the memory of the provided vector<bool>.
    void setData(std::vector<bool>* data) { m_data = data; }

private:

    /// @brief The vector<bool> that holds the file's data.
    /// @note If this was provided in the constructor, the caller is responsible for managing its memory.
    std::vector<bool> *m_data;
};

/// @brief This class represents a CSV file.
/// @note The data is stored in a vector of vector of strings for ease of manipulation.
class CSVFile : public File {
public:

    /// @brief The constructor for CSVFile.
    /// @param path The file path.
    /// @param data The vector<vector<string>> pointer to use as data storage. If nullptr, a new vector<vector<string>> will be created.
    /// @note If a vector<vector<string>> is provided, the caller is responsible for managing its memory.
    CSVFile(std::string path = "", std::vector<std::vector<std::string>> *data = nullptr);

    /// @brief The destructor for CSVFile.
    ~CSVFile();

    /// @brief Reads the file from disk into the vector<vector<string>>.
    void read() override;

    /// @brief Writes the contents of the vector<vector<string>> to disk.
    void write() override;

    /// @brief Gets the vector<vector<string>> data.
    /// @note The caller is responsible for managing the memory of the returned vector<vector<string>> if it was provided in the constructor.
    /// @return The vector<vector<string>> data.
    std::vector<std::vector<std::string>> *getData() { return m_data; }

    /// @brief Sets the vector<vector<string>> data.
    /// @param data The new vector<vector<string>> pointer to use as data storage.
    /// @note The caller is responsible for managing the memory of the provided vector<vector<string>>.
    void setData(std::vector<std::vector<std::string>> *data) { m_data = data; }

private:
    /// @brief The vector<vector<string>> pointer that holds the file's data.
    /// @note If this was provided in the constructor, the caller is responsible for managing its memory.
    std::vector<std::vector<std::string>> *m_data;
};