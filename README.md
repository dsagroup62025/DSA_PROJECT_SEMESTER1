# DSA_PROJECT_SEMESTER1
# Big Data File Sorter (C++)
**High-performance External Sorting for datasets exceeding RAM limits.**

External sorting algorithm for files larger than RAM using heap sort and k-way merge.
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()

## 🎯 Project Overview

**Course:** Data Structures & Algorithms - Final Project  
**Authors:** [Huynh Minh Tai - 22110068]  +  [Van Pham Thao Nhi - 23110049]

**Date:** 31 December 2025  

**Topics Covered:**
- **D1-2:** Asymptotic Analysis  
- **D8:** External Sorting (I/O Bottlenecks)  
- **D9:** Heap Sort & Hybrid Guarding  

## 📊 Features

- ✅ Sorts files 10x larger than available RAM
- ✅ Guaranteed O(n log n) time complexity
- ✅ In-place heap sort for chunks
- ✅ Efficient k-way merge with min-heap
- ✅ Automatic cleanup of temporary files
- ✅ Real-time progress monitoring

## 🚀 Quick Start

### Prerequisites
- C++17 compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Minimum 512 MB RAM
- Sufficient disk space (2x input file size)

### Building

#### Windows (Visual Studio)
```cmd
# Open DSA_PROJECT_SEMESTER1.sln in Visual Studio
# Build → Build Solution (Ctrl+Shift+B)
```

#### Windows (MinGW)
```bash
g++ -std=c++17 -O3 src/*.cpp -o sorter.exe
```

#### Linux/Mac
```bash
make
# or
g++ -std=c++17 -O3 src/*.cpp -o sorter
```

### Running
```bash
# Basic usage
./sorter input.txt output.txt

# With custom chunk size (in MB)
./sorter input.txt output.txt 256
```

## 📁 Project Structure
```
DSA_PROJECT_SEMESTER1/
├── src/
│   ├── main.cpp           # Main program
│   ├── chunker.h/cpp      # File splitting & sorting
│   ├── merger.h/cpp       # K-way merge
│   ├── heap.h/cpp         # Heap sort implementation
│   ├── file_io.h/cpp      # I/O utilities
│   ├── sorter.h           # Main sorter interface
│   └── utils.h/cpp        # Helper functions
├── tools/
│   ├── generate_data.cpp  # Test data generator
│   ├── verify_sorted.cpp  # Output verification
│   └── benchmark.cpp      # Performance testing
├── tests/
│   ├── test_heap.cpp
│   ├── test_chunker.cpp
│   └── test_merger.cpp
├── benchmarks/
│   ├── results.csv        # Performance data
│   ├── plot_results.py    # Visualization script
│   └── charts/            # Generated charts
├── docs/
│   └── report.pdf         # Research synthesis report
└── README.md
```

## 🔧 Utility Tools

### Generate Test Data
```bash
./generate_data 10000000 input.txt
# Generates 10 million random integers
```

### Verify Output
```bash
./verify_sorted output.txt
# Checks if file is correctly sorted
```

### Run Benchmarks
```bash
./benchmark
# Tests with various file sizes
```

### Run All Tests
```bash
./run_tests.bat    
```

## 📊 Performance

Tested on: [Terminal on Windows]

| Chunk Size | Num Chunks | Phase 1 (Sort) | Phase 2 (Merge) | Total Time | Throughput     |
| :---       | :---       | :---           | :---            | :---       | :---           |
|**1.00 MB** | 4 chunks   | 4.1s           | 4.4s            | **8.5s**   | 228,571 elem/s |
|**2.00 MB** | 2 chunks   | 4.1s           | 4.3s            | **8.4s**   | 238,095 elem/s |
|**5.00 MB** | 1 chunk    | 4.1s           | 4.3s            | **8.4s**   | 238,095 elem/s |


## 🧮 Algorithm Complexity

**Time Complexity:**
- Chunking: O(n log k) where k = chunk size
- Merging: O(n log m) where m = number of chunks
- **Total: O(n log n)**

**Space Complexity:**
- O(M) where M = chunk size (in-place sorting)
- Temporary storage: O(n) on disk

**I/O Complexity:**
- O((n/B) × log(M/B)) in external memory model
- B = block size, M = memory size

## 📖 Documentation

Full documentation and theoretical analysis available in [docs/report.pdf](report.pdf)

## 🎥 Demo Video

https:

## 🤝 Contributing

This is an academic project. For contribution guidelines, see [CONTRIBUTING.md](CONTRIBUTING.md)

## 📄 License

MIT

## 👥 Authors

- **[Huynh Minh Tai - 22110068]** - Heap Sort & Chunking Implementation
- **[Van Pham Thao Nhi - 23110049]** - K-way Merge & Integration

## 🙏 Acknowledgments

- Course: Data Structures & Algorithms
- Institution: [Ho Chi Minh University of Technology and Engineering]
- Instructor: [Huynh Xuan Phung]
- References: [Data Structures and Algorithm]

## 📞 Contact

For questions about this project:
- Email: [dsagroup62025@gmail.com]
- GitHub: [DSA_PROJECT_SEMESTER1]
