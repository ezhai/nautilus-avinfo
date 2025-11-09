#pragma once

#include <glib.h>
#include <nautilus-extension.h>

#define EXT_MENU_ACTION_REFRESH "nautilus-avinfo::refresh"

void avinfo_menu_provider_interface_init(NautilusMenuProviderInterface *interface);

GList *avinfo_menu_provider_get_file_items(NautilusMenuProvider *provider, GList *files);

void avinfo_menu_item_on_activate(NautilusMenuItem *item, gpointer user_data);
