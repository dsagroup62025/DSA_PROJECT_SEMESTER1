#include <iostream>
#include <string>
#include <stdexcept>
#include "chunker.h"
#include "merger.h"
#include "utils.h"

#ifdef _WIN32
#include <windows.h>
void setMainColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define COLOR_CYAN 11
#define COLOR_RED 12
#define COLOR_WHITE 7
#else
#define setMainColor(x)
#define COLOR_GREEN 0
#define COLOR_YELLOW 0
#define COLOR_CYAN 0
#define COLOR_RED 0
#define COLOR_WHITE 0
#endif

void printHeader() {
    setMainColor(COLOR_CYAN);
    std::cout << "**********************************************************\n";
    std::cout << "*                                                        *\n";
    std::cout << "*          BIG DATA FILE SORTER v1.0                     *\n";
    std::cout << "*          External Sorting Algorithm                    *\n";
    std::cout << "*                                                        *\n";
    std::cout << "**********************************************************\n";
    setMainColor(COLOR_WHITE);
    std::cout << "\n";
}

void printUsage(const char* programName) {
    std::cout << "Usage:\n";
    std::cout << "  " << programName << " <input_file> <output_file> [chunk_size_mb]\n\n";
    std::cout << "Arguments:\n";
    std::cout << "  input_file    - File containing unsorted integers (one per line)\n";
    std::cout << "  output_file   - Output file for sorted integers\n";
    std::cout << "  chunk_size_mb - (Optional) Chunk size in MB (default: 512)\n\n";
    std::cout << "Example:\n";
    std::cout << "  " << programName << " input.txt output.txt\n";
    std::cout << "  " << programName << " input.txt output.txt 256\n\n";
}

void printSummary(const std::string& inputFile,
    const std::string& outputFile,
    int chunkCount,
    double totalTime,
    size_t inputSize) {

    setMainColor(COLOR_GREEN);
    std::cout << "\n**********************************************************\n";
    std::cout << "  *                 SORTING COMPLETE                      *\n";
    std::cout << "  **********************************************************\n";
    setMainColor(COLOR_WHITE);

    std::cout << "  * Input file:        " << inputFile;
    // Padding
    for (size_t i = inputFile.length(); i < 34; i++) std::cout << " ";
    std::cout << "  *\n";

    std::cout << "  * Output file:       " << outputFile;
    for (size_t i = outputFile.length(); i < 34; i++) std::cout << " ";
    std::cout << "  *\n";

    std::cout << "  * File size:         " << formatSize(inputSize);
    std::string sizeStr = formatSize(inputSize);
    for (size_t i = sizeStr.length(); i < 34; i++) std::cout << " ";
    std::cout << "  *\n";

    std::cout << "  * Chunks created:    " << chunkCount;
    std::string chunkStr = std::to_string(chunkCount);
    for (size_t i = chunkStr.length(); i < 34; i++) std::cout << " ";
    std::cout << "  *\n";

    std::cout << "  * Total time:        " << formatTime(totalTime);
    std::string timeStr = formatTime(totalTime);
    for (size_t i = timeStr.length(); i < 34; i++) std::cout << " ";
    std::cout << "  *\n";

    if (totalTime > 0 && inputSize > 0) {
        double throughputMBs = (inputSize / (1024.0 * 1024.0)) / totalTime;
        std::cout << "* Throughput:        " << std::fixed << std::setprecision(2)
            << throughputMBs << " MB/s";
        std::string tpStr = std::to_string((int)throughputMBs) + " MB/s";
        for (size_t i = tpStr.length() + 5; i < 34; i++) std::cout << " ";
        std::cout << "*\n";
    }

    setMainColor(COLOR_GREEN);
    std::cout << "**************************************************\n";
    setMainColor(COLOR_WHITE);
}

// Get file size
size_t getFileSize(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return 0;
    return file.tellg();
}

int main(int argc, char* argv[]) {
    printHeader();

    // Parse command line arguments
    if (argc < 3) {
        printUsage(argv[0]);
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    // Default chunk size: 512MB
    size_t chunkSizeMB = 512;
    if (argc >= 4) {
        try {
            chunkSizeMB = std::stoul(argv[3]);
            if (chunkSizeMB < 1 || chunkSizeMB > 4096) {
                setMainColor(COLOR_RED);
                std::cerr << "Error: Chunk size must be between 1 and 4096 MB\n";
                setMainColor(COLOR_WHITE);
                return 1;
            }
        }
        catch (...) {
            setMainColor(COLOR_RED);
            std::cerr << "Error: Invalid chunk size\n";
            setMainColor(COLOR_WHITE);
            return 1;
        }
    }

    size_t chunkSizeBytes = chunkSizeMB * 1024 * 1024;

    // Display configuration
    std::cout << "Configuration:\n";
    std::cout << "  Input:      " << inputFile << "\n";
    std::cout << "  Output:     " << outputFile << "\n";
    std::cout << "  Chunk size: " << chunkSizeMB << " MB\n\n";

    // Check input file exists
    std::ifstream checkInput(inputFile);
    if (!checkInput.is_open()) {
        setMainColor(COLOR_RED);
        std::cerr << "Error: Cannot open input file: " << inputFile << "\n";
        setMainColor(COLOR_WHITE);
        std::cout << "\nPress Enter to exit...";
        std::cin.get();
        return 1;
    }

    size_t inputSize = getFileSize(inputFile);
    checkInput.close();

    std::cout << "Input file size: " << formatSize(inputSize) << "\n\n";

    // Start processing
    Timer totalTimer;

    try {
        // PHASE 1: CHUNKING
        setMainColor(COLOR_YELLOW);
        std::cout << "*******************************************************\n";
        std::cout << "  PHASE 1/3: CHUNKING & SORTING\n";
        std::cout << "*******************************************************\n";
        setMainColor(COLOR_WHITE);

        Chunker chunker(inputFile, chunkSizeBytes);
        auto chunkFiles = chunker.createSortedChunks();

        if (chunkFiles.empty()) {
            setMainColor(COLOR_RED);
            std::cerr << "Error: No chunks created (empty input file?)\n";
            setMainColor(COLOR_WHITE);
            return 1;
        }

        // PHASE 2: MERGING
        setMainColor(COLOR_YELLOW);
        std::cout << "*******************************************************\n";
        std::cout << "  PHASE 2/3: K-WAY MERGE\n";
        std::cout << "*******************************************************\n";
        setMainColor(COLOR_WHITE);

        Merger merger(chunkFiles, outputFile);
        merger.merge();

        // PHASE 3: CLEANUP
        setMainColor(COLOR_YELLOW);
        std::cout << "*******************************************************\n";
        std::cout << "  PHASE 3/3: CLEANUP\n";
        std::cout << "*******************************************************\n";
        setMainColor(COLOR_WHITE);

        std::cout << "Cleaning up temporary files...\n";
        chunker.cleanupTempFiles();
        setMainColor(COLOR_GREEN);
        std::cout << " Cleanup complete\n";
        setMainColor(COLOR_WHITE);

        // SUMMARY
        double totalTime = totalTimer.elapsed();
        printSummary(inputFile, outputFile, chunker.getChunkCount(),
            totalTime, inputSize);

        std::cout << "\nSorting completed successfully!\n";

    }
    catch (const std::exception& e) {
        setMainColor(COLOR_RED);
        std::cerr << "\n Error occurred: " << e.what() << "\n";
        setMainColor(COLOR_WHITE);

        std::cout << "\nPress Enter to exit...";
        std::cin.get();
        return 1;
    }

    std::cout << "\nPress Enter to exit...";
    std::cin.get();

    return 0;
}