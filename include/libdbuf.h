#pragma once


#include <stdint.h>
#include <stdbool.h>


#ifndef DBUF_CHUNK_SIZE
    #define DBUF_CHUNK_SIZE 10
#endif


typedef struct dbuf_s dbuf_t;


// === Standard C style library interface ===
// Creation and destruction
dbuf_t* dbuf_create(uint64_t initial_size);
void dbuf_destroy(dbuf_t* buffer);

// Normal functions
uint64_t dbuf_size(dbuf_t* buffer);
uint64_t dbuf_length(dbuf_t* buffer);
uint8_t dbuf_pop(dbuf_t* buffer);
bool dbuf_append(dbuf_t* buffer, uint8_t value);
bool dbuf_append_sequence(dbuf_t* buffer, uint8_t* sequence, uint64_t length);

// Conversion functions
uint64_t dbuf_as_bytes(dbuf_t* buffer, uint8_t* destination);
dbuf_t* dbuf_from_bytes(uint8_t* source, uint64_t length);
