#include "display/menu.h"
#include "system.h"

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
menu_item* populate_shared_menu(const menu_item* items, size_t count);