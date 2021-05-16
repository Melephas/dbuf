#include "libdbuf.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>


struct dbuf_s {
    uint64_t allocated_size;
    uint64_t length_filled;
    uint8_t* data;
};


// === Module function definitions ===
// Creation and destruction
dbuf* dbuf_create(uint64_t initial_size) {
    dbuf* ret = malloc(sizeof(struct dbuf_s));
    ret->allocated_size = initial_size;
    ret->length_filled = 0;
    ret->data = malloc(initial_size);

    return ret;
}

void dbuf_destroy(dbuf* buffer) {
    free(buffer->data);
    free(buffer);
}


// Normal functions
uint64_t dbuf_size(dbuf* buffer) {
    return buffer->allocated_size;
}

uint64_t dbuf_length(dbuf* buffer) {
    return buffer->length_filled;
}

bool dbuf_append(dbuf* buffer, uint8_t value) {
    buffer->data[buffer->length_filled] = value;
    buffer->length_filled++;

    if (buffer->length_filled == buffer->allocated_size) {
        buffer->allocated_size += DBUF_CHUNK_SIZE;
        void* new_data = realloc(buffer->data, buffer->allocated_size);
        if (!new_data) {
            buffer->allocated_size -= DBUF_CHUNK_SIZE;
            return false;
        }
        buffer->data = new_data;
    }
    return true;
}

uint8_t dbuf_pop(dbuf* buffer) {
    uint8_t byte = buffer->data[buffer->length_filled];
    buffer->length_filled--;
    return byte;
}

uint8_t* dbuf_data(dbuf* buffer) {
    return buffer->data;
}

// Serialization and deserialization
void dbuf_serialize(dbuf* buffer, FILE* file) {
    // Write magic number to file
    fwrite(DBUF_SERIAL_MAGIC, sizeof(uint8_t), 4, file);

    // Write length of buffer to file
    fwrite(&buffer->length_filled, sizeof(uint64_t), 1, file);

    // Write buffer data to file
    fwrite(buffer->data, sizeof(uint8_t), buffer->length_filled, file);
}

dbuf* dbuf_deserialize(FILE* file) {
    uint8_t magic[5];
    size_t bytes_read;

    // Read magic number
    bytes_read = fread(magic, sizeof(uint8_t), 4, file);

    // Make sure 4 bytes read
    if (bytes_read != 4) {
        errno = EIO;
        return NULL;
    }

    // Make sure the read bytes are correct
    if (strcmp(magic, DBUF_SERIAL_MAGIC) != 0) {
        errno = EILSEQ;
        return NULL;
    }

    // Read the length of the data
    uint64_t length;
    bytes_read = fread(&length, sizeof(uint64_t), 1, file);

    // Make sure uint64_t was read
    if (bytes_read != sizeof(uint64_t)) {
        errno = EIO;
        return NULL;
    }

    // Create a dbuf to fill with data
    dbuf* ret = dbuf_create(length);

    // Read data into dbuf directly (without using append function call)
    bytes_read = fread(ret->data, sizeof(uint8_t), length, file);

    // Make sure the right amount of bytes have been read
    if (bytes_read != sizeof(uint8_t) * length) {
        dbuf_destroy(ret);
        errno = EIO;
        return NULL;
    }

    return ret;
}


