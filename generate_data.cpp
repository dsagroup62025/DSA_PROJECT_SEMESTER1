#include <iostream>
#include <fstream>
#include <random>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: generate_data <count> <filename>\n";
        std::cout << "Example: generate_data 1000000 input.txt\n";
        return 1;
    }

    size_t count = std::stoull(argv[1]);
    std::string filename = argv[2];

    std::cout << "Generating " << count << " random integers...\n";

    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Cannot create file: " << filename << "\n";
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-1000000, 1000000);

    for (size_t i = 0; i < count; i++) {
        out << dis(gen) << "\n";

        if (i % 100000 == 0) {
            std::cout << "\r" << (i * 100 / count) << "%" << std::flush;
        }
    }

    std::cout << "\r100%\n";
    std::cout << " File created: " << filename << "\n";

    return 0;
}