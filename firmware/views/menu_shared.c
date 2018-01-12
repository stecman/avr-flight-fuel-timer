#include "menu_shared.h"

// C
#include "assert.h"
#include "string.h"

#define SHARED_MENU_SIZE 8

static menu_item _shared_items[SHARED_MENU_SIZE];

menu_item* populate_shared_menu(const menu_item* items, size_t count)
{
	assert(count <= SHARED_MENU_SIZE);

	// Doing this with memcpy ends up smaller than assigning each index of the menu
	// items array to a specific value. This is also slightly more readable.
	memcpy(_shared_items, items, count * sizeof(menu_item));
	return _shared_items;
}