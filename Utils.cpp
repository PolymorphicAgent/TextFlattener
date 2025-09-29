#include "Utils.h"

std::string Utils::extractFileName(std::string path){
    
    // simple cross-platform filename extraction
    auto pos = path.find_last_of("/\\");
    if (pos == std::string::npos) return path;
    return path.substr(pos + 1);
}