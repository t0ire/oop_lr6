#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>

#include "observer.h"

FileObserver::FileObserver(const std::string& filename) 
    : filename(filename) {
    std::ofstream clearFile(filename);
}

void FileObserver::update(const std::string& message) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        file << "[" << std::put_time(std::localtime(&time), "%H:%M:%S") << "] " 
             << message << std::endl;
        file.close();
    }
}

void ConsoleObserver::update(const std::string& message) {
    std::cout << "ок" << message << std::endl;
}