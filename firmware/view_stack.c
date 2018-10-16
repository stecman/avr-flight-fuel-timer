#include "view_stack.h"

/**
 * Activate a view (prepare, bind and render)
 *
 * @param isFirstMount - signal to the view that it's new (not just focused)
 */
static void viewstack_make_active(ViewStackFrame* view, bool isFirstMount)
{
    // Prepare view
    if (isFirstMount && view->frameWillMount != NULL) {
        view->frameWillMount();
    }

    if (view->frameWillGetFocus != NULL) {
        view->frameWillGetFocus();
    }

    // Bind input controls
    rtenc_bind_incr(view->handleIncrement);
    rtenc_bind_decr(view->handleDecrement);
    rtenc_bind_short_press(view->handleShortPress);

    // Bind renderer
    display_set_renderer(view->renderer);

    // Trigger a render at end-of-loop
    display_mark_dirty();
}

// Deactivate a view (prepare for removal from the top of the stack)
static void viewstack_teardown(ViewStackFrame* view)
{
    // Clear encoder binding so the old view doesn't recieve input during transition
    // These events might be better proxied through viewstack so it can pass queued
    // events to a new handler if there are any waiting after transition.
    rtenc_bind_incr(NULL);
    rtenc_bind_decr(NULL);
    rtenc_bind_short_press(NULL);
}

// Get the item at the top of the stack
static inline ViewStackFrame* _get_top_frame(ViewStack* vs)
{
    return vs->stack[vs->top];
}

// Set the item at the current top index
static inline void _set_top_frame(ViewStack* vs, ViewStackFrame* view)
{
    vs->stack[vs->top] = view;
}

void viewstack_init(ViewStack* vs, ViewStackFrame* rootView)
{
    vs->top = 0;
    _set_top_frame(vs, rootView);
    viewstack_make_active(rootView, true);
}

ViewStackFrame* viewstack_pop(ViewStack* vs)
{
    // Bail out if there are no more views to pop
    // (View 0 can only be replaced once initially set)
    if (vs->top == 0) {
        return NULL;
    }

    ViewStackFrame* oldView = _get_top_frame(vs);
    viewstack_teardown(oldView);

    // Decrement top and activate the next view in the stack
    // The old pointer isn't cleaned up or nulled as all views are assumed to be static
    --(vs->top);
    viewstack_make_active(_get_top_frame(vs), false);

    return oldView;
}

bool viewstack_push(ViewStack* vs, ViewStackFrame* view)
{
    // Bail out if there's no space to put another view
    if (vs->top >= VIEWSTACK_SIZE) {
        return false;
    }

   // Add to the top of the stack and activate
   ++(vs->top);
   _set_top_frame(vs, view);
   viewstack_make_active(view, true);

   return true;
}

void viewstack_replace(ViewStack* vs, ViewStackFrame* view)
{
    viewstack_teardown(_get_top_frame(vs));

    // Replace the current top item with the passed view
    _set_top_frame(vs, view);
    viewstack_make_active(view, true);
}
