#pragma once

#include <glib.h>
#include <nautilus-extension.h>


#define AVINFO_MENU_ITEM_REFRESH_THUMBNAIL "nautilus-avinfo::refresh"

void avinfo_menu_provider_interface_init(NautilusMenuProviderInterface *interface);

GList *avinfo_menu_provider_get_file_items(NautilusMenuProvider *provider,
                                           GList *files);

void avinfo_menu_item_on_activate(NautilusMenuItem *item,
                                  gpointer user_data);

void avinfo_menu_item_on_destroy(gpointer data,
                                 GClosure *closure);
