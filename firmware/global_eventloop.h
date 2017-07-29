#pragma once

#include "eventloop.h"

// Aliases to eventloop functions that implicitly use the global queue
// For documentation see eventloop.h
void global_eventloop_init(void);

bool global_eventloop_queue(EventHandler handler);

EventHandler global_eventloop_dequeue(void);

void global_eventloop_run(void);
