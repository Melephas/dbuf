#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


#ifndef DBUF_CHUNK_SIZE
    #define DBUF_CHUNK_SIZE 10
#endif

#ifdef DBUF_SERIAL_MAGIC
    #undef DBUF_SERIAL_MAGIC
#endif
#define DBUF_SERIAL_MAGIC "dbuf"


typedef struct dbuf_s dbuf;

// === Module function declarations ===
// Creation and destruction
dbuf*    dbuf_create     (uint64_t initial_size);
void     dbuf_destroy    (dbuf* buffer);

// Normal functions
uint64_t dbuf_size       (dbuf* buffer);
uint64_t dbuf_length     (dbuf* buffer);
bool     dbuf_append     (dbuf* buffer, uint8_t value);
uint8_t  dbuf_pop        (dbuf* buffer);
uint8_t* dbuf_data       (dbuf* buffer);

// Serialization and deserialization
void     dbuf_serialize  (dbuf* buffer, FILE* file);
dbuf*    dbuf_deserialize(FILE* file);
