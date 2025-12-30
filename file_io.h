#pragma once
#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>

class FileIO {
public:
    // Check if a file exists on disk
    static bool exists(const std::string& filename);

    // Get file size in bytes
    static size_t getFileSize(const std::string& filename);

    // Safely delete a file
    static void deleteFile(const std::string& filename);

    // Create a directory (useful for temp folders)
    static void makeDirectory(const std::string& path);
};

#endif
