#pragma once

#include <string>
#include <sstream>
#include <vector>

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

class TextFile : public File {
public:
    // Constructors and Destructors
    TextFile(std::string path = "", std::stringstream* data = nullptr);
    ~TextFile();

    void read() override;

    void write() override;

    // Setters and Getters
    std::stringstream* getData() const { return m_data; }
    void setData(std::stringstream* data) { m_data = data; }
private:
    std::stringstream *m_data;
};

class BinaryFile : public File {
public:
    // Constructors and Destructors
    BinaryFile(std::string path = "", std::vector<bool>* data = nullptr);
    ~BinaryFile();

    void read() override;

    void write() override;

    // Setters and Getters
    std::vector<bool> *getData() { return m_data; }
    void setData(std::vector<bool>* data) { m_data = data; }

private:
    std::vector<bool> *m_data;
};

class CSVFile : public File {
public:
    // Constructors and Destructors
    CSVFile(std::string path = "", std::vector<std::vector<std::string>>* data = nullptr);
    ~CSVFile();

    void read() override;

    void write() override;

    // Setters and Getters
    std::vector<std::vector<std::string>> *getData() { return m_data; }
    void setData(std::vector<std::vector<std::string>>* data) { m_data = data; }
private:
    std::vector<std::vector<std::string>> *m_data;
};