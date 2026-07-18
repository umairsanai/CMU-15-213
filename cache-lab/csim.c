#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "cachelab.h"

struct Line {
    unsigned long long tag;
    unsigned long long timestamp;
};

int getSetIndex(unsigned long long address, unsigned long long sets_count, int block_bits) {
    return (address >> block_bits) % sets_count;
}

int getTag(unsigned long long address, unsigned int set_bits, unsigned int block_bits) {
    return address >> (block_bits + set_bits);
}

int getLRULineIndex(struct Line* set, unsigned int size) {
    if (size == 0) return -1;

    int lru_index = 0;
    for (unsigned int i = 0; i < size; i++) {
        if (set[i].timestamp == 0)
            return i;
        if (set[i].timestamp < set[lru_index].timestamp)
            lru_index = i;
    }
    return lru_index;
}

int getMaxTimestamp(struct Line* set, unsigned int size) {
    unsigned long long max_timestamp = 0;
    for (unsigned int i = 0; i < size; i++) {
        max_timestamp =  max_timestamp > set[i].timestamp ? max_timestamp : set[i].timestamp;
    }
    return max_timestamp;
}

int present_in_cache(struct Line* set, unsigned int size, unsigned long long address, unsigned int set_bits, unsigned int block_bits) {
    for (unsigned int i = 0; i < size; i++) {
        if (set[i].tag == getTag(address, set_bits, block_bits) && set[i].timestamp != 0) {
            return i;
        }
    }
    return -1;
}

void load(struct Line* set, unsigned int associativity, unsigned long long address, unsigned int set_bits, unsigned int block_bits, int* hits, int* misses, int* evictions) {
    int present_index = present_in_cache(set, associativity, address, set_bits, block_bits);
    if (present_index >= 0) {
        *hits = *hits + 1;
        set[present_index].timestamp = getMaxTimestamp(set, associativity) + 1;
    } else {
        *misses = *misses + 1;
        int lru_index = getLRULineIndex(set, associativity);
        if (set[lru_index].timestamp != 0) {
            *evictions = *evictions + 1;
        }
        set[lru_index].timestamp = getMaxTimestamp(set, associativity) + 1;
        set[lru_index].tag = getTag(address, set_bits, block_bits);
    }
}
void store(struct Line* set, int associativity, unsigned long long address, unsigned int set_bits, unsigned int block_bits, int* hits, int* misses, int* evictions) {
    load(set, associativity, address, set_bits, block_bits, hits, misses, evictions);
}





int main(int argc, char* argv[]) {

    int flag;
    unsigned int set_bits, block_bits, associativit_number, sets_count;
    FILE* file = NULL;

    int hits = 0, misses = 0, evictions = 0;

    while ((flag = getopt(argc, argv, "s:E:b:t:")) != -1) {
        switch (flag) {
            case 's':
                set_bits = atoi(optarg);
                sets_count = (1 << set_bits);
                break;
            case 'E':
                associativit_number = atoi(optarg);
                break;
            case 'b':
                block_bits = atoi(optarg);
                break;
            case 't':
                file = fopen(optarg, "r");
                break;
            default:
                fprintf(stderr, "Usage: %s -s <s> -E <E> -b <b> -t <tracefile>\n", argv[0]);
                exit(1);
        }
    }

    if (!file) {
        printf("Error! File cannot be opened.");
        exit(2);
    }

    struct Line** cache = malloc(sizeof(struct Set*) * sets_count);

    for (int i = 0; i < sets_count; i++) {
        cache[i] = malloc(sizeof(struct Line) * associativit_number);

        // If some cache set doesn't get memory allocated, free previously allocated memory and exit the program. 
        if (!cache[i]) {
            for (int k = 0; k < i; i++) {
                free(cache[k]);
            }
            free(cache);
            exit(3);
        }

        for (int j = 0; j < associativit_number; j++) {
            cache[i][j].timestamp = 0;
        }
    }


    char operation;
    int size_operated;
    unsigned long long address;
    while (fscanf(file, " %c %llx,%d", &operation, &address, &size_operated) == 3) {
        if (operation == 'I') continue;

        int set_index = getSetIndex(address, sets_count, block_bits);
        if (set_index < 0) exit(4);
        struct Line* set = cache[set_index];
        
        if (operation == 'L') {
            load(set, associativit_number, address, set_bits, block_bits, &hits, &misses, &evictions);
        } else if (operation == 'S') {
            store(set, associativit_number, address, set_bits, block_bits, &hits, &misses,  &evictions);
        } else if (operation == 'M') {
            load(set, associativit_number, address, set_bits, block_bits, &hits, &misses, &evictions);
            store(set, associativit_number, address, set_bits, block_bits, &hits, &misses,  &evictions);
        }
    }

    // FREE MEMORY AT THE END OF THE PROGRAM
    for (int i = 0; i < sets_count; i++) {
        if (cache[i]) free(cache[i]);
        cache[i] = NULL;
    }    
    free(cache);
    fclose(file);
    printSummary(hits, misses, evictions);
    return 0;
}