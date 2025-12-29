#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: verify_sorted <filename>\n";
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream in(filename);

    if (!in.is_open()) {
        std::cerr << "Cannot open file: " << filename << "\n";
        return 1;
    }

    int prev, curr;
    if (!(in >> prev)) {
        std::cout << "Empty file\n";
        return 0;
    }

    size_t count = 1;
    bool sorted = true;

    while (in >> curr) {
        if (curr < prev) {
            std::cout << " NOT SORTED at position " << count << "\n";
            std::cout << "  Previous: " << prev << ", Current: " << curr << "\n";
            sorted = false;
            break;
        }
        prev = curr;
        count++;

        if (count % 1000000 == 0) {
            std::cout << "\rVerified " << (count / 1000000) << "M elements..." << std::flush;
        }
    }

    if (sorted) {
        std::cout << "\r File is correctly sorted!                    \n";
        std::cout << "  Total elements: " << count << "\n";

        // Show first and last few
        in.clear();
        in.seekg(0);
        std::cout << "\nFirst 5 elements:\n";
        for (int i = 0; i < 5 && in >> curr; i++) {
            std::cout << "  " << curr << "\n";
        }
    }

    return sorted ? 0 : 1;
}