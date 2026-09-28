#include "experiments.h"
#include <iomanip>
#include <string>
#include <thread>

namespace {
    constexpr int kWidth = 13;

    void print_times(const std::vector<double>& times) {
        for (double time : times) {
            std::cout << std::setw(kWidth) << time;
        }
        std::cout << '\n';
    }
}

volatile std::ptrdiff_t measurement_sink = 0;

void print_library_header() {
    std::cout << "Library count_if, ms\n";
    std::cout << std::left << std::setw(12) << "N" << std::right << std::setw(12) << "count" 
    << std::setw(kWidth) << "plain" << std::setw(kWidth) << "seq" << std::setw(kWidth) << "unseq" 
    << std::setw(kWidth) << "par" << std::setw(kWidth) << "par_unseq" << '\n';
}

void print_library_row(std::size_t length, std::ptrdiff_t count, const std::vector<double>& times) {
    std::cout << std::fixed << std::setprecision(3);
    std::cout << std::left << std::setw(12) << length << std::right << std::setw(12) << count;
    print_times(times);
}

void print_custom_header(const std::vector<Sequence>& dataset) {
    std::cout << "\nCustom parallel count_if, ms\n";
    std::cout << std::left << std::setw(6) << "K" << std::right;
    for (const auto& sequence : dataset) {
        std::cout << std::setw(kWidth) << ("N=" + std::to_string(sequence.size()));
    }
    std::cout << '\n';
}

void print_custom_row(std::size_t parts, const std::vector<double>& times) {
    std::cout << std::fixed << std::setprecision(3);
    std::cout << std::left << std::setw(6) << parts << std::right;
    print_times(times);
}

void print_custom_counts(std::size_t parts, const std::vector<std::ptrdiff_t>& counts) {
    std::cout << "\nCounts at K=" << parts << ':';
    for (std::ptrdiff_t count : counts) {
        std::cout << std::setw(kWidth) << count;
    }
    std::cout << '\n';
}

void print_best_k(const std::vector<Sequence>& dataset, const std::vector<std::vector<double>>& times) {
    const unsigned hw_threads = std::thread::hardware_concurrency();
    std::cout << "\nBest K\n";
    for (std::size_t column = 0; column < dataset.size(); ++column) {
        std::size_t best_row = 0;
        for (std::size_t row = 1; row < times.size(); ++row) {
            if (times[row][column] < times[best_row][column]) best_row = row;
        }
        const std::size_t best_k = kPartCounts[best_row];
        const double ratio = hw_threads == 0 ? 0.0 : static_cast<double>(best_k) / hw_threads;
        std::cout << "N=" << dataset[column].size() << "  K=" << best_k << "  K/HW=" << std::fixed 
        << std::setprecision(2) << ratio << '\n';
    }
}