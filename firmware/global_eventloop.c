#include "global_eventloop.h"

// Internal event loop for globaL_* eventloop functions to use
static EventQueue _eventQueue;

void global_eventloop_init(void)
{
    eventloop_init(&_eventQueue);
}

bool global_eventloop_queue(EventHandler handler)
{
    eventloop_queue(&_eventQueue, handler);
}

EventHandler global_eventloop_dequeue(void)
{
    return eventloop_dequeue(&_eventQueue);
}

void global_eventloop_run(void)
{
    eventloop_run(&_eventQueue);
}
