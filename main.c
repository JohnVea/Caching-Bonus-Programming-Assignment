// John Vea
// CDA 3103C
// Bonus Programming Assignment

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

// cache info structs
typedef struct {
    int valid;
    int tag;
    int last_used;
} cache_line;

typedef struct {
    cache_line *lines;
    int num_lines;
    int num_ways;
} cache;

// constant for cache size and line
#define CACHE_SIZE 32
#define CACHE_LINE_SIZE 4


// function to simulate the cache
void simulate_cache(char* filename, cache *cache, char *replacement_policy) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int num_accesses = 0;
    int num_hits = 0;
    fp = fopen(filename, "r");
    // throws error if file was not found in current directory
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }
    // loop to read file
    while ((read = getline(&line, &len, fp)) != -1) {
        unsigned int address = strtoul(line, NULL, 16);
        int index = (address / CACHE_LINE_SIZE) % cache->num_lines;
        int tag = address / (CACHE_LINE_SIZE * cache->num_lines);
        int hit = 0;
        int i;
        for (i = 0; i < cache->num_ways; i++) {
            cache_line *line = &cache->lines[index * cache->num_ways + i];
            if (line->valid && line->tag == tag) {
                hit = 1;
                line->last_used = num_accesses;
                break;
            }
        }
        if (hit) {
            num_hits++;
        } else {
            int min_index = 0;
            int min_last_used = cache->lines[index * cache->num_ways].last_used;
            for (i = 1; i < cache->num_ways; i++) {
                int curr_last_used = cache->lines[index * cache->num_ways + i].last_used;
                if (curr_last_used < min_last_used) {
                    min_index = i;
                    min_last_used = curr_last_used;
                }
            }
            cache_line *line = &cache->lines[index * cache->num_ways + min_index];
            line->valid = 1;
            line->tag = tag;
            line->last_used = num_accesses;
        }
        num_accesses++;
    }
    fclose(fp);
    if (line) {
        free(line);
    }
    double hit_rate = (double) num_hits / (double) num_accesses;
    // print out the information regarding the cache size, number of hits and total access anf hit rate
    printf("Cache size: %d, %d-way associative, %s replacement policy\n", cache->num_lines * CACHE_LINE_SIZE,
           cache->num_ways, replacement_policy);
    printf("Number of hits: %d\n", num_hits);
    printf("Number of total accesses: %d\n", num_accesses);
    printf("Hit rate: %f\n", hit_rate);
}



int main() {
    // loops through all 4 caching and calls simulate cache function on all counts
    for(int i = 0; i < 4; i++){
        if(i == 0){
            cache direct_mapped_cache;
            direct_mapped_cache.num_lines = CACHE_SIZE / CACHE_LINE_SIZE;
            direct_mapped_cache.num_ways = 1;
            direct_mapped_cache.lines = malloc(direct_mapped_cache.num_lines * direct_mapped_cache.num_ways * sizeof(cache_line));
            simulate_cache("traces.txt", &direct_mapped_cache, "LRU");
            free(direct_mapped_cache.lines);
        }
        if(i == 1){
            cache two_way_cache;
            two_way_cache.num_lines = CACHE_SIZE / CACHE_LINE_SIZE / 2;
            two_way_cache.num_ways = 2;
            two_way_cache.lines = malloc(two_way_cache.num_lines * two_way_cache.num_ways * sizeof(cache_line));
            simulate_cache("traces.txt", &two_way_cache, "LRU");
            free(two_way_cache.lines);
        }
        if(i == 2){
            cache four_way_cache;
            four_way_cache.num_lines = CACHE_SIZE / CACHE_LINE_SIZE / 4;
            four_way_cache.num_ways = 4;
            four_way_cache.lines = malloc(four_way_cache.num_lines * four_way_cache.num_ways * sizeof(cache_line));
            simulate_cache("traces.txt", &four_way_cache, "LRU");
            free(four_way_cache.lines);
        }
        if(i == 3){
            cache fully_assoc_cache;
            fully_assoc_cache.num_lines = 1;
            fully_assoc_cache.num_ways = CACHE_SIZE / CACHE_LINE_SIZE;
            fully_assoc_cache.lines = malloc(fully_assoc_cache.num_lines * fully_assoc_cache.num_ways * sizeof(cache_line));
            simulate_cache("traces.txt", &fully_assoc_cache, "LRU");
            free(fully_assoc_cache.lines);
        }
    }

    return 0;
}

