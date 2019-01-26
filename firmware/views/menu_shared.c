#include "menu_shared.h"

// C
#include "string.h"

static menu_item _shared_items[SHARED_MENU_SIZE];

menu_item* populate_shared_menu(const menu_item* items, uint8_t count)
{
	// Doing this with memcpy ends up smaller than assigning each index of the menu
	// items array to a specific value. This is also slightly more readable.
	memcpy(_shared_items, items, count * sizeof(menu_item));
	return _shared_items;
}