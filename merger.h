#pragma once
#ifndef MERGER_H
#define MERGER_H

#include <string>
#include <vector>
#include <fstream>
#include <queue>

//Element in merge heap
struct MergeElement {
	int value;
	int chunkIndex;

	//For min-heap comparison
	bool operator > (const MergeElement& other) const {
		return value > other.value;
	}
};

//Merger: K-way merge of sorted chunk files
class Merger {
private:
	std::vector<std::string> chunkFilenames;
	std::string outputFilename;

	//File streams for each chunk
	std::vector<std::ifstream> chunkStreams;

	//Min-heap for k-way merge
	std::priority_queue<MergeElement,
		std::vector<MergeElement>,
		std::greater<MergeElement>> minHeap;

	//Helper functions
	bool readNextFromChunk(int chunkIndex);
	void openAllChunks();
	void closeAllChunks();

public:
	//Constructor
	Merger(const std::vector<std::string>& chunks, const std::string& output);

	//Destructor
	~Merger();

	//Main operation: merge all chunks
	void merge();

	//Getters
	int getChunkCount() const { return chunkFilenames.size(); }
};

#endif