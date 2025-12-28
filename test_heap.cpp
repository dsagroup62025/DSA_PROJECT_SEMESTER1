#include <iostream>
#include <vector>
#include <algorithm>
#include "heap.h"

void testHeapBasic() {
	std::cout << "Testing basic heap opearions...\n";

	Heap h;
	h.insert(5);
	h.insert(3);
	h.insert(7);
	h.insert(1);

	std::cout << "Min: " << h.peek() << " (expected: 1)\n";
	std::cout << "Extract: " << h.extractMin() << " (expected: 1)\n";
	std::cout << "Extract: " << h.extractMin() << " (expected: 3)\n";
	std::cout << "Extract: " << h.extractMin() << " (expected: 5)\n";
	std::cout << "Extract: " << h.extractMin() << " (expected: 7)\n";
}

void testHeapSort() {
	std::cout << "\nTesting heap sort...\n";

	std::vector<int> arr = { 64, 34, 25, 12, 22, 11, 90 };
	std::cout << " Original :";
	for (int x : arr) std::cout << x << " ";
	std::cout << "\n";

	Heap::heapSort(arr);

	std::cout << "Sorted:  ";
	for (int x : arr) std::cout << x << " ";
	std::cout << "\n";

	bool sorted = std::is_sorted(arr.begin(), arr.end());
	std::cout << " Is Sorted :" << (sorted ? " YES " : " NO ") << "\n";
}

void testHeapSortLarge() {
	std::cout << "\nTesting heap sort with 10000 random numbers...\n";

	std::vector<int> arr(10000);
	for (int i = 0; i < 10000; i++) {
		arr[i] = rand() % 100000;
	}

	Heap::heapSort(arr);
	bool sorted = std::is_sorted(arr.begin(), arr.end());
	std::cout << " Is Sorted :" << (sorted ? "YES " : "NO") << "\n";
}

int main() {
	std::cout << "***Heap Sort Test Suite***\n\n";

	testHeapBasic();
	testHeapSort();
	testHeapSortLarge();

	std::cout << "\n***All tests complete***\n";
	return 0;
}