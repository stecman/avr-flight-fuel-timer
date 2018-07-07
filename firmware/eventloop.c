#include "eventloop.h"

// AVR
#include <avr/interrupt.h>

// C
#include <stdlib.h>

/**
 * Return the next index in a circular buffer of QUEUE_SIZE items
 */
static uint8_t next_index(uint8_t index)
{
    index++;

    if (index >= QUEUE_SIZE) {
        index = 0;
    }

    return index;
}

void eventloop_init(EventQueue* queue)
{
    queue->insertionIndex = 0;
    queue->readIndex = 0;
}

void eventloop_run(EventQueue* queue)
{
    while (true) {
        EventHandler handler = eventloop_dequeue(queue);

        if (handler == NULL) {
            break;
        }

        handler();
    }
}

bool eventloop_queue(EventQueue* queue, EventHandler handler)
{
    cli();

    uint8_t nextIndex = next_index(queue->insertionIndex);

    // Bail out if inserting something would overwrite an index waiting to be read
    if (nextIndex == queue->readIndex) {
        return false;
    }

    // Add this event to the queue
    queue->queue[ queue->insertionIndex ] = handler;

    // Increment insertion index
    queue->insertionIndex = nextIndex;

    sei();

    return true;
}

EventHandler eventloop_dequeue(EventQueue* queue)
{
    // Disable interrupts while we grab a copy of the current function pointer
    // This avoids an event being dropped if an interrupt occurs between copy
    // and increment when the queue is full.
    cli();

    const uint8_t readIndex = queue->readIndex;

    // When the read index catches up to the insertion index, there's no more data
    if (queue->insertionIndex == readIndex) {
        sei();
        return NULL;
    }

    // Get a copy of the function pointer
    EventHandler handler = queue->queue[ readIndex ];

    // Clear this slot in the queue
    // This isn't strictly necessary, but it helps prevent any bugs or corruption
    // calling handlers that were previously in the queue but should no longer be called
    queue->queue[ readIndex ] = NULL;

    // Increment read index
    queue->readIndex = next_index(readIndex);

    sei();

    return handler;
}
