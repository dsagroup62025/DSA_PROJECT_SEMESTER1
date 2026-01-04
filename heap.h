#pragma once
#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <stdexcept>

// Min-Heap implementation for heap sort
class Heap {
private:
	std::vector<int> data;

	// Helper functions
	int parent(int i) const { return (i - 1) / 2; }
	int leftChild(int i) const { return 2 * i + 1; }
	int rightChild(int i) const { return 2 * i + 2; }

	void heapifyDown(int index);
	void heapifyUp(int index);

public:
	Heap();
	explicit Heap(const std::vector<int>& arr);
	void insert(int value);
	int extractMin();
	int peek() const;
	bool isEmpty() const { return data.empty(); }
	size_t size() const { return data.size(); }

	// Static heap sort function (ascending order)
	static void heapSort(std::vector<int>& arr);

private:
	// Static heap sort function (ascending order)
	static void  heapifyDownForSort(std::vector<int>& arr, int n, int i);
};

#endif
