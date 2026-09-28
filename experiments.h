#pragma once
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <execution>
#include <iostream>
#include <iterator>
#include <limits>
#include <string_view>
#include <vector>
#include "count_if.h"
#include "sequence.h"

inline constexpr int kRepeats = 3;
inline constexpr std::size_t kPartCounts[] = {1, 2, 3, 4, 6, 8, 12, 16, 24, 32, 48, 64};

struct Measurement {
    double ms;
    std::ptrdiff_t count;
};

extern volatile std::ptrdiff_t measurement_sink;

void print_library_header();
void print_library_row(std::size_t length, std::ptrdiff_t count, const std::vector<double>& times);
void print_custom_header(const std::vector<Sequence>& dataset);
void print_custom_row(std::size_t parts, const std::vector<double>& times);
void print_best_k(const std::vector<Sequence>& dataset, const std::vector<std::vector<double>>& times);
void print_custom_counts(std::size_t parts, const std::vector<std::ptrdiff_t>& counts);

template <class Callable>
Measurement measure(Callable&& callable) {
    double best_ms = std::numeric_limits<double>::max();
    std::ptrdiff_t count = 0;
    for (int i = 0; i < kRepeats; ++i) {
        const auto start = std::chrono::steady_clock::now();
        count = callable();
        measurement_sink = count;
        const auto end = std::chrono::steady_clock::now();
        const double ms = std::chrono::duration<double, std::milli>(end - start).count();
        best_ms = std::min(best_ms, ms);
    }
    return {best_ms, count};
}

template <class Predicate>
void run_library_experiments(const std::vector<Sequence>& dataset, Predicate predicate) {
    print_library_header();
    for (const auto& sequence : dataset) {
        const auto first = sequence.begin();
        const auto last = sequence.end();
        const auto plain = measure([&] { return std::count_if(first, last, predicate); });
        const auto seq = measure([&] { return std::count_if(std::execution::seq, first, last, predicate); });
        const auto unseq = measure([&] { return std::count_if(std::execution::unseq, first, last, predicate); });
        const auto par = measure([&] { return std::count_if(std::execution::par, first, last, predicate); });
        const auto par_unseq = measure([&] { return std::count_if(std::execution::par_unseq, first, last, predicate); });
        print_library_row(sequence.size(), plain.count, {plain.ms, seq.ms, unseq.ms, par.ms, par_unseq.ms});
    }
}

template <class Predicate>
void run_custom_experiments(const std::vector<Sequence>& dataset, Predicate predicate) {
    print_custom_header(dataset);

    std::vector<std::vector<double>> all_times;
    all_times.reserve(std::size(kPartCounts));

    std::vector<std::ptrdiff_t> counts;

    for (std::size_t parts : kPartCounts) {
        std::vector<double> row;
        row.reserve(dataset.size());

        counts.clear();

        for (const auto& sequence : dataset) {
            const auto result = measure([&] {
                return parallel_count_if(sequence.begin(), sequence.end(), predicate, parts);
            });

            row.push_back(result.ms);
            counts.push_back(result.count);
        }

        print_custom_row(parts, row);
        all_times.push_back(row);
    }

    print_custom_counts(kPartCounts[std::size(kPartCounts) - 1], counts);
    print_best_k(dataset, all_times);
}

template <class Predicate>
void run_all_experiments(std::string_view name, const std::vector<Sequence>& dataset, Predicate predicate) {
    std::cout << "\nPredicate: " << name << '\n';
    run_library_experiments(dataset, predicate);
    run_custom_experiments(dataset, predicate);
}