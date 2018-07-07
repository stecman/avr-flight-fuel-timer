#include "system.h"

// Internal event loop for globaL_* eventloop functions
static EventQueue _eventQueue;

void global_eventloop_init(void)
{
    eventloop_init(&_eventQueue);
}

bool global_eventloop_queue(EventHandler handler)
{
    return eventloop_queue(&_eventQueue, handler);
}

EventHandler global_eventloop_dequeue(void)
{
    return eventloop_dequeue(&_eventQueue);
}

void global_eventloop_run(void)
{
    eventloop_run(&_eventQueue);
}


// Internal view stack for global_* viewstack functions
static ViewStack _viewStack;

void global_viewstack_init(ViewStackFrame* rootView)
{
    viewstack_init(&_viewStack, rootView);
}

ViewStackFrame* global_viewstack_pop(void)
{
    return viewstack_pop(&_viewStack);
}

void global_viewstack_pop_silent(void)
{
    viewstack_pop(&_viewStack);
}

bool global_viewstack_push(ViewStackFrame* view)
{
    return viewstack_push(&_viewStack, view);
}

void global_viewstack_replace(ViewStackFrame* view)
{
    viewstack_replace(&_viewStack, view);
}


// Menu handling
void global_handle_menu_select(menu_screen* menu, const menu_item* item)
{
    if (item == NULL) {
        return;
    }

    switch (item->type) {
        case kFunctionCall:
            if (item->onClick != NULL) {
                item->onClick();
            }
            break;

        case kValueEditable:
            // Toggle editing state and redraw
            menu->flags ^= kEditingMenuItem;
            display_mark_dirty();

        default:
            break;
    }
}
