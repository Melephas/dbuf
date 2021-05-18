#include <libdbuf.h>


#include <stdlib.h>
#include <string.h>


struct dbuf_s {
    uint64_t allocated_size;
    uint64_t filled_length;
    uint8_t* data;
};


// === Standard C style library interface ===
// Creation and destruction
dbuf_t* dbuf_create(uint64_t initial_size) {
    dbuf_t* buffer = malloc(sizeof(dbuf_t));

    buffer->allocated_size = initial_size;
    buffer->filled_length = 0;
    buffer->data = malloc(initial_size * sizeof(uint8_t));

    return buffer;
}

void dbuf_destroy(dbuf_t* buffer) {
    free(buffer->data);
    free(buffer);
}


// Normal functions
uint64_t dbuf_size(dbuf_t* buffer) {
    return buffer->allocated_size;
}

uint64_t dbuf_length(dbuf_t* buffer) {
    return buffer->filled_length;
}

uint8_t dbuf_pop(dbuf_t* buffer) {
    uint8_t byte = buffer->data[buffer->filled_length];
    buffer->filled_length--;

    return byte;
}

bool dbuf_append(dbuf_t* buffer, uint8_t value) {
    buffer->filled_length++;

    if (buffer->filled_length > buffer->allocated_size) {
        buffer->allocated_size += DBUF_CHUNK_SIZE;
        uint8_t* new_data = realloc(buffer->data, buffer->allocated_size);
        if (!new_data) {
            return false;
        }
        buffer->data = new_data;
    }

    buffer->data[buffer->filled_length] = value;
    return true;
}

bool dbuf_append_sequence(dbuf_t* buffer, uint8_t* sequence, uint64_t length) {
    for (int i = 0; i < length; i++) {
        if (!dbuf_append(buffer, sequence[i])) {
            return false;
        }
    }
    return true;
}


// Conversion functions
uint64_t dbuf_as_bytes(dbuf_t* buffer, uint8_t* destination) {
    // Calculate lengths
    uint64_t length = dbuf_length(buffer);
    uint64_t total_length = (length * sizeof(uint8_t)) + sizeof(uint64_t);

    // Initialise destination memory
    destination = malloc(total_length);

    // Write size to first bytes by interpreting as an array of uint64_t
    uint64_t* size_pos = (uint64_t*) destination;
    size_pos[0] = length;

    // Move past written uint64_t
    uint8_t* data_pos = ((uint8_t*) ++size_pos);

    // Write the bytes stored in the buffer to the destination array
    memcpy(data_pos, buffer->data, length);

    return total_length;
}

dbuf_t* dbuf_from_bytes(uint8_t* source, uint64_t length) {
    // Read stored size uint64_t by reinterpretation
    uint64_t* size_pos = (uint64_t*) source;
    uint64_t size = size_pos[0];

    // Check the length of the array matches the length of the data to be copied int the buffer (plus the storage for the uint64_t size variable)
    if (length != size + sizeof(uint64_t)) {
        return NULL;
    }

    // Create the buffer and copy the data into it
    dbuf_t* ret = dbuf_create(size);
    memcpy(ret->data, source + (sizeof(uint64_t)/sizeof(uint8_t)), size);

    return ret;
}
