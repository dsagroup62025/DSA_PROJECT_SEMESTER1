#include <iostream>
#include "sorter.h"
#include "file_io.h"

int main() {
    // 1. Setup paths
    std::string inputFile = "input.txt"; // File 1,000,000 numbers
    std::string outputFile = "output_benchmark.txt";

    // 2. Ensure temp directory exists
    FileIO::makeDirectory("temp");

    // 3. Define different chunk sizes to test (in Bytes)
    // Testing with 1MB, 2MB, and 5MB chunks
    size_t tests[] = { 1024 * 1024, 2 * 1024 * 1024, 5 * 1024 * 1024 };

    for (size_t size : tests) {
        std::cout << "\n>>> TESTING WITH CHUNK SIZE: " << (size / (1024 * 1024)) << " MB\n";

        Sorter tester(inputFile, outputFile, size);
        tester.run();

        std::cout << "*******************************************\n";
    }

    std::cout << "Benchmark finished. Press Enter to exit...";
    std::cin.get();
    return 0;
}