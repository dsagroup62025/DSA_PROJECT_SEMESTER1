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

| File Size | Elements | Time | Throughput | Memory Used |
|-----------|----------|------|------------|-------------|
| 40 MB     | 10M      |15.2s | 2.6 MB/s   | 256 MB      |
| 400 MB    | 100M     | 178s | 2.2 MB/s   | 256 MB      |
| 1 GB      | 250M     | 520s | 2.0 MB/s   | 256 MB      |
| 4 GB      | 1B       | 2400s| 1.7 MB/s   | 512 MB      |

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

Full documentation and theoretical analysis available in [docs/report.pdf](docs/report.pdf)

## 🎥 Demo Video

[Link to demo video]

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
