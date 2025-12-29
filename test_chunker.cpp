#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "chunker.h"
#include "utils.h"

// Generate test input file
void generateTestFile(const std::string& filename, int count) {
    std::cout << "Generating test file with " << count << " integers...\n";

    std::ofstream out(filename);
    for (int i = 0; i < count; i++) {
        out << (rand() % 10000 - 5000) << "\n";  // Random -5000 to 5000
    }
    out.close();

    std::cout << " Test file created: " << filename << "\n\n";
}

// Verify chunk file is sorted
bool verifyChunkSorted(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return false;

    int prev, curr;
    if (!(in >> prev)) return true;  // Empty file is sorted

    while (in >> curr) {
        if (curr < prev) {
            std::cout << "  NOT SORTED: " << prev << " > " << curr << "\n";
            return false;
        }
        prev = curr;
    }

    return true;
}

// Test 1: Small file
void testSmallFile() {
    std::cout << "***Test 1: Small File (1000 integers)***\n\n";

    generateTestFile("test_input_small.txt", 1000);

    // Create chunker with small chunk size (100 integers = 400 bytes)
    Chunker chunker("test_input_small.txt", 400);

    auto chunkFiles = chunker.createSortedChunks();

    std::cout << "Verifying chunks are sorted...\n";
    bool allSorted = true;
    for (const auto& file : chunkFiles) {
        bool sorted = verifyChunkSorted(file);
        std::cout << "  " << file << ": "
            << (sorted ? " SORTED" : " NOT SORTED") << "\n";
        allSorted = allSorted && sorted;
    }

    std::cout << "\n";
    if (allSorted) {
        std::cout << " All chunks are correctly sorted!\n";
    }
    else {
        std::cout << " Some chunks have errors!\n";
    }

    chunker.cleanupTempFiles();
    std::remove("test_input_small.txt");

    std::cout << "\n***********************************\n\n";
}

// Test 2: Medium file
void testMediumFile() {
    std::cout << "***Test 2: Medium File (100k integers)***\n\n";

    generateTestFile("test_input_medium.txt", 100000);

    // Chunk size: 10MB (can hold ~2.5M integers)
    Chunker chunker("test_input_medium.txt", 10 * 1024 * 1024);

    Timer timer;
    auto chunkFiles = chunker.createSortedChunks();

    std::cout << "Chunking took: " << formatTime(timer.elapsed()) << "\n";
    std::cout << "Chunks created: " << chunkFiles.size() << "\n\n";

    // Verify
    bool allSorted = true;
    for (const auto& file : chunkFiles) {
        if (!verifyChunkSorted(file)) {
            allSorted = false;
            break;
        }
    }

    if (allSorted) {
        std::cout << " All chunks verified sorted!\n";
    }

    chunker.cleanupTempFiles();
    std::remove("test_input_medium.txt");

    std::cout << "\n*************************************\n\n";
}

// Test 3: Edge cases
void testEdgeCases() {
    std::cout << "***Test 3: Edge Cases***\n\n";

    // Empty file
    {
        std::cout << "Testing empty file...\n";
        std::ofstream out("test_empty.txt");
        out.close();

        Chunker chunker("test_empty.txt", 1024);
        auto chunks = chunker.createSortedChunks();
        std::cout << "  Chunks from empty file: " << chunks.size() << " (expected: 0)\n";
        std::remove("test_empty.txt");
    }

    // Single element
    {
        std::cout << "\nTesting single element...\n";
        std::ofstream out("test_single.txt");
        out << "42\n";
        out.close();

        Chunker chunker("test_single.txt", 1024);
        auto chunks = chunker.createSortedChunks();
        std::cout << "  Chunks from single element: " << chunks.size() << " (expected: 1)\n";

        if (!chunks.empty() && verifyChunkSorted(chunks[0])) {
            std::cout << "  Single element chunk is sorted\n";
        }

        chunker.cleanupTempFiles();
        std::remove("test_single.txt");
    }

    // Already sorted
    {
        std::cout << "\nTesting already sorted file...\n";
        std::ofstream out("test_sorted.txt");
        for (int i = 0; i < 1000; i++) {
            out << i << "\n";
        }
        out.close();

        Chunker chunker("test_sorted.txt", 4000);  // ~1000 ints per chunk
        auto chunks = chunker.createSortedChunks();

        bool allSorted = true;
        for (const auto& chunk : chunks) {
            if (!verifyChunkSorted(chunk)) {
                allSorted = false;
                break;
            }
        }

        std::cout << "  " << (allSorted ? "YES" : "NO")
            << " Sorted input remains sorted\n";

        chunker.cleanupTempFiles();
        std::remove("test_sorted.txt");
    }

    std::cout << "\n*********************************************\n\n";
}

int main() {
    std::cout << "******************************************\n";
    std::cout << "*            CHUNKER TEST SUITE          *\n";
    std::cout << "******************************************\n\n";

    testSmallFile();
    testMediumFile();
    testEdgeCases();

    std::cout << "******************************************\n";
    std::cout << "*            ALL TESTS COMPLETE          *\n";
    std::cout << "******************************************\n";

    return 0;
}