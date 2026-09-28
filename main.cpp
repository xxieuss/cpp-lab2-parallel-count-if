// Compiler: g++-16 (Homebrew GCC 16.2.0), C++20.
#include <iostream>
#include <string_view>
#include <thread>
#include "experiments.h"
#include "predicates.h"
#include "sequence.h"

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string_view(argv[1]) == "--generate") {
        generate_data_files();
        return 0;
    }

    const auto dataset = load_dataset();

    std::cout << "count_if benchmark\n";
    std::cout << "HW threads: " << std::thread::hardware_concurrency() << '\n';
    std::cout << "Repeats: " << kRepeats << " (best time)\n";

    run_all_experiments("even", dataset, IsEven{});
    run_all_experiments("prime", dataset, IsPrime{});
}