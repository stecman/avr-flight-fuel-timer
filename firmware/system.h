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

// Delegate view stack functions that implicity use the global view stack
// For documentation see eventloop.h
void global_viewstack_init(ViewStackFrame* rootView);
ViewStackFrame* global_viewstack_pop(void);
bool global_viewstack_push(ViewStackFrame* view);
void global_viewstack_replace(ViewStackFrame* view);

// Same as global_viewstack_pop, but with no return (for use in actions)
void global_viewstack_pop_silent(void);

/**
 * Handle presses in a menu
 *
 * This is separate to the definition of menu_screen and menu_item as it relies
 * on global state.
 */
void global_handle_menu_select(const menu_item* item);
