#include "sorter.h"
#include <iostream>

int main() {
    // Configuration for 1,000,000 elements test
    std::string inputFile = "input.txt";
    std::string outputFile = "output_sorted.txt";

    // Set chunk size to 1MB to ensure multiple chunks are created for demo
    size_t chunkSize = 1024 * 1024;

    // Initialize the coordinator class
    Sorter externalSorter(inputFile, outputFile, chunkSize);

    // Run the full process
    externalSorter.run();

    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}