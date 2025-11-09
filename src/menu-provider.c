#include <config.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <stdbool.h>
#include <stdint.h>

#define GNOME_DESKTOP_USE_UNSTABLE_API
#include <libgnome-desktop/gnome-desktop-thumbnail.h>
#include <nautilus-extension.h>

#include "menu-provider.h"

void
avinfo_menu_provider_interface_init(NautilusMenuProviderInterface *interface)
{
    interface->get_file_items = avinfo_menu_provider_get_file_items;
}

GList *
avinfo_menu_provider_get_file_items(NautilusMenuProvider *provider,
                                    GList *files)
{
    GList *ret = NULL;
    for (GList *item = files; item != NULL; item = item->next) {
        NautilusFileInfo *file = (NautilusFileInfo *)item->data;
        NautilusMenuItem *menu_item = nautilus_menu_item_new(EXT_MENU_ACTION_REFRESH,
                                                             dgettext(PACKAGE, "Refresh Thumbnail"),
                                                             NULL, NULL);
        // Note: g_signal_connect_object is NOT thread-safe
        g_signal_connect_object(menu_item,
                                "activate",
                                G_CALLBACK(avinfo_menu_item_on_activate),
                                file, 
                                G_CONNECT_DEFAULT);
        ret = g_list_prepend(ret, menu_item);
    }
    ret = g_list_reverse(ret);
    return ret;
}

void
avinfo_menu_item_on_activate(NautilusMenuItem *item,
                             gpointer user_data)
{
    NautilusFileInfo *nautilus_file_info = (NautilusFileInfo *)user_data;
    g_autoptr(GFile) file = nautilus_file_info_get_location(nautilus_file_info);
    g_autofree char *file_path = g_file_get_path(file);
    if (file_path == NULL) {
        return;
    }

    g_autoptr(GError) error = NULL;
    g_autoptr(GFileInfo) file_info = g_file_query_info(file, "thumbnail::*", 0, NULL, &error);
    if (error != NULL) {
        g_warning("Failed to get attributes for file %s: %s", file_path, error->message);
        return;
    }

    // g_auto(GStrv) data = g_file_info_list_attributes(file_info, "thumbnail");
    // if (error != NULL) {
    //     g_warning("Failed to get attributes for file %s: %s", file_path, error->message);
    //     return;
    // }

    // g_autoptr(GFileAttributeInfoList) data2 = g_file_query_writable_namespaces(file, NULL, &error);
    // for (int i = 0; i < data2->n_infos; ++i) {
    //     GFileAttributeInfo *info = &data2->infos[i];
    //     g_info("Settable attribute for file %s: %s", file_path, info->name);
    // }

    // bool updated = FALSE;
    // for (GStrv attr = data; *attr != NULL; ++attr) {
    //     if (g_str_has_prefix(*attr, G_FILE_ATTRIBUTE_THUMBNAIL_IS_VALID)) {            
    //         g_file_info_set_attribute_boolean(file_info, *attr, FALSE);
    //         updated = TRUE;
    //     }
    // }
    // if (updated) {
    //     g_file_set_attributes_from_info(file, file_info, G_FILE_QUERY_INFO_NONE, NULL, &error);
    //     if (error != NULL) {
    //         g_warning("Failed to set attributes for file %s: %s", file_path, error->message);
    //         return;
    //     }
    // }

    g_autofree char *file_uri = g_file_get_uri(file);
    g_autofree char *mime_type = nautilus_file_info_get_mime_type(nautilus_file_info);
    g_autoptr(GDateTime) m_date_time = g_file_info_get_modification_date_time(file_info);
    if (m_date_time == NULL) {
        m_date_time = g_date_time_new_now_local();
        if (m_date_time == NULL) {
            g_warning("Failed to get modification time for file %s", file_path);
            return;
        }
    }
    int64_t m_time = g_date_time_to_unix(m_date_time);

    g_autoptr(GTimer) timer = g_timer_new();
    g_timer_start(timer);

    g_autoptr(GnomeDesktopThumbnailFactory) factory = gnome_desktop_thumbnail_factory_new(GNOME_DESKTOP_THUMBNAIL_SIZE_NORMAL);
    g_autoptr(GdkPixbuf) thumbnail = gnome_desktop_thumbnail_factory_generate_thumbnail(factory, file_uri, mime_type,
                                                                                        NULL, &error);
    if (error != NULL) {
        g_warning("Failed to generate thumbnail for file %s: %s", file_path, error->message);
        return;
    }
    gnome_desktop_thumbnail_factory_save_thumbnail(factory, thumbnail, file_uri, m_time,
                                                   NULL, &error);
    if (error != NULL) {
        g_warning("Failed to generate thumbnail for file %s: %s", file_path, error->message);
        return;
    }
    g_info("Refreshed thumbnail for file %s in %.4fs", file_path, g_timer_elapsed(timer, NULL));
}

