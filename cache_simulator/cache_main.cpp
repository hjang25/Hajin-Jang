/*
 * C++ implementation of cache calculator main function
 * Jiwon Moon, Hajin Jang
 */

#include <iostream>
#include "cache_simulator.h"
#include <string>

/*
 * Helper function that determine if an integer is a positive power of 2
 *
 * Parameters:
 *   x - an integer value to be determined
 *
 * Returns:
 *   boolean value, true if x is a power of 2, false if not
 *
 */
bool is_power_of_2(int x) {
    return (x > 0) && ((x & (x - 1)) == 0);
}

/**
 * Helper function that reads input from standard input and returns
 * a vector of pairs containing the processor command and memory address.
 * Each pair consists of a character representing the processor 
 * command (either 'l' for load or 's' for store) and a string representing 
 * the memory address to be accessed.
 * 
 * @return a vector of pairs containing processor commands and memory addresses.
*/
std::vector<std::pair<char, std::string>> get_input() {
    std::vector<std::pair<char, std::string>> input;
    char processor;
    std::string address;
    std::string offset;

    while (std::cin >> processor && std::cin >> address) {
        std::cin >> offset; // ignore the third field
        auto temp = std::make_pair(processor, address);
        input.emplace_back(temp);
    }
    
    return input;
}

/**
 * Main function to run the cache simulator.
 * 
 * @param argc number of command line arguments
 * @param argv array of strings containing command line arguments
 * @return 0 if program executed successfully
 */
int main(int argc, char *argv[]) {
    // Check if number of arguments is valid, if not print corresponding error message.
    if (argc != 7) {
        std::cerr << "Error: invalid number of arguments";
        std::exit(EXIT_FAILURE);
    }

    // Store argv values
    const int n_sets = std::atoi(argv[1]);
    const int n_blocks_per_set = std::atoi(argv[2]);
    const int n_bytes_per_block = std::atoi(argv[3]);
    const std::string write_allocate = argv[4];
    const std::string write_through = argv[5];
    const std::string eviction = argv[6];

    bool is_write_allocate = true; // true if write-allocate, false if not
    bool is_write_through = true; // true if write-through, false if not
    bool is_eviction = true; // true if lru, false if not

    // Check the configuration parameters, and print corresponding error message.
    // Check if the number of set is a positive power of 2.
    if (!is_power_of_2(n_sets)) {
        std::cerr << "Error: number of sets must be a positive power of 2.\n";
        std::exit(EXIT_FAILURE);
    }

    // Check if the number of blocks in each set is a positive power of 2.
    if (!is_power_of_2(n_blocks_per_set)) {
        std::cerr << "Error: number of blocks in each set must be a positive power of 2.\n";
        std::exit(EXIT_FAILURE);
    }

    // Check if the block size is a positive power of 2 and at least 4.
    if (!is_power_of_2(n_bytes_per_block) || n_bytes_per_block < 4) {
        std::cerr << "Error: block size must be a positive power of 2 and at least 4.\n";
        std::exit(EXIT_FAILURE);
    }

    // Check if the block size is a positive power of 2 and at least 4.
    if (!(write_allocate == "write-allocate" || write_allocate == "no-write-allocate")) {
        std::cerr << "Error: invalid write allocate policy.\n";
        std::exit(EXIT_FAILURE);
    } else if (write_allocate == "no-write-allocate") {
        is_write_allocate = false;
    }

    // Check if write policy is valid
    if (!(write_through == "write-through" || write_through == "write-back")) {
        std::cerr << "Error: invalid write policy.\n";
        std::exit(EXIT_FAILURE);
    } else if (write_through == "write-back") {
        is_write_through = false;
    }

    // Check if eviction policy is valid
    if (!(eviction == "lru" || eviction == "fifo")) {
        std::cerr << "Error: invalid eviction policy.\n";
        std::exit(EXIT_FAILURE);
    } else if (eviction == "fifo") {
        is_eviction = false;
    }

    // Check if no-write-allocate and write-back were both specified
    if (!is_write_allocate && !is_write_through) {
        std::cerr << "Error: invalid combination of write policies.\n";
        std::exit(EXIT_FAILURE);
    }

    // store processor status and traces
    std::vector<std::pair<char, std::string>> input = get_input();

    // Create cache object
    Cache_Simulator cache_simlator(n_sets, n_blocks_per_set, n_bytes_per_block, is_write_allocate, is_write_through, is_eviction);

    for (auto &line : input) { // process each line from input
        if (line.first == 'l') { // 'l' for loading from memory
            cache_simlator.load(line.second);
        } else { // 's' for storing to memory
            cache_simlator.store(line.second);
        }
        cache_simlator.inc_timer();
    }
    // print summary information for the cache object
    cache_simlator.print_stats();
    return 0;
}