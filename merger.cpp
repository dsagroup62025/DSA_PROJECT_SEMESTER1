#include "merger.h"
#include "utils.h"
#include <iostream>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
void setMergerColor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define COLOR_CYAN 11
#define COLOR_WHITE 7
#else
#define setMergerColor(x)
#define COLOR_GREEN 0
#define COLOR_YELLOW 0
#define COLOR_CYAN 0
#define COLOR_WHITE 0
#endif

//Constructor
Merger::Merger(const std::vector<std::string>& chunks, const std::string& output)
	: chunkFilenames(chunks), outputFilename(output) {
	
	if (chunks.empty()) {
		throw std::runtime_error("No chunks to merge");
	}

	std::cout << "Merger initialized:\n";
	std::cout << "Input chunks: " << chunks.size() << "\n";
	std::cout << "Ouput file : " << output << "\n";
}

//Destructor
Merger::~Merger() {
	closeAllChunks();
}

//Open all chunk files
void Merger::openAllChunks() {
	chunkStreams.resize(chunkFilenames.size());

	for (size_t i = 0; i < chunkFilenames.size(); i++) {
		chunkStreams[i].open(chunkFilenames[i]);

		if(!chunkStreams[i].is_open()) {
			throw std::runtime_error("Cannot open chunk files: " + chunkFilenames[i]);
		}

		//read first elements from each chunk into help
		readNextFromChunk(i);
	}
	std::cout << "All " << chunkFilenames.size() << " chunks opened\n";
}
//Clode all chunk files
void Merger::closeAllChunks() {
	for (auto& stream : chunkStreams) {
		if (stream.is_open()) {
			stream.close();
		}
	}
}
//Read next integer from specific chunk
bool Merger::readNextFromChunk(int chunkIndex) {
	if (chunkIndex >= static_cast<int>(chunkStreams.size())) {
		return false;
	}

	int value;
	if (chunkStreams[chunkIndex] >> value) {
		MergeElement element;
		element.value = value;
		element.chunkIndex = chunkIndex;
		minHeap.push(element);
		return true;
	}
	return false; //No more data in this chunk
}

//Main merge operation
void Merger::merge() {
	setMergerColor(COLOR_YELLOW);
	std::cout << "\n[Merging Phase]\n";
	setMergerColor(COLOR_WHITE);

	Timer timer;

	//Open all chunk files
	std::cout << "Opening chunk files...\n";
	openAllChunks();

	//Open output file
	std::ofstream output(outputFilename);
	if (!output.is_open()) {
		throw std::runtime_error("Cannot create output file: " + outputFilename);
	}

	std::cout << " Starting " << chunkFilenames.size() << "-wahy merge...\n";
	
	size_t totalMerged = 0;
	size_t progressInterval = 100000; // Update

	//K-way merge using minHeap
	while (!minHeap.empty()) {
		//Extract minium element
		MergeElement minElement = minHeap.top();
		minHeap.pop();

		//Write to output
		output << minElement.value << "\n";
		totalMerged++;

		//Progress indicator
		if (totalMerged % progressInterval == 0) {
			std::cout << "\nMerged: " << (totalMerged / 1000000.0) << " M elements..."
				<< std::flush;
		}
		//Read next element from same chunk
		readNextFromChunk(minElement.chunkIndex);
	}

	//Cleanup
	output.close();
	closeAllChunks();

	//Summary
	setMergerColor(COLOR_GREEN);
	std::cout << "\r MERGING COMPLETE!   \n";
	setMergerColor(COLOR_WHITE);
	std::cout << " Total elements Merged :" << totalMerged << "\n";
	std::cout << " Output file :" << outputFilename << "\n";
	std::cout << " Time: " << formatTime(timer.elapsed()) << "\n";

	//calculate throughput
	double seconds = timer.elapsed();
	if (seconds> 0) {
		double throughput = totalMerged / seconds;
		std::cout << " Throughput :" << static_cast<size_t>(throughput)
			<< "elements/sec\n";
	}
	std::cout << "\n";
}