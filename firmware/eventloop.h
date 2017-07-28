#pragma once

#include <stdbool.h>
#include <stdint.h>

#define QUEUE_SIZE 50

typedef void (*EventHandler)(void);

// Circular buffer for the event queue 
typedef struct EventQueue {
    EventHandler queue[QUEUE_SIZE];
    uint8_t insertionIndex;
    uint8_t readIndex;
} EventQueue;

/**
 * Prepare a queue for use
 */
void eventloop_init(EventQueue* queue);

/**
 * Add a call to the queue
 * Returns false to indicate failure if the queue has no slots
 */
bool eventloop_queue(EventQueue* queue, EventHandler handler);

/**
 * Get the first item in the queue and free its slot in the queue
 */
EventHandler eventloop_dequeue(EventQueue* queue);

/**
 * Process all pending events in the queue
 */
void eventloop_run(EventQueue* queue);
