#include "chunker.h"
#include "heap.h"
#include "utils.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
void setColor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define COLOR_WHITE 7
#else
#define setColor(x)
#define COLOR_GREEN 0
#define COLOR_YELLOW 0
#define COLOR_WHITE 0
#endif

//Constructor
Chunker::Chunker(const std::string& filename, size_t chunkSize)
	:inputFilename(filename), chunkSizeBytes(chunkSize), chunkCount(0) {
	maxIntegers = chunkSizeBytes / sizeof(int);

	std::cout << " Chunker initialized:\n" ;
	std::cout << " Chunk size: " << formatSize(chunkSizeBytes) << "\n";
	std::cout << " Max integers per chunk : " << maxIntegers << "\n";
}

//Generate temp filename for chunk
std::string Chunker::getTempFilename(int index) const {
	return "temp_chunk_" + std::to_string(index) + ".txt";
}

//Sort chunk and write to file
void Chunker::sortAndWriteChunk(std::vector<int>& chunk, int index) {
	if (chunk.empty()) return;

	//Sort using Heap Sort
	Heap::heapSort(chunk);

	//Write to temp file
	std::string filename = getTempFilename(index);
	std::ofstream out(filename);

	if (!out.is_open()) {
		throw std::runtime_error("Cannot create temp file: " + filename);
	}

	for (int value : chunk) {
		out << value << "\n";
	}
	out.close();
}

//Main operation : create sorted chunks
std::vector<std::string> Chunker::createSortedChunks() {
	std::ifstream input(inputFilename);

	if (!input.is_open()) {
		throw std::runtime_error("Cannot open input file: " + inputFilename);
	}

	std::vector<std::string> chunkFiles;
	std::vector<int> currentChunk;
	currentChunk.reserve(maxIntegers);

	Timer timer;
	size_t totalIntegers = 0;
	int value;

	setColor(COLOR_YELLOW);
	std::cout << "\n[Chunking Phase]\n";
	setColor(COLOR_WHITE);

	//Read integers from input file
	while (input >> value) {
		currentChunk.push_back(value);
		totalIntegers++;

		//When chunk is full , sort an write it
		if (currentChunk.size() >= maxIntegers) {
			std::cout << "Proceesing chunk " << (chunkCount + 1)
				<< " (" << currentChunk.size() << " integers)...\n";

			sortAndWriteChunk(currentChunk, chunkCount);
			chunkFiles.push_back(getTempFilename(chunkCount));

			chunkCount++;
			currentChunk.clear();
			currentChunk.reserve(maxIntegers);
		}
	}

	//Handle remaining data in last chunk
	if (!currentChunk.empty()) {
		std::cout << "Processing final chunk" << (chunkCount + 1)
			<< " (" << currentChunk.size() << " integers)...\n";

		sortAndWriteChunk(currentChunk, chunkCount);
		chunkFiles.push_back(getTempFilename(chunkCount));
		chunkCount++;
	}
	input.close();

	//Summary
	setColor(COLOR_GREEN);
	std::cout << "Chunking Complete!\n";
	setColor(COLOR_WHITE);
	std::cout << " Total integers: " << totalIntegers << "\n";
	std::cout << " Chunks creates: " << chunkCount << "\n";
	std::cout << " Time: " << formatTime(timer.elapsed()) << "\n\n";

	return chunkFiles;
 }

//Cleanup temporary chunk files
void Chunker::cleanupTempFiles() {
	std::cout << " Cleaning up temporary files...\n";

	for (int i = 0; i < chunkCount; i++) {
		std::string filename = getTempFilename(i);
		if (std::remove(filename.c_str()) == 0) {
			std::cout << " Deleted " << filename << "\n";
		}
	}

	std::cout << " CLeanup completed \n";
}