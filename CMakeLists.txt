# 1. Thiết lập cơ bản
cmake_minimum_required(VERSION 3.10) [cite: 1]
project(BigDataSorter VERSION 1.0) [cite: 1]

# 2. Cấu hình C++ Standard (Sử dụng C++17 cho tính năng hiện đại)
set(CMAKE_CXX_STANDARD 17) [cite: 1]
set(CMAKE_CXX_STANDARD_REQUIRED ON) [cite: 1]

# 3. Tối ưu hóa trình biên dịch (Giữ nguyên phần nâng cao của bạn)
if(MSVC)
    add_compile_options(/W4 /O2) # Cấp độ cảnh báo cao và tối ưu tốc độ cho Windows 
else()
    add_compile_options(-Wall -Wextra -O3) # Cho Linux/Mac 
endif()

# 4. Tự động tìm file nguồn (Cải tiến để tránh lỗi đường dẫn)
# Lệnh này sẽ tự quét file .cpp ở thư mục gốc nếu bạn không để trong folder /src
file(GLOB SORTER_SOURCES "*.cpp") 
# Loại bỏ các file công cụ khỏi danh sách nguồn của chương trình chính
list(REMOVE_ITEM SORTER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/generate_data.cpp" 
                                "${CMAKE_CURRENT_SOURCE_DIR}/verify_sorted.cpp" 
                                "${CMAKE_CURRENT_SOURCE_DIR}/benchmark.cpp")

# 5. Tạo các file thực thi (Targets)
# Chương trình chính
add_executable(sorter ${SORTER_SOURCES}) [cite: 1]

# Các công cụ hỗ trợ (Chỉ cần biên dịch khi cần thiết)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/generate_data.cpp")
    add_executable(generate_data generate_data.cpp) [cite: 1]
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/verify_sorted.cpp")
    add_executable(verify_sorted verify_sorted.cpp) [cite: 1]
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/benchmark.cpp")
    add_executable(benchmark benchmark.cpp ${SORTER_SOURCES}) [cite: 1]
endif()

# 6. Cài đặt (Xuất file ra thư mục bin khi hoàn tất)
install(TARGETS sorter generate_data verify_sorted DESTINATION bin) [cite: 1]