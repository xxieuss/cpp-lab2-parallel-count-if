#include "count_if.h"

std::vector<std::size_t> split_bounds(std::size_t size, std::size_t parts) {
    parts = std::max<std::size_t>(1, parts);
    if (size > 0) {
        parts = std::min(parts, size);
    } else {
        parts = 1;
    }

    const std::size_t base = size / parts;
    const std::size_t remainder = size % parts;

    std::vector<std::size_t> bounds;
    bounds.reserve(parts + 1);
    bounds.push_back(0);

    std::size_t position = 0;
    for (std::size_t i = 0; i < parts; i++) {
        position += base + (i < remainder ? 1 : 0);
        bounds.push_back(position);
    }
    return bounds;
}