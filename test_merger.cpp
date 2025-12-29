#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "merger.h"
#include "chunker.h"
#include "utils.h"

//Sorted chunk fils for testing
std::vector<std::string> generateTestChunks(int numChunks, int elementsPerChunk) {
	std::cout << "Generating " << numChunks << " test chunks...\n";
	std::vector<std::string> chunkFiles;

	for (int i = 0; i < numChunks; i++) {
		std::string filename = "test_merge_chunk_" + std::to_string(i) + ".txt";
		std::ofstream out(filename);

		//Generate sorted integers for this chunk
		int start = i * elementsPerChunk;
		for (int j = 0; j < elementsPerChunk; j++) {
			int value = i + j + numChunks;
			out << value << "\n";
		}
		out.close();
		chunkFiles.push_back(filename);
		std::cout<< " Created: " << filename << " ( " << elementsPerChunk << " elments)\n";
	}
	std::cout << "\n";
	return chunkFiles;
}
//Verify output is sorted
bool verifyOutputSorted(const std::string& filename, size_t expectedCount) {
	std::ifstream in(filename);
	if (!in.is_open()) {
		std::cout << " Cannot open output file : " << filename << "\n";
		return false;
	}

	int prev, curr;
	if (!(in >> prev)) {
		std::cout << " Empty output file \n";
		return expectedCount == 0;
	}

	size_t count = 1;

	while (in >> curr) {
		if (curr < prev) {
			std::cout << " NOT SORTED at position " << count << ": "
				<< prev << " > " << curr << "\n";
			return false;
		}
		prev = curr;
		count++;
	}

	if (count != expectedCount) {
		std::cout << " Element count mismatch : got " << count
			<< ", expected " << expectedCount << "\n";
		return false;
 	}
	std::cout << " Output is correctly sorted (" << count << " elements)\n";
	return true;
}

//cleanup test file
void cleanupTestFiles(const std::vector<std::string> & files) {
	for (const auto& file : files) {
		std::remove(file.c_str());
	}
}

//Test 1: small merge ( 3 chunks)
void testSmallMerge() {
	std::cout << " *** Test 1 : Small Merge ( 3 chunks * 100 elements)***\n\n";

	auto chunks = generateTestChunks(3, 100);

	Merger merger(chunks, "test_output_small.txt");
	Timer timer;
	merger.merge();
	
	std::cout << " Merge time :" << formatTime(timer.elapsed()) << "\n\n";

	//Verify
	std::cout << " Verifying output...\n";
	bool correct = verifyOutputSorted("test_ouput_small.txt", 300);

	if (correct) {
		std::cout << " TEST 1 PASSED\n";
	}
	else {
		std::cout << " TEST 1 FAILED\n";
	}

	//Cleanup
    cleanupTestFiles(chunks);
    std::remove("test_output_small.txt");

    std::cout << "\n***************************************\n\n";
}

// Test 2: Medium merge (10 chunks)
void testMediumMerge() {
    std::cout << "***Test 2: Medium Merge (10 chunks × 10K elements)***\n\n";

    auto chunks = generateTestChunks(10, 10000);

    Merger merger(chunks, "test_output_medium.txt");
    Timer timer;
    merger.merge();

    std::cout << "Merge time: " << formatTime(timer.elapsed()) << "\n\n";

    // Verify
    std::cout << "Verifying output...\n";
    bool correct = verifyOutputSorted("test_output_medium.txt", 100000);

    if (correct) {
        std::cout << " Test 2 PASSED\n";
    }
    else {
        std::cout << " Test 2 FAILED\n";
    }

    // Cleanup
    cleanupTestFiles(chunks);
    std::remove("test_output_medium.txt");

    std::cout << "\n*****************************************\n\n";
}

// Test 3: Integration test (Chunker + Merger)
void testIntegration() {
    std::cout << "***Test 3: Integration Test (Chunker -> Merger)***\n\n";

    // Generate unsorted test file
    std::cout << "Generating 50K random integers...\n";
    std::ofstream testInput("test_integration_input.txt");
    std::vector<int> expected;

    for (int i = 0; i < 50000; i++) {
        int value = rand() % 100000 - 50000;  // -50000 to 50000
        testInput << value << "\n";
        expected.push_back(value);
    }
    testInput.close();

    // Sort expected for verification
    std::sort(expected.begin(), expected.end());

    std::cout << "\nStep 1: Chunking...\n";
    Chunker chunker("test_integration_input.txt", 50000);  // ~12K ints per chunk
    auto chunks = chunker.createSortedChunks();

    std::cout << "Step 2: Merging...\n";
    Merger merger(chunks, "test_integration_output.txt");
    merger.merge();

    std::cout << "Step 3: Verification...\n";

    // Verify output matches expected
    std::ifstream output("test_integration_output.txt");
    bool allMatch = true;
    size_t count = 0;
    int value;

    while (output >> value) {
        if (count >= expected.size() || value != expected[count]) {
            std::cout << " Mismatch at position " << count << "\n";
            allMatch = false;
            break;
        }
        count++;
    }

    if (allMatch && count == expected.size()) {
        std::cout << " Integration test PASSED\n";
        std::cout << "  Full pipeline works correctly!\n";
    }
    else {
        std::cout << " Integration test FAILED\n";
    }

    // Cleanup
    chunker.cleanupTempFiles();
    std::remove("test_integration_input.txt");
    std::remove("test_integration_output.txt");

    std::cout << "\n****************************************\n\n";
}

// Test 4: Edge cases
void testEdgeCases() {
    std::cout << "***Test 4: Edge Cases***\n\n";

    // Single chunk
    {
        std::cout << "Testing single chunk...\n";
        auto chunks = generateTestChunks(1, 100);

        Merger merger(chunks, "test_single_chunk.txt");
        merger.merge();

        bool correct = verifyOutputSorted("test_single_chunk.txt", 100);
        std::cout << (correct ? "YES" : "NO") << " Single chunk test\n\n";

        cleanupTestFiles(chunks);
        std::remove("test_single_chunk.txt");
    }

    // Many small chunks
    {
        std::cout << "Testing many small chunks (20 chunks * 50 elements)...\n";
        auto chunks = generateTestChunks(20, 50);

        Merger merger(chunks, "test_many_chunks.txt");
        merger.merge();

        bool correct = verifyOutputSorted("test_many_chunks.txt", 1000);
        std::cout << (correct ? "YES" : "NO") << " Many small chunks test\n\n";

        cleanupTestFiles(chunks);
        std::remove("test_many_chunks.txt");
    }

    std::cout << "******************************************\n\n";
}

int main() {
    std::cout << "******************************************\n";
    std::cout << "*            MERGER TEST SUITE           *\n";
    std::cout << "******************************************\n\n";

    testSmallMerge();
    testMediumMerge();
    testIntegration();
    testEdgeCases();

    std::cout << "******************************************\n";
    std::cout << "*            ALL TESTS COMPLETE          *\n";
    std::cout << "******************************************\n";

    return 0;
}