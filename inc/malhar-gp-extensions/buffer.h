#ifndef __MALHARGPEXT_BUFFER_H
#define __MALHARGPEXT_BUFFER_H
    
#ifdef _cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

// General buffer type
typedef struct {
    uint8_t *data;          // Pointer to actual buffer data
    size_t count;           // Number of elements currently in buffer
    size_t head;            // Position of buffer head (newest value)
    size_t tail;            // Position of buffer tail (oldest value)
    const size_t unit;      // Size of each element in buffer
    const size_t length;    // Maximum number of elements in buffer
    const size_t size;      // Size of buffer
} buffer_t;

// Buffer read flags
typedef uint8_t buffer_readflag_t;
#define BUFFER_READ_CLEARVALUE      0x01    // Removes the value from the buffer (default is keep)
#define BUFFER_READ_CLEARBUFFER     0x02    // Clears the buffer

// Buffer write flags
typedef uint8_t buffer_writeflag_t;
#define BUFFER_WRITE_OVERWRITE      0x01    // If buffer is full, overwrites oldest value (default is error)

/**
 * @brief Creates a new buffer with ciMax units of size szUnit.
 * 
 * @param pBuffer   Pointer to a buffer structure.
 * @param szUnit    Size of each unit in the buffer.
 * @param ciMax     Number of units in the buffer.
 * @return int      Zero if successful.
 */
int buffer_create(buffer_t * const pBuffer, const size_t szUnit, const size_t ciMax);

// Pushes a new value onto a buffer
int buffer_push(buffer_t * const pBuffer, void * const pData, const buffer_writeflag_t flags);

// Retrieves the newest value from a buffer
int buffer_pop(buffer_t * const pBuffer, void * const pData, const buffer_readflag_t flags);
#define buffer_peek(pBuffer, pData) buffer_pop(pBuffer, pData, 0)

// Retrieves the oldest value from a buffer
int buffer_shift(buffer_t * const pBuffer, void * const pData, const buffer_readflag_t flags);
#define buffer_spy(pBuffer, pData) buffer_shift(pBuffer, pData, 0)

// Clears a buffer
int buffer_clear(buffer_t * const pBuffer);

#ifdef _cplusplus
} // extern "C"
#endif
    
#endif // __MALHARGPEXT_BUFFER_H