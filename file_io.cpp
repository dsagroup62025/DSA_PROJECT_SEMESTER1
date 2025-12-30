#include "file_io.h"
#include <fstream>
#include <cstdio>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h> 
#endif

bool FileIO::exists(const std::string& filename) {
    std::ifstream f(filename.c_str());
    return f.good();
}

size_t FileIO::getFileSize(const std::string& filename) {
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : 0;
}

void FileIO::deleteFile(const std::string& filename) {
    if (exists(filename)) {
        std::remove(filename.c_str());
    }
}

void FileIO::makeDirectory(const std::string& path) {
#ifdef _WIN32
    _mkdir(path.c_str());
#else
    mkdir(path.c_str(), 0777);
#endif
}