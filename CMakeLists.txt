cmake_minimum_required(VERSION 3.10)
project(BigDataSorter VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Danh sách các file LOGIC (KHÔNG chứa hàm main)
set(CORE_SOURCES 
    chunker.cpp 
    merger.cpp 
    heap.cpp 
    file_io.cpp 
    utils.cpp
)

# 2. Tạo file thực thi chính (sorter.exe)
add_executable(sorter main.cpp ${CORE_SOURCES})

# 3. Tạo file chạy Benchmark (benchmark.exe)
add_executable(benchmark benchmark.cpp ${CORE_SOURCES})

# 4. Các công cụ hỗ trợ khác 
add_executable(generate_data generate_data.cpp)

add_executable(verify_sorted verify_sorted.cpp)
