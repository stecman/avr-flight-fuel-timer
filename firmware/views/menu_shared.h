#include "display/menu.h"
#include "macros.h"
#include "system.h"

#define SHARED_MENU_SIZE 8

/**
 * Compile-time assert that the passed array of menu items will fit into the shared buffer
 */
#define ASSERT_VALID_MENU_SIZE(array) \
	_Static_assert(COUNT_OF(array) <= SHARED_MENU_SIZE, \
        "Menu item count exceeds the shared buffer size of " \
        STR(SHARED_MENU_SIZE) ". Increase the shared buffer size!");

/**
 * Copy items to shared memory and return the shared list
 *
 * Menus should be defined in stack memory then copied into the shared array of
 * menu items with this function. Ideally menu items would be defined in progmem
 * and copied across, but that hasn't been implemented yet.
 *
 * @param items list of items to copy
 * @param count number of items to copy
 */
menu_item* populate_shared_menu(const menu_item* items, uint8_t count);