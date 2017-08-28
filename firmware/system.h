#pragma once

#include "display/menu.h"
#include "eventloop.h"
#include "view_stack.h"

/**
 * Access to functions that manipulate global state (event loop, view stack, etc)
 *
 * This should ONLY be included in implementations, not headers to avoid
 * circular dependency.  Instead of relying on this to get typedefs, include
 * the header that defines the typedef you need directly.
 */

// Delegate eventloop functions that implicitly use the global queue
// For documentation see eventloop.h
void global_eventloop_init(void);
bool global_eventloop_queue(EventHandler handler);
EventHandler global_eventloop_dequeue(void);
void global_eventloop_run(void);
