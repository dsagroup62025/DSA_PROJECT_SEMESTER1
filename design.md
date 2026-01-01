\# Big Data File Sorter - Design Document



\*\*Version:\*\* 1.0  

\*\*Date:\*\* December 30, 2025  

\*\*Authors:\*\* Huỳnh Minh Tài  



---



\## Table of Contents



1\. \[System Overview](#1-system-overview)

2\. \[Architecture Design](#2-architecture-design)

3\. \[Module Specifications](#3-module-specifications)

4\. \[Data Structures](#4-data-structures)

5\. \[Algorithm Design](#5-algorithm-design)

6\. \[File Format Specifications](#6-file-format-specifications)

7\. \[Performance Considerations](#7-performance-considerations)

8\. \[Design Decisions](#8-design-decisions)



---



\## 1. System Overview



\### 1.1 Problem Statement



Sort extremely large files (10+ GB) that cannot fit entirely in RAM using external sorting techniques.



\### 1.2 Solution Approach



Divide-and-conquer strategy using:

1\. \*\*Chunking\*\*: Split input into RAM-sized chunks

2\. \*\*Sorting\*\*: Sort each chunk independently using Heap Sort

3\. \*\*Merging\*\*: K-way merge of sorted chunks using min-heap



\### 1.3 Key Requirements



\*\*Functional Requirements:\*\*

\- Sort files larger than available RAM

\- Handle integer data (one per line)

\- Produce sorted output file

\- Clean up temporary files



\*\*Non-Functional Requirements:\*\*

\- Time complexity: O(n log n)

\- Space complexity: O(M) where M = chunk size

\- Memory usage: < 512 MB regardless of input size

\- Reliable error handling



---



\## 2. Architecture Design



\### 2.1 High-Level Architecture

```

┌─────────────────────────────────────────────────────────────┐

│                     MAIN PROGRAM                            │

│                    (Orchestrator)                           │

└────────────┬────────────────────────────┬───────────────────┘

&nbsp;            │                            │

&nbsp;            ▼                            ▼

&nbsp;   ┌────────────────┐          ┌─────────────────┐

&nbsp;   │    CHUNKER     │          │     MERGER      │

&nbsp;   │   (Phase 1)    │          │   (Phase 2)     │

&nbsp;   └────────┬───────┘          └────────┬────────┘

&nbsp;            │                            │

&nbsp;            ▼                            ▼

&nbsp;   ┌────────────────┐          ┌─────────────────┐

&nbsp;   │  HEAP SORT     │          │   MIN-HEAP      │

&nbsp;   │  (Algorithm)   │          │  (K-way merge)  │

&nbsp;   └────────────────┘          └─────────────────┘

&nbsp;            │                            │

&nbsp;            └────────────┬───────────────┘

&nbsp;                         ▼

&nbsp;                 ┌──────────────┐

&nbsp;                 │   FILE I/O   │

&nbsp;                 │  (Utilities) │

&nbsp;                 └──────────────┘

&nbsp;                         │

&nbsp;                         ▼

&nbsp;                 ┌──────────────┐

&nbsp;                 │    UTILS     │

&nbsp;                 │  (Helpers)   │

&nbsp;                 └──────────────┘

```



\### 2.2 Data Flow

```

INPUT FILE (unsorted)

&nbsp;       │

&nbsp;       ▼

┌───────────────────┐

│  READ IN CHUNKS   │ ← Chunker

└────────┬──────────┘

&nbsp;        │

&nbsp;        ▼

┌───────────────────┐

│  SORT EACH CHUNK  │ ← Heap Sort

└────────┬──────────┘

&nbsp;        │

&nbsp;        ▼

┌───────────────────┐

│ WRITE TEMP FILES  │ ← temp\_chunk\_0.txt, temp\_chunk\_1.txt, ...

└────────┬──────────┘

&nbsp;        │

&nbsp;        ▼

┌───────────────────┐

│  K-WAY MERGE      │ ← Merger with Min-Heap

└────────┬──────────┘

&nbsp;        │

&nbsp;        ▼

┌───────────────────┐

│  WRITE OUTPUT     │ ← output.txt (sorted)

└────────┬──────────┘

&nbsp;        │

&nbsp;        ▼

┌───────────────────┐

│  CLEANUP TEMPS    │

└───────────────────┘

```



\### 2.3 Module Dependency Graph

```

main.cpp

&nbsp; ├─→ chunker.h/cpp

&nbsp; │     ├─→ heap.h/cpp

&nbsp; │     ├─→ file\_io.h/cpp

&nbsp; │     └─→ utils.h/cpp

&nbsp; │

&nbsp; └─→ merger.h/cpp

&nbsp;       ├─→ file\_io.h/cpp

&nbsp;       └─→ utils.h/cpp



heap.h/cpp (standalone)



utils.h/cpp (standalone)



file\_io.h/cpp

&nbsp; └─→ utils.h/cpp

```



---



\## 3. Module Specifications



\### 3.1 Main Module (`main.cpp`)



\*\*Responsibility:\*\* Orchestrate the entire sorting pipeline



\*\*Key Functions:\*\*

```cpp

int main(int argc, char\* argv\[])

```



\*\*Input:\*\* Command-line arguments (input file, output file, chunk size)



\*\*Output:\*\* Sorted file or error message



\*\*Flow:\*\*

1\. Parse arguments

2\. Validate input file

3\. Create Chunker instance

4\. Execute chunking phase

5\. Create Merger instance

6\. Execute merging phase

7\. Cleanup temporary files

8\. Display summary



---



\### 3.2 Chunker Module (`chunker.h/cpp`)



\*\*Responsibility:\*\* Split large file into sorted chunks



\*\*Class Definition:\*\*

```cpp

class Chunker {

private:

&nbsp;   std::string inputFilename;

&nbsp;   size\_t chunkSizeBytes;

&nbsp;   size\_t maxIntegers;

&nbsp;   int chunkCount;

&nbsp;   

public:

&nbsp;   Chunker(const std::string\& filename, size\_t chunkSize);

&nbsp;   std::vector<std::string> createSortedChunks();

&nbsp;   void cleanupTempFiles();

};

```



\*\*Key Operations:\*\*

1\. \*\*Read Chunk\*\*: Read up to `maxIntegers` from input

2\. \*\*Sort Chunk\*\*: Use Heap Sort

3\. \*\*Write Chunk\*\*: Write to `temp\_chunk\_N.txt`

4\. \*\*Repeat\*\*: Until entire file processed



\*\*Design Decisions:\*\*

\- Chunk size: Default 512 MB (configurable)

\- Max integers per chunk: `chunkSize / sizeof(int)`

\- Temp file naming: `temp\_chunk\_0.txt`, `temp\_chunk\_1.txt`, ...



---



\### 3.3 Merger Module (`merger.h/cpp`)



\*\*Responsibility:\*\* Merge sorted chunks into final output



\*\*Class Definition:\*\*

```cpp

class Merger {

private:

&nbsp;   std::vector<std::string> chunkFilenames;

&nbsp;   std::string outputFilename;

&nbsp;   std::vector<std::ifstream> chunkStreams;

&nbsp;   std::priority\_queue<MergeElement, ...> minHeap;

&nbsp;   

public:

&nbsp;   Merger(const std::vector<std::string>\& chunks, 

&nbsp;          const std::string\& output);

&nbsp;   void merge();

};



struct MergeElement {

&nbsp;   int value;

&nbsp;   int chunkIndex;

&nbsp;   bool operator>(const MergeElement\& other) const;

};

```



\*\*K-Way Merge Algorithm:\*\*

```

1\. Open all chunk files

2\. Read first element from each chunk into min-heap

3\. WHILE heap not empty:

&nbsp;    a. Extract minimum element from heap

&nbsp;    b. Write to output

&nbsp;    c. Read next element from same chunk

&nbsp;    d. If available, insert into heap

4\. Close all files

```



\*\*Time Complexity:\*\* O(n log k) where k = number of chunks



---



\### 3.4 Heap Module (`heap.h/cpp`)



\*\*Responsibility:\*\* Provide heap data structure and heap sort



\*\*Class Definition:\*\*

```cpp

class Heap {

private:

&nbsp;   std::vector<int> data;

&nbsp;   void heapifyDown(int index);

&nbsp;   void heapifyUp(int index);

&nbsp;   

public:

&nbsp;   void insert(int value);

&nbsp;   int extractMin();

&nbsp;   static void heapSort(std::vector<int>\& arr);

};

```



\*\*Heap Properties:\*\*

\- \*\*Type\*\*: Min-heap for priority queue, Max-heap for sorting

\- \*\*Structure\*\*: Complete binary tree (array representation)

\- \*\*Parent-child relationship\*\*: 

&nbsp; - Parent of i: `(i-1)/2`

&nbsp; - Left child of i: `2\*i+1`

&nbsp; - Right child of i: `2\*i+2`



\*\*Heap Sort Algorithm:\*\*

1\. Build max-heap: O(n)

2\. Extract max repeatedly: O(n log n)

3\. Total: O(n log n)



---



\### 3.5 File I/O Module (`file\_io.h/cpp`)



\*\*Responsibility:\*\* Buffered file reading/writing



\*\*Key Classes:\*\*

```cpp

class BufferedReader {

public:

&nbsp;   BufferedReader(const std::string\& filename, size\_t bufSize);

&nbsp;   bool readInt(int\& value);

&nbsp;   bool hasMore() const;

};



class BufferedWriter {

public:

&nbsp;   BufferedWriter(const std::string\& filename, size\_t bufSize);

&nbsp;   void writeInt(int value);

&nbsp;   void flush();

};

```



\*\*Design Rationale:\*\*

\- Buffer size: 8192 bytes (configurable)

\- Reduces system calls

\- Improves I/O performance by 2-3x



---



\### 3.6 Utils Module (`utils.h/cpp`)



\*\*Responsibility:\*\* Helper functions and utilities



\*\*Key Components:\*\*

```cpp

// Formatting

std::string formatSize(size\_t bytes);

std::string formatTime(double seconds);



// Timing

class Timer {

&nbsp;   void reset();

&nbsp;   double elapsed() const;

};



// Progress display

class ProgressBar {

&nbsp;   void update(size\_t progress);

&nbsp;   void finish();

};



// Console colors

void setConsoleColor(Color color);

```



---



\## 4. Data Structures



\### 4.1 Heap (Binary Heap)



\*\*Structure:\*\*

```

Array representation of complete binary tree



Index:  0   1   2   3   4   5   6

Value: \[1,  3,  5,  7,  4,  6,  8]



Tree view:

&nbsp;       1

&nbsp;      / \\

&nbsp;     3   5

&nbsp;    / \\ / \\

&nbsp;   7  4 6  8

```



\*\*Operations:\*\*

\- Insert: O(log n)

\- Extract-Min: O(log n)

\- Peek: O(1)

\- Build heap: O(n)



\*\*Use Cases:\*\*

\- Sorting chunks (max-heap)

\- K-way merge (min-heap)



---



\### 4.2 Priority Queue (for K-Way Merge)



\*\*Implementation:\*\* STL `std::priority\_queue` with min-heap



\*\*Element Structure:\*\*

```cpp

struct MergeElement {

&nbsp;   int value;        // The integer value

&nbsp;   int chunkIndex;   // Which chunk it came from

};

```



\*\*Why Min-Heap?\*\*

\- Always extract smallest element: O(log k)

\- k = number of chunks (typically 2-20)

\- Much smaller than n (total elements)



---



\### 4.3 Vector (Dynamic Array)



\*\*Used For:\*\*

\- Storing chunk data during sorting

\- Storing chunk filenames

\- Temporary buffers



\*\*Why Vector?\*\*

\- Dynamic resizing

\- Cache-friendly (contiguous memory)

\- STL algorithms support



---



\## 5. Algorithm Design



\### 5.1 Overall Algorithm: External Merge Sort



\*\*Pseudocode:\*\*

```

ExternalSort(inputFile, outputFile, chunkSize):

&nbsp;   // Phase 1: Chunking

&nbsp;   chunks = \[]

&nbsp;   WHILE data remains in inputFile:

&nbsp;       chunk = ReadChunk(inputFile, chunkSize)

&nbsp;       HeapSort(chunk)

&nbsp;       tempFile = WriteChunk(chunk)

&nbsp;       chunks.append(tempFile)

&nbsp;   

&nbsp;   // Phase 2: K-way Merge

&nbsp;   KWayMerge(chunks, outputFile)

&nbsp;   

&nbsp;   // Phase 3: Cleanup

&nbsp;   DeleteTemporaryFiles(chunks)

```



\*\*Time Complexity:\*\*

```

Let n = total elements, k = chunk size, m = number of chunks



Phase 1: O(n log k) × m = O(n log k)

Phase 2: O(n log m)

Total:   O(n log k) + O(n log m) = O(n log n)

```



\*\*Space Complexity:\*\*

```

In-memory: O(k) for chunk storage

On-disk:   O(n) for temporary files

```



---



\### 5.2 Heap Sort Algorithm



\*\*Pseudocode:\*\*

```

HeapSort(arr):

&nbsp;   n = arr.length

&nbsp;   

&nbsp;   // Build max-heap

&nbsp;   FOR i = n/2 - 1 DOWN TO 0:

&nbsp;       HeapifyDown(arr, n, i)

&nbsp;   

&nbsp;   // Extract elements

&nbsp;   FOR i = n-1 DOWN TO 1:

&nbsp;       Swap(arr\[0], arr\[i])

&nbsp;       HeapifyDown(arr, i, 0)



HeapifyDown(arr, n, i):

&nbsp;   largest = i

&nbsp;   left = 2\*i + 1

&nbsp;   right = 2\*i + 2

&nbsp;   

&nbsp;   IF left < n AND arr\[left] > arr\[largest]:

&nbsp;       largest = left

&nbsp;   IF right < n AND arr\[right] > arr\[largest]:

&nbsp;       largest = right

&nbsp;   

&nbsp;   IF largest != i:

&nbsp;       Swap(arr\[i], arr\[largest])

&nbsp;       HeapifyDown(arr, n, largest)

```



\*\*Properties:\*\*

\- \*\*In-place\*\*: No extra array needed

\- \*\*Time\*\*: O(n log n) worst-case (guaranteed!)

\- \*\*Space\*\*: O(1) auxiliary (O(log n) stack)



---



\### 5.3 K-Way Merge Algorithm



\*\*Pseudocode:\*\*

```

KWayMerge(chunkFiles, outputFile):

&nbsp;   minHeap = PriorityQueue()

&nbsp;   streams = \[]

&nbsp;   

&nbsp;   // Initialize

&nbsp;   FOR each chunkFile:

&nbsp;       stream = Open(chunkFile)

&nbsp;       streams.append(stream)

&nbsp;       value = Read(stream)

&nbsp;       minHeap.insert(MergeElement(value, chunkIndex))

&nbsp;   

&nbsp;   output = Open(outputFile)

&nbsp;   

&nbsp;   // Merge

&nbsp;   WHILE minHeap is not empty:

&nbsp;       element = minHeap.extractMin()

&nbsp;       Write(output, element.value)

&nbsp;       

&nbsp;       nextValue = Read(streams\[element.chunkIndex])

&nbsp;       IF nextValue exists:

&nbsp;           minHeap.insert(MergeElement(nextValue, element.chunkIndex))

&nbsp;   

&nbsp;   Close all streams

```



\*\*Time Complexity:\*\*

```

n elements × log(k) heap operations = O(n log k)



Where k << n (typically k = 2-20)

So effectively linear in practice!

```



---



\## 6. File Format Specifications



\### 6.1 Input File Format

```

<integer>\\n

<integer>\\n

<integer>\\n

...

```



\*\*Example:\*\*

```

42

-17

999

0

-5

```



\*\*Constraints:\*\*

\- One integer per line

\- Range: INT\_MIN to INT\_MAX (-2³¹ to 2³¹-1)

\- No leading/trailing whitespace (will be trimmed)



---



\### 6.2 Temporary Chunk Files



\*\*Naming Convention:\*\*

```

temp\_chunk\_0.txt

temp\_chunk\_1.txt

temp\_chunk\_2.txt

...

temp\_chunk\_N.txt

```



\*\*Format:\*\* Same as input (one integer per line, sorted)



\*\*Location:\*\* Same directory as executable



\*\*Lifecycle:\*\*

1\. Created during chunking phase

2\. Read during merging phase

3\. Deleted during cleanup phase



---



\### 6.3 Output File Format



\*\*Format:\*\* Same as input file (one integer per line)



\*\*Guarantee:\*\* Sorted in ascending order



\*\*Example:\*\*

```

-17

-5

0

42

999

```



---



\## 7. Performance Considerations



\### 7.1 Time Complexity Analysis



\*\*Phase 1 - Chunking:\*\*

```

Reading:  O(n)         - sequential read

Sorting:  O(n log k)   - heap sort each chunk

Writing:  O(n)         - sequential write

Total:    O(n log k)

```



\*\*Phase 2 - Merging:\*\*

```

Heap ops: O(log m)     - m chunks

Elements: n            - total elements

Total:    O(n log m)

```



\*\*Combined:\*\*

```

O(n log k) + O(n log m) = O(n log n)

Since k × m ≈ n

```



---



\### 7.2 Space Complexity Analysis



\*\*RAM Usage:\*\*

```

Chunk buffer:    O(k)     - k integers

Heap overhead:   O(k)     - for sorting

Merge heap:      O(m)     - m elements

I/O buffers:     O(1)     - constant

Total:           O(k)     - dominated by chunk

```



\*\*Disk Usage:\*\*

```

Input file:      O(n)

Output file:     O(n)

Temp chunks:     O(n)

Peak usage:      O(3n)

```



---



\### 7.3 I/O Complexity (External Memory Model)



\*\*Model Parameters:\*\*

\- B = block size (typically 4KB)

\- M = RAM size

\- n = file size



\*\*I/O Complexity:\*\*

```

Chunk sorting: O(n/B)          - sequential I/O

Merge phase:   O((n/B) log(M/B)) - per pass

Total:         O((n/B) log(M/B))

```



\*\*Practical Implications:\*\*

\- Larger chunks → fewer passes → faster

\- But chunks must fit in RAM

\- Optimal chunk size: M/2 to M/4



---



\### 7.4 Optimization Strategies



\*\*1. Chunk Size Tuning\*\*

```

Too small: Many chunks → slow merge

Too large: Doesn't fit in RAM → swapping

Optimal:   M/4 (leaves room for OS and buffers)

```



\*\*2. I/O Buffering\*\*

```

Unbuffered: 1 system call per integer → SLOW

Buffered:   1 system call per 8KB → FAST

Speedup:    2-3x improvement

```



\*\*3. K-Way Merge\*\*

```

Binary merge: log₂(m) passes

K-way merge:  log\_k(m) passes (fewer!)

Trade-off:    More complex, but fewer I/O

```



\*\*4. Memory Alignment\*\*

```

Align data structures to cache lines (64 bytes)

Improves cache hit rate

```



---



\## 8. Design Decisions



\### 8.1 Why Heap Sort (not Quick Sort)?



\*\*Reasons:\*\*



| Criterion | Heap Sort | Quick Sort |

|-----------|-----------|------------|

| Worst-case time | O(n log n) ✅ | O(n²) ❌ |

| Space | O(1) ✅ | O(log n) stack ⚠️ |

| In-place | Yes ✅ | Yes ✅ |

| Cache-friendly | Poor ⚠️ | Good ✅ |

| Predictable | Yes ✅ | No ❌ |



\*\*Decision:\*\* Heap Sort for \*\*guaranteed performance\*\* on worst-case input



---



\### 8.2 Why K-Way Merge (not Binary Merge)?



\*\*Binary Merge:\*\*

```

20 chunks → 10 → 5 → 3 → 2 → 1

5 passes, each reads/writes entire file

Total I/O: 5 × 2n = 10n

```



\*\*20-Way Merge:\*\*

```

20 chunks → 1

1 pass, reads all chunks once

Total I/O: 2n

```



\*\*Decision:\*\* K-way merge with min-heap reduces I/O passes



---



\### 8.3 Why Store Chunks on Disk (not in RAM)?



\*\*Reason:\*\* Input larger than RAM!

```

Input:      10 GB

RAM:        4 GB

Solution:   Store intermediate results on disk

```



---



\### 8.4 Why Cleanup Temporary Files?



\*\*Reasons:\*\*

1\. \*\*Disk space\*\*: Temp files = input size

2\. \*\*User expectations\*\*: No leftover files

3\. \*\*Professional\*\*: Clean up after yourself



\*\*Implementation:\*\* 

```cpp

void Chunker::cleanupTempFiles() {

&nbsp;   for (int i = 0; i < chunkCount; i++) {

&nbsp;       std::remove(getTempFilename(i).c\_str());

&nbsp;   }

}

```



---



\### 8.5 Design Trade-offs Summary



| Decision | Trade-off | Rationale |

|----------|-----------|-----------|

| Chunk size = 512MB | Memory vs Passes | Balance between RAM and I/O |

| Heap Sort | Speed vs Guarantee | Reliability over avg-case speed |

| K-way merge | Complexity vs I/O | Fewer passes worth the complexity |

| Buffered I/O | Code size vs Speed | 2-3x speedup worth extra code |

| No compression | Size vs Time | Sorting already slow, avoid overhead |



---



\## Appendix A: Class Diagrams

```

┌─────────────────────┐

│      Chunker        │

├─────────────────────┤

│ - inputFilename     │

│ - chunkSizeBytes    │

│ - maxIntegers       │

│ - chunkCount        │

├─────────────────────┤

│ + Chunker(...)      │

│ + createSortedChunks()│

│ + cleanupTempFiles()│

└──────────┬──────────┘

&nbsp;          │ uses

&nbsp;          ▼

┌─────────────────────┐

│       Heap          │

├─────────────────────┤

│ - data: vector<int> │

├─────────────────────┤

│ + insert(int)       │

│ + extractMin()      │

│ + heapSort(vector)  │

└─────────────────────┘



┌─────────────────────┐

│       Merger        │

├─────────────────────┤

│ - chunkFilenames    │

│ - outputFilename    │

│ - chunkStreams      │

│ - minHeap           │

├─────────────────────┤

│ + Merger(...)       │

│ + merge()           │

└──────────┬──────────┘

&nbsp;          │ uses

&nbsp;          ▼

┌─────────────────────┐

│   MergeElement      │

├─────────────────────┤

│ + value: int        │

│ + chunkIndex: int   │

│ + operator>()       │

└─────────────────────┘

```



---



\## Appendix B: Sequence Diagrams



\### B.1 Main Sorting Flow

```

User          Main         Chunker      HeapSort     Merger

&nbsp;|             |              |            |           |

&nbsp;|--run------->|              |            |           |

&nbsp;|             |--create----->|            |           |

&nbsp;|             |              |--sort----->|           |

&nbsp;|             |              |<-done------|           |

&nbsp;|             |<-chunks------|            |           |

&nbsp;|             |--create------------------>|           |

&nbsp;|             |                           |--merge--->|

&nbsp;|             |                           |<-done-----|

&nbsp;|             |--cleanup--->|             |           |

&nbsp;|<--complete--|             |             |           |

```



---



\## Appendix C: Performance Benchmarks

**Test System:**
- **CPU:** AMD Ryzen 7 6800H (16 CPUs), ~3.2GHz
- **RAM:** 24GB DDR5
- **Storage:** NVMe SSD
- **OS:** Windows 11 Home Single Language

**Experimental Results (1,000,000 Integers):**

Dựa trên dữ liệu thu thập được từ các lần chạy thực tế (Screenshot 21-24), hệ thống duy trì hiệu suất ổn định như sau:

| Chunk Size | Num Chunks | Phase 1 (Sort) | Phase 2 (Merge) | Total Time | Throughput |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **1.00 MB** | 4 chunks | 4.1s | 4.4s | **8.5s** | 228,571 elem/s |
| **2.00 MB** | 2 chunks | 4.1s | 4.3s | **8.4s** | 238,095 elem/s |
| **5.00 MB** | 1 chunk | 4.1s | 4.3s | **8.4s** | 238,095 elem/s |



**Observations & Analysis:**
1. **Tính ổn định cao:** Tổng thời gian thực thi duy trì ở mức ~8.4s - 8.5s bất kể số lượng chunk, chứng minh hiệu quả của thuật toán K-way Merge và Min-Heap.
2. **Nút thắt cổ chai I/O:** Thời gian Phase 1 (Chunking) cố định ở mức 4.1s, cho thấy tốc độ xử lý bị giới hạn bởi tốc độ đọc/ghi của ổ đĩa cứng hơn là tốc độ CPU.
3. **Tối ưu hóa bộ nhớ:** Việc sử dụng Min-Heap giúp quá trình trộn diễn ra mượt mà với độ phức tạp $O(N \log K)$, giảm thiểu tối đa các thao tác truy xuất đĩa không cần thiết.

\## 9. Conclusion & Future Work

\### 9.1 Conclusion
Dự án đã triển khai thành công hệ thống sắp xếp ngoài (External Sort) với cấu trúc module hóa rõ ràng. Kết quả thực nghiệm cho thấy hệ thống hoạt động ổn định, xử lý 1 triệu phần tử với tốc độ ấn tượng (~230,000 elem/s) và quản lý tài nguyên bộ nhớ hiệu quả thông qua việc điều chỉnh kích thước chunk.

\### 9.2 Future Work
- **Parallel Processing:** Áp dụng đa luồng (Multithreading) để song song hóa việc đọc và sắp xếp các chunk nhằm tối ưu hóa thời gian CPU.
- **Data Compression:** Nén các tệp tạm thời để giảm băng thông I/O và tiết kiệm dung lượng đĩa cứng.
- **Generic Support:** Mở rộng hệ thống để hỗ trợ các kiểu dữ liệu khác như chuỗi ký tự (strings) hoặc các bản ghi phức tạp (structs).



| Version | Date | Changes | Author |

|---------|------|---------|--------|

| 1.0 | 2025-12-30 | Initial design | \[Huỳnh Minh Tài] |



---



\*\*End of Design Document\*\*

