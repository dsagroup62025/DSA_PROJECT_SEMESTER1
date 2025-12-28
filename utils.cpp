// utils.cpp
#include "utils.h"
#include <sstream>
#include <cmath>

// Format bytes to human-readable size
std::string formatSize(size_t bytes) {
    const char* units[] = { "B", "KB", "MB", "GB", "TB" };
    int unitIndex = 0;
    double size = static_cast<double>(bytes);

    while (size >= 1024.0 && unitIndex < 4) {
        size /= 1024.0;
        unitIndex++;
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << size << " " << units[unitIndex];
    return oss.str();
}

// Format seconds to readable time
std::string formatTime(double seconds) {
    if (seconds < 60) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << seconds << "s";
        return oss.str();
    }
    else if (seconds < 3600) {
        int mins = static_cast<int>(seconds / 60);
        int secs = static_cast<int>(seconds) % 60;
        return std::to_string(mins) + "m " + std::to_string(secs) + "s";
    }
    else {
        int hours = static_cast<int>(seconds / 3600);
        int mins = static_cast<int>(seconds / 60) % 60;
        return std::to_string(hours) + "h " + std::to_string(mins) + "m";
    }
}

// Timer implementation
Timer::Timer() {
    startTime = std::chrono::steady_clock::now();
}

void Timer::reset() {
    startTime = std::chrono::steady_clock::now();
}

double Timer::elapsed() const {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
    return duration.count() / 1000.0;
}

// ProgressBar implementation
ProgressBar::ProgressBar(size_t total, int width)
    : total(total), current(0), barWidth(width) {
    startTime = std::chrono::steady_clock::now();
}

void ProgressBar::update(size_t progress) {
    current = progress;
    display();
}

void ProgressBar::display() {
    float percentage = (total > 0) ? (static_cast<float>(current) / total) : 0.0f;

    // Calculate speed and ETA
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();

    double speed = (elapsed > 0) ? (static_cast<double>(current) / elapsed) : 0.0;
    int eta = (speed > 0 && current < total) ?
        static_cast<int>((total - current) / speed) : 0;

    // Draw progress bar
    std::cout << "\r[";
    int pos = static_cast<int>(barWidth * percentage);
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }

    std::cout << "] " << std::setw(3) << static_cast<int>(percentage * 100) << "% ";

    // Show speed if available
    if (speed > 0) {
        std::cout << "| " << formatSize(static_cast<size_t>(speed)) << "/s ";
    }

    // Show ETA if available
    if (eta > 0 && current < total) {
        std::cout << "| ETA: " << formatTime(eta);
    }

    std::cout << std::flush;
}

void ProgressBar::finish() {
    current = total;
    display();
    std::cout << "\n";
}

// Console color functions
#ifdef _WIN32
void setConsoleColor(Color color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));
}

void resetConsoleColor() {
    setConsoleColor(Color::WHITE);
}
#endif