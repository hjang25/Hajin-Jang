/*
 * C++ implementation of file for cache simulator
 * Jiwon Moon, Hajin Jang
 */

#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include "cache_simulator.h"

/**
 * Cache_Simulator constructor that instantiates every variable.
 * 
 * @param nSets the number of sets in the cache
 * @param nSlotsPerSet the number of slots per set in the cache
 * @param nBytesPerBlock the number of bytes per block in the cache
 * @param isWriteAllocate a flag indicating whether write allocate is enabled
 * @param isWriteThrough a flag indicating whether write through is enabled
 * @param evictionType a flag indicating whether LRU or FIFO eviction is used
 */
Cache_Simulator::Cache_Simulator(unsigned int n_sets, unsigned int n_blocks_per_set, unsigned int n_bytes_per_block, 
                                bool is_write_allocate, bool is_write_through, bool eviction_type) {
    num_sets = n_sets;
    num_slots = n_blocks_per_set;
    num_bytes = n_bytes_per_block;
    num_loads = 0;
    load_hits = 0;
    load_misses = 0;
    num_stores = 0;
    store_hits = 0;
    store_misses = 0;
    num_cycles = 0;
    timer = 0;
    // Create cache structure by instantiating every variable. Vector of sets that 
    // each set contains vector of slot that each slot (struct) contains informations about
    // itself (tag, load_ts, access_ts, valid, dirty). 
    std::vector<std::vector<Slot>> allSets(num_sets);
    for (std::vector<Slot> sets : allSets) {
        std::vector<Slot> emptySet(num_slots);
        sets = emptySet;
        for (Slot& block : sets) {
            block.tag = 0;
            block.load_ts = 0;
            block.access_ts = 0;
            block.valid = false;
            block.dirty = false;
        }
    }
    cache = allSets;
    this->is_write_allocate = is_write_allocate; // false if no-write-allocate
    this->is_write_through = is_write_through; // false if write-back
    this->eviction_type = eviction_type; // true if lru, false if fifo
}


/**
 * Function to print the summary information in given format.
 */
void Cache_Simulator::print_stats() const {
    std::cout << "Total loads: " << num_loads << "\n";
    std::cout << "Total stores: " << num_stores << "\n";
    std::cout << "Load hits: " << load_hits << "\n";
    std::cout << "Load misses: " << load_misses << "\n";
    std::cout << "Store hits: " << store_hits << "\n";
    std::cout << "Store misses: " << store_misses << "\n";
    std::cout << "Total cycles: " << num_cycles << "\n";
}

/**
 * Check if the given memory address is present in the cache.
 * 
 * @param address the memory address to check
 * @return true if the memory address is found in the cache, false otherwise
 */
bool Cache_Simulator::check_cache_hit(std::string address) {
    uint32_t tag = get_tag(address); // extract tag from address
    uint32_t index = get_index(address); // extract index from address
    for (auto &slot : cache[index]) { // loop through all slots in the cache set
        if (slot.tag == tag && slot.valid) { // check if the tag matches and the slot is valid
            return true; // cache hit
        }
    }
    return false; // cache miss
}

/**
 * Load data from memory into the cache.
 * 
 * @param address the memory address to load data from
 * @return void
 */
void Cache_Simulator::load(std::string address) {
    num_loads++; // increment number of loads
    // get tag and index from address
    uint32_t tag = get_tag(address);
    uint32_t index = get_index(address);
    if (check_cache_hit(address)) { // check if cache hit
        // update access timestamp and increment load hits
        cache[index].at(find_tag(tag, index)).access_ts = timer;
        load_hits++;
        num_cycles++; // increment cycles for cache hit
    } else {
        remove(address); // evict block from cache
        Slot slot;
        slot.tag = tag; // set tag and timestamps
        slot.load_ts = timer;
        slot.access_ts = timer;
        slot.valid = true; // mark as valid
        slot.dirty = false; // mark as not dirty
        cache.at(index).push_back(slot); // add new block to cache
        load_misses++; // increment load misses
        num_cycles += 100 * num_bytes / 4; // increment cycles for cache miss
    }
}


/**
 * Removes the block associated with the given memory address from the cache if the cache is full. 
 * Uses a specific eviction policy (either Least Recently Used or Least Recently Loaded) to determine 
 * which block to evict. If the removed block is dirty and the cache is not write-through, adds additional 
 * cycles to simulate writing the block back to memory. 
 * 
 * @param address the memory address of the block to remove from the cache
 */
