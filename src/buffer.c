#include <malhar-gp-extensions/buffer.h>

#include <string.h>

// Resets state variables
void buffer_resetState(buffer_t * const pBuffer)
{
    pBuffer->count = 0;
    pBuffer->head = 0;
    pBuffer->tail = 0;
}

// Creates a new buffer
int buffer_create(buffer_t * const pBuffer, const size_t szUnit, const size_t ciMax)
{
	// Template buffer to initialise const values
    buffer_t bufTemplate = {.unit = szUnit, .length = ciMax, .size = szUnit * ciMax};
    
    // Make sure memory is allocated
    if (!pBuffer)
        return 1;
    
    // Initialise buffer structure
    if(!memcpy(pBuffer, &bufTemplate, sizeof(buffer_t)))
        return 2;
    
    // Allocate memory for the buffer data space itself
    pBuffer->data = malloc(pBuffer->size);
    if (!pBuffer->data)
        return 1;
    
    // Clear buffer and initialise state variables
	return buffer_clear(pBuffer);
}

// Pushes a new value onto a buffer
int buffer_push(buffer_t * const pBuffer, void * const pData, const buffer_writeflag_t flags)
{
    uint8_t bOverwrite = 0;
    
    // Make sure buffer exists
    if (!pBuffer)
        return -1;
    
    // Calculate new head value
    size_t nextHead = pBuffer->head + 1;
    if ((nextHead >= pBuffer->length) || (pBuffer->count == 0))
        nextHead = 0;
    
    // Check if overwrite will occur and proceed only if permitted
    bOverwrite = (nextHead == pBuffer->tail) && (pBuffer->count > 0);
    if (bOverwrite & !(flags & BUFFER_WRITE_OVERWRITE))
        return -1;
    
    // Move head of buffer
    pBuffer->head = nextHead;
    
    // Load data into buffer
    if(!memcpy(pBuffer->data + (pBuffer->head * pBuffer->unit), pData, pBuffer->unit))
        return 1;
    
    // Check if overwrite has occurred
	if (bOverwrite)
	{
		// Move tail whilst accounting for overwrap
		if (pBuffer->tail + 1 >= pBuffer->length)
			pBuffer->tail = 0;
		else
			pBuffer->tail++;
	}
	else
		// Update count
        pBuffer->count++;
    
    return 0;
}

// Retrieves the newest value from a buffer
int buffer_pop(buffer_t * const pBuffer, void * const pData, const buffer_readflag_t flags)
{
    size_t nextHead = 0;
    
    // Make sure buffer exists
    if (!pBuffer)
        return -1;
    
    // Check if head movement is required
    nextHead = pBuffer->head;
    if (flags & BUFFER_READ_CLEARVALUE)
    {
        // Check for wraparound with new head value
        if (nextHead == 0)
            nextHead = pBuffer->length - 1;
        else
            nextHead--;
    }
    
    // Return data
    if (!memcpy(pData, pBuffer->data + (pBuffer->head * pBuffer->unit), pBuffer->unit))
        return 1;
    
    // Adjust head value or clear buffer
    if (flags & BUFFER_READ_CLEARBUFFER)
		return buffer_clear(pBuffer);
    else
        pBuffer->head = nextHead;
        
    // Adjust count if required
    if (flags & BUFFER_READ_CLEARVALUE)
        pBuffer->count--;
    
    return 0;
}

// Retrieves the oldest value from a buffer
int buffer_shift(buffer_t * const pBuffer, void * const pData, const buffer_readflag_t flags)
{
    size_t nextTail = 0;
    
    // Make sure buffer exists
    if (!pBuffer)
        return -1;
    
    // Check if tail movement is required
    nextTail = pBuffer->tail;
    if (flags & BUFFER_READ_CLEARVALUE)
        // Check for wraparound with new tail value
        if (++nextTail >= pBuffer->length)
            nextTail = 0;
    
    // Return data
    if (!memcpy(pData, pBuffer->data + (pBuffer->tail * pBuffer->unit), pBuffer->unit))
        return 1;
    
    // Adjust tail value or clear buffer
	if (flags & BUFFER_READ_CLEARBUFFER)
		return buffer_clear(pBuffer);
    else
        pBuffer->tail = nextTail;
        
    // Adjust count if required
    if (flags & BUFFER_READ_CLEARVALUE)
        pBuffer->count--;
    
    return 0;
}

// Clears a buffer
int buffer_clear(buffer_t * const pBuffer)
{
    // Make sure buffer exists
    if (!pBuffer)
        return -1;
    
    // Clear contents of buffer
    if (!memset(pBuffer->data, 0, pBuffer->size))
        return 1;
    
    // Reset state variables
    buffer_resetState(pBuffer);
    
    return 0;
}


