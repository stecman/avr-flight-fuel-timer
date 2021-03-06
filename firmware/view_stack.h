#pragma once

#include "control/rotary_encoder.h"
#include "display/display.h"
#include "stdbool.h"

// Define the deepest level of views
#define VIEWSTACK_SIZE 5

typedef struct ViewStackFrame {
    // Callback: called when this frame is about to be added to the stack for
    // the first time. Use to initialise any state the view has.
    // Called before drawing.
    void (*frameWillMount)(void);

    // Callback: called each time this frame is about to become visible
    // Called before drawing. Called after frameWillMount.
    void (*frameWillGetFocus)(void);

    // Callback: called each time this frame is no longer visible
    // Called after the last draw
    void (*frameLostFocus)(void);

    // User input event handlers
    // The top view currently always recieves all input
    EventHandler handleIncrement;
    EventHandler handleDecrement;
    EventHandler handleShortPress;

    // Draw display (see display/display.h)
    Renderer renderer;

} ViewStackFrame;

typedef struct ViewStack {
    ViewStackFrame* stack[VIEWSTACK_SIZE];
    uint8_t top;
} ViewStack;

/**
 * Prepare a viewstack for use and set a root view
 *
 * The root view cannot be popped. Use viewstack_replace() to change it after init.
 * Calling this on a ViewStack that's in use will reset it with a root_view.
 */
void viewstack_init(ViewStack* vs, ViewStackFrame* rootView);

/**
 * Discard the current view state and move to the previous one
 *
 * Returns NULL if the current view is the only one on the stack. Use replace
 * if the root of the stack needs to be replaced.
 */
ViewStackFrame* viewstack_pop(ViewStack* vs);

/**
 * Add a view to the top of the stack
 *
 * Returns false if the stack is full and cannot hold another view.
 */
bool viewstack_push(ViewStack* vs, ViewStackFrame* view);

/**
 * Replace the current view with another
 */
void viewstack_replace(ViewStack* vs, ViewStackFrame* view);