void Cache_Simulator::remove(std::string address) {
    uint32_t temp_tag;   // stores the tag of the block to be removed
    uint32_t index = get_index(address);   // gets the index of the block to be removed
    uint32_t min_ts = 4294967295;   // sets the minimum timestamp to the maximum possible value
    if (cache.at(index).size() < (unsigned) num_slots) {   // if the cache is not full, do nothing
        return;
    }
    // find the block with the smallest timestamp
    for (auto &slot : cache.at(index)) {
        if (eviction_type && slot.access_ts < min_ts) {   // for LRU eviction
            min_ts = slot.access_ts;
            temp_tag = slot.tag;
        } else if (!eviction_type && slot.load_ts < min_ts) {   // for FIFO eviction
            min_ts = slot.load_ts;
            temp_tag = slot.tag;
        }
    }
    // if the cache uses write-back policy and the block being removed is dirty, 
    // add additional cycles to write back to main memory
    if (!is_write_through && cache.at(index).at(find_tag(temp_tag, index)).dirty) {
        num_cycles += (100 * (num_bytes / 4));
    }
    // remove the block from the cache
    cache.at(index).erase(cache.at(index).begin() + find_tag(temp_tag, index));
}


/**
 * Stores a value at the specified memory address in the cache. Updates the cache statistics
 * accordingly, including the number of store hits and misses, and the number of cycles taken to
 * perform the store operation.
 * 
 * @param address the memory address to store the value at
 */
void Cache_Simulator::store(std::string address) {
    num_stores++;
    uint32_t tag = get_tag(address);
    uint32_t index = get_index(address);
    if(check_cache_hit(address)) { // if address is in cache
        store_hits++;
        cache[index].at(find_tag(tag, index)).access_ts = timer; // update access timestamp
        cache[index].at(find_tag(tag, index)).dirty = true; // set dirty bit
        if (is_write_through) { 
            num_cycles += 101; 
        } else {
            num_cycles++; 
        }
    } else { // if address is not in cache
        store_misses++;
        if (is_write_allocate) { // if write-allocate
            remove(address); // remove victim block from cache
            Slot slot;
            slot.tag = tag;
            slot.load_ts = timer;
            slot.access_ts = timer;
            slot.valid = true;
            slot.dirty = false;
            cache.at(index).push_back(slot); // insert new block into cache
            cache[index].at(find_tag(tag, index)).dirty = true; // set dirty bit
            num_cycles += 100 * (num_bytes / 4) + 1 ; // add cycles for cache miss and load
        }
        if (is_write_through) { 
            num_cycles += 101;
        } else {
            num_cycles++;
        }
    }
}


/**
 * This function extracts the tag bits from the given address.
 * 
 * @param address The address to extract the tag from.
 * @return The tag bits of the given address.
 */
uint32_t Cache_Simulator::get_tag(std::string address) {
    uint32_t tag = stol(address, nullptr, 16);
    uint32_t indexBits = log2(num_sets);
    uint32_t offsetBits = log2(num_bytes);
    return tag >> (indexBits + offsetBits);
}


/**
 * Calculates and returns the index of the cache block that the given memory address maps to.
 *
 * @param trace The memory address to map to an index.
 * @return The index of the cache block that the memory address maps to.
 */
uint32_t Cache_Simulator::get_index(std::string trace) {
    uint32_t address = stol(trace, nullptr, 16);
    uint32_t indexBits = log2(num_sets);
    uint32_t offsetBits = log2(num_bytes);
    uint32_t tagBits = 32U - (indexBits + offsetBits);
    // 64 bit to capture potential larger shifts
    uint64_t index = address << tagBits;
    return index >> (tagBits + offsetBits); // will drop to 32 bit
}


/**
 * Increments the timer of the cache simulator by 1.
 */
void Cache_Simulator::inc_timer() {
    timer++;
}


/**
 * Searches for the index of the slot in the cache that has the given tag in the given index.
 * 
 * @param tag the tag to search for
 * @param index the index to search in
 * @return the index of the slot if found, and 0 otherwise
*/
uint32_t Cache_Simulator::find_tag(uint32_t tag, uint32_t index) {
    std::vector<Slot> set = cache.at(index);
    // iterate through set to find index of tag
    for (int i = 0; i < (int) set.size(); i++) {
        int counter = 0;
        for (auto t : set) {
            if (t.tag == tag) {
                return counter;
            }
            counter++;
        }
    }
    return 0;
}