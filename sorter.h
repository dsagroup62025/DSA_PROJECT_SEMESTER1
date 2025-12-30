#pragma once
#ifndef SORTER_H
#define SORTER_H

#include <string>
#include <vector>
#include <iostream>
#include "chunker.h"
#include "merger.h"
#include "utils.h"
#include "file_io.h"

class Sorter {
private:
    std::string inputFile;
    std::string outputFile;
    size_t chunkSize;
    inline void setColor(int color) {
#ifdef _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#endif
    }

    inline void printHeader() {
        setColor(11); // COLOR_CYAN
        std::cout << "**********************************************************\n";
        std::cout << "*          BIG DATA FILE SORTER v1.0                     *\n";
        std::cout << "*          External Sorting Algorithm                    *\n";
        std::cout << "**********************************************************\n\n";
        setColor(7); // WHITE
    }

public:
    Sorter(std::string input, std::string output, size_t size)
        : inputFile(input), outputFile(output), chunkSize(size) {
    }

    void run() {
        try {
            Timer totalTimer;
            printHeader();

            if (!FileIO::exists(inputFile)) {
                setColor(12); // COLOR_RED
                std::cout << "[Error] Input file not found: " << inputFile << "\n";
                setColor(7);
                return;
            }

            // PHASE 1: CHUNKING
            setColor(14); // COLOR_YELLOW
            std::cout << "PHASE 1/3: CHUNKING & SORTING (Heap Sort)\n";
            std::cout << "**********************************************************\n";
            setColor(7);

            Chunker chunker(inputFile, chunkSize);
            std::vector<std::string> tempFiles = chunker.createSortedChunks();

            // PHASE 2: MERGING
            setColor(14); // COLOR_YELLOW
            std::cout << "\nPHASE 2/3: K-WAY MERGE (Min-Heap)\n";
            std::cout << "**********************************************************\n";
            setColor(7);

            Merger merger(tempFiles, outputFile);
            merger.merge();

            // PHASE 3: CLEANUP
            setColor(14); // YELLOW
            std::cout << "\n**********************************************************\n";
            std::cout << "  PHASE 3/3: CLEANUP\n";
            std::cout << "**********************************************************\n";
            setColor(7);
            std::cout << "Cleaning up temporary files...\n";
            chunker.cleanupTempFiles();

            setColor(10); // GREEN
            std::cout << " Cleanup complete!\n";

            // --- SUMMARY ---
            setColor(10); // GREEN
            std::cout << "\n==========================================================\n";
            std::cout << " PROCESS COMPLETED SUCCESSFULLY\n";
            std::cout << " Total Execution Time: " << formatTime(totalTimer.elapsed()) << "\n";
            std::cout << "==========================================================\n";
            setColor(7);

        }
        catch (const std::exception& e) {
            setColor(12);
            std::cerr << "\n[Runtime Error] " << e.what() << "\n";
            setColor(7);
        }
    }
};

#endif