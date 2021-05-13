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

typedef struct {
    // Creation and destruction
    dbuf*    (*create)     (uint64_t initial_size);
    void     (*destroy)    (dbuf* buffer);

    // Normal functions
    uint64_t (*size)       (dbuf* buffer);
    uint64_t (*length)     (dbuf* buffer);
    bool     (*append)     (dbuf* buffer, uint8_t value);
    uint8_t  (*pop)        (dbuf* buffer);
    uint8_t* (*data)       (dbuf* buffer);

    // Serialization and deserialization
    void     (*serialize)  (dbuf* buffer, FILE* file);
    dbuf*    (*deserialize)(FILE* file);
} dbuf_m;

dbuf_m* dbuf_init_module();
void    dbuf_destroy_module(dbuf_m* module);
