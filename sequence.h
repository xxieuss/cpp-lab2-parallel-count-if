#pragma once
#include <cstddef>
#include <vector>

using Sequence = std::vector<int>;
inline constexpr int kMinValue = 0;
inline constexpr int kMaxValue = 1000000;
inline constexpr std::size_t kLengths[] = {100000, 1000000, 5000000};
inline constexpr const char* kDataDirectory = "data";

// File format: N first, then N integers separated by whitespace.
void generate_data_files();
std::vector<Sequence> load_dataset();