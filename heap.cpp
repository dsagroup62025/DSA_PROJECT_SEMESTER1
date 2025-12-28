// heap.cpp
#include "heap.h"

// Constructor
Heap::Heap() {}

// Constructor from array (build heap)
Heap::Heap(const std::vector<int>& arr) : data(arr) {
    // Build min-heap from bottom up
    for (int i = static_cast<int>(data.size() / 2) - 1; i >= 0; i--) {
        heapifyDown(i);
    }
}

// Insert element into heap
void Heap::insert(int value) {
    data.push_back(value);
    heapifyUp(data.size() - 1);
}

// Extract minimum element
int Heap::extractMin() {
    if (isEmpty()) {
        throw std::runtime_error("Heap is empty");
    }

    int minValue = data[0];

    // Move last element to root
    data[0] = data.back();
    data.pop_back();

    // Restore heap property
    if (!isEmpty()) {
        heapifyDown(0);
    }

    return minValue;
}

// Peek at minimum element
int Heap::peek() const {
    if (isEmpty()) {
        throw std::runtime_error("Heap is empty");
    }
    return data[0];
}

// Heapify down (for min-heap)
void Heap::heapifyDown(int index) {
    int smallest = index;
    int left = leftChild(index);
    int right = rightChild(index);
    int heapSize = static_cast<int>(data.size());

    // Find smallest among node and its children
    if (left < heapSize && data[left] < data[smallest]) {
        smallest = left;
    }
    if (right < heapSize && data[right] < data[smallest]) {
        smallest = right;
    }

    // If smallest is not the current node, swap and recurse
    if (smallest != index) {
        std::swap(data[index], data[smallest]);
        heapifyDown(smallest);
    }
}

// Heapify up (for insertion)
void Heap::heapifyUp(int index) {
    while (index > 0) {
        int parentIdx = parent(index);

        if (data[index] < data[parentIdx]) {
            std::swap(data[index], data[parentIdx]);
            index = parentIdx;
        }
        else {
            break;
        }
    }
}

// Static heap sort function (ascending order)
void Heap::heapSort(std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());

    // Build max-heap (for ascending sort)
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapifyDownForSort(arr, n, i);
    }

    // Extract elements one by one
    for (int i = n - 1; i > 0; i--) {
        // Move current root (maximum) to end
        std::swap(arr[0], arr[i]);

        // Heapify reduced heap
        heapifyDownForSort(arr, i, 0);
    }
}

// Helper for heap sort (max-heap)
void Heap::heapifyDownForSort(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // Find largest among root and children
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    // If largest is not root
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapifyDownForSort(arr, n, largest);
    }
}