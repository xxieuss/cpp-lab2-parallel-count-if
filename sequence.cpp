#include "sequence.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <random>
#include <string>

namespace {
    constexpr unsigned kSeed = 20240401u;
    constexpr std::size_t kNumbersPerLine = 20;

    std::string file_name(std::size_t length) {
        return std::string(kDataDirectory) + "/seq_" + std::to_string(length) + ".txt";
    }

    Sequence generate_sequence(std::size_t length, unsigned seed) {
        std::mt19937 engine(seed);
        std::uniform_int_distribution<int> distribution(kMinValue, kMaxValue);
        Sequence values(length);
        for (int& value : values) {
            value = distribution(engine);
        }
        return values;
    }

    void save_sequence(const std::string& name, const Sequence& values) {
        std::ofstream file(name);
        file << values.size() << "\n";
        for (std::size_t i=0; i<values.size(); i++) {
            file << values[i];
            file << ((i+1) % kNumbersPerLine == 0 ? "\n" : " ");
        }
        file << "\n";
    }

    Sequence load_sequence(const std::string& name) {
        std::ifstream file(name);
        std::size_t length;
        file >> length;
        Sequence values(length);
        for (int& value : values) {
            file >> value;
        }
        return values;
    }
}

void generate_data_files() {
    std::filesystem::create_directories(kDataDirectory);
    unsigned seed = kSeed;
    for (std::size_t length : kLengths) {
        const std::string name = file_name(length);
        save_sequence(name, generate_sequence(length, seed++));
        std::cout << "Generated " << name << "\n";
    }
}

std::vector<Sequence> load_dataset() {
    std::vector<Sequence> dataset;
    dataset.reserve(std::size(kLengths));
    for (std::size_t length : kLengths) {
        dataset.push_back(load_sequence(file_name(length)));
    }
    return dataset;
}