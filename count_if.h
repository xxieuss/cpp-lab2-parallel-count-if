#pragma once
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <thread>
#include <vector>

std::vector<std::size_t> split_bounds(std::size_t size, std::size_t parts);

template <class RandomIt, class Predicate>
std::ptrdiff_t parallel_count_if(RandomIt first, RandomIt last, Predicate predicate, std::size_t parts) {
    const std::size_t size = static_cast<std::size_t>(std::distance(first, last));
    const auto bounds = split_bounds(size, parts);
    const std::size_t actual_parts = bounds.size() - 1;

    std::vector<std::ptrdiff_t> counts(actual_parts, 0);
    std::vector<std::thread> threads;
    threads.reserve(actual_parts);

    for (std::size_t i = 0; i < actual_parts; i++) {
        const auto begin = first + static_cast<std::ptrdiff_t>(bounds[i]);
        const auto end = first + static_cast<std::ptrdiff_t>(bounds[i + 1]);
        threads.emplace_back([begin, end, predicate, &counts, i] {
            counts[i] = std::count_if(begin, end, predicate);
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return std::accumulate(counts.begin(), counts.end(), std::ptrdiff_t{0});
}