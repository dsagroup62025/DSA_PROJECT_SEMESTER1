#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <chrono>
#include <iostream>
#include <iomanip>

// Format file size for display
std::string formatSize(size_t bytes);

// Format time duration
std::string formatTime(double seconds);

// Timer class for measuring performance
class Timer {
private:
    std::chrono::time_point<std::chrono::steady_clock> startTime;

public:
    Timer();
    void reset();
    double elapsed() const; // Returns seconds
};

// Progress bar for visual feedback
class ProgressBar {
private:
    size_t total;
    size_t current;
    int barWidth;
    std::chrono::time_point<std::chrono::steady_clock> startTime;

public:
    ProgressBar(size_t total, int width = 50);
    void update(size_t progress);
    void finish();

private:
    void display();
};

// Console colors (Windows)
#ifdef _WIN32
#include <windows.h>

enum class Color {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    YELLOW = 6,
    WHITE = 7,
    GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_YELLOW = 14,
    BRIGHT_WHITE = 15
};

void setConsoleColor(Color color);
void resetConsoleColor();

#else
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_CYAN    "\033[36m"
#endif

#endif 
