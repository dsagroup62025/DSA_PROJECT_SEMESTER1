#ifndef CHUNKER_H
#define CHUNKER_H

#include <string>
#include <vector>
#include <fstream>

// Chunker : Splits large file into sorted chunks
class Chunker {
private :
	std::string inputFilename;
	size_t chunkSizeBytes;   //Max bytes per chunk
	size_t maxIntegers;    //Max integers perchunk
	int chunkCount;

	//Helper functions
	std::string getTempFilename(int index) const;
	void sortAndWriteChunk(std::vector<int>& chunk, int index);

public:
	//Construtor
	Chunker(const std::string& filenaem, size_t chunkSize = 512 * 1024 * 1024);

	//Main operation: create sorted chunks
	std::vector<std::string> createSortedChunks();

	//Getters
	int getChunkCount() const { return chunkCount;  }
	size_t getChunkSize() const { return chunkSizeBytes; }

	//Cleanup temp files
	void cleanupTempFiles();
};

#endif  