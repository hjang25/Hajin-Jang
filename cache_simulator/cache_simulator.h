/*
 * h file for cache simulator
 * Jiwon Moon, Hajin Jang
 */

#ifndef CACHE_SIMULATOR
#define CACHE_SIMULATOR

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

struct Slot {
    uint32_t tag;
    uint32_t load_ts;
    uint32_t access_ts;
    bool valid;
    bool dirty;
};

class Cache_Simulator {
private:
    int num_sets, num_slots, num_bytes, num_loads, load_hits, load_misses, num_stores,
        store_hits, store_misses, num_cycles, timer;
    std::vector<std::vector<Slot>> cache;
    bool is_write_allocate, is_write_through, eviction_type;

public:
    /**
     * Cache_Simulator constructor that instantiates every variable.
     * 
     * @param n_sets the number of sets in the cache
     * @param n_blocks_per_set the number of blocks/slots per set in the cache
     * @param n_bytes_per_block the number of bytes per block in the cache
     * @param is_write_allocate a flag indicating whether write allocate is enabled
     * @param is_write_through a flag indicating whether write through is enabled
     * @param eviction_type a flag indicating whether LRU or FIFO eviction is used
     */
    Cache_Simulator(unsigned int n_sets, unsigned int n_blocks_per_set, unsigned int n_bytes_per_block, 
                    bool is_write_allocate, bool is_write_through, bool eviction_type);
    
    /**
     * Function to print the summary information in given format.
     */
    void print_stats() const;

    /**
     * Check if the given memory address is present in the cache.
     * 
     * @param address the memory address to check
     * @return true if the memory address is found in the cache, false otherwise
     */
    bool check_cache_hit(std::string address);

    /**
     * Load data from memory into the cache.
     * 
     * @param address the memory address to load data from
     * @return void
     */
    void load(std::string address);

    /**
     * Removes the block associated with the given memory address from the cache if the cache is full. 
     * Uses a specific eviction policy (either Least Recently Used or Least Recently Loaded) to determine 
     * which block to evict. If the removed block is dirty and the cache is not write-through, adds additional 
     * cycles to simulate writing the block back to memory. 
     * 
     * @param address the memory address of the block to remove from the cache
     */
    void remove(std::string address);

    /**
     * Stores a value at the specified memory address in the cache. Updates the cache statistics
     * accordingly, including the number of store hits and misses, and the number of cycles taken to
     * perform the store operation.
     * 
     * @param address the memory address to store the value at
     */
    void store(std::string address);

    /**
     * This function extracts the tag bits from the given address.
     * 
     * @param address The address to extract the tag from.
     * @return The tag bits of the given address.
     */
    uint32_t get_tag(std::string address);

    /**
     * Calculates and returns the index of the cache block that the given memory address maps to.
     *
     * @param trace The memory address to map to an index.
     * @return The index of the cache block that the memory address maps to.
     */
    uint32_t get_index(std::string trace);

    /**
     * Searches for the index of the slot in the cache that has the given tag in the given index.
     * 
     * @param tag the tag to search for
     * @param index the index to search in
     * @return the index of the slot if found, and 0 otherwise
    */
    uint32_t find_tag(uint32_t trace, uint32_t index);

    /**
     * Increments the timer of the cache simulator by 1.
     */
    void inc_timer();
};

#endif //CACHE_SIMULATOR