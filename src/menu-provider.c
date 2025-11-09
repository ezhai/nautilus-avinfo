#include <config.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <nautilus-extension.h>
#include <stdbool.h>
#include <stdint.h>

#define GNOME_DESKTOP_USE_UNSTABLE_API
#include <libgnome-desktop/gnome-desktop-thumbnail.h>

#include "menu-provider.h"


typedef struct _AVInfoThumbnailRefreshJob AVInfoThumbnailRefreshJob;

struct _AVInfoThumbnailRefreshJob {
    char *file_uri;
    char *mime_type;
    int64_t m_time;
    GTimer *timer;
};

/**
 * avinfo_thumbnail_refresh_job_create:
 * 
 * Creates and initializes an AVInfoThumbnailRefreshJob struct.
 * The caller of the method takes ownership of the return value, and is responsible for freeing it.
 * 
 */
AVInfoThumbnailRefreshJob *
avinfo_thumbnail_refresh_job_create(char *file_uri, char *mime_type, int64_t m_time)
{
    GTimer *timer = g_timer_new();

    g_debug("Starting AVInfoThumbnailRefreshJob for %s", file_uri);
    AVInfoThumbnailRefreshJob *job = g_new(AVInfoThumbnailRefreshJob, 1);
    job->file_uri = g_strdup(file_uri);
    job->mime_type = g_strdup(mime_type);
    job->m_time = m_time;
    job->timer = timer;
    return job; 
}

/**
 * avinfo_thumbnail_refresh_job_free:
 * 
 * Frees an AVInfoThumbnailRefreshJob struct and its associated resources.
 * 
 */
void
avinfo_thumbnail_refresh_job_free(AVInfoThumbnailRefreshJob *job)
{
    if (job == NULL) {
        return;
    }
    g_debug("Freeing AVInfoThumbnailRefreshJob for %s", job->file_uri);
    g_free(job->file_uri);
    g_free(job->mime_type);
    g_timer_destroy(job->timer);
    g_free(job);
}

G_DEFINE_AUTOPTR_CLEANUP_FUNC(AVInfoThumbnailRefreshJob, avinfo_thumbnail_refresh_job_free);

/**
 * is_eligible_for_thumbnail_refresh:
 *
 * Determines if @nautilus_file_info is eligible for thumbnail refresh based on GNOME Desktop's
 * eligibility criteria.
 *
 */
bool
is_eligible_for_thumbnail_refresh(NautilusFileInfo *nautilus_file_info)
{
    g_autoptr(GError) error = NULL;
    g_autoptr(GFile) file = nautilus_file_info_get_location(nautilus_file_info);
    g_autofree char *file_path = g_file_get_path(file);
    if (file_path == NULL) {
        return false;
    }
    g_autoptr(GFileInfo) file_info = g_file_query_info(file, "thumbnail:*,time::*", 0, NULL, &error);
    if (error != NULL) {
        return false;
    }
    g_autofree char *file_uri = g_file_get_uri(file);
    g_autofree char *mime_type = nautilus_file_info_get_mime_type(nautilus_file_info);
    g_autoptr(GDateTime) m_date_time = g_file_info_get_modification_date_time(file_info);
    if (m_date_time == NULL) {
        return false;
    }
    int64_t m_time = g_date_time_to_unix(m_date_time);

    g_autoptr(GnomeDesktopThumbnailFactory) factory = gnome_desktop_thumbnail_factory_new(GNOME_DESKTOP_THUMBNAIL_SIZE_NORMAL);
    return gnome_desktop_thumbnail_factory_can_thumbnail(factory, file_uri, mime_type, m_time);
}

/**
 * on_thumbnail_refresh_finished:
 * 
 * GAsyncReadyCallback for when the thumbnail generation is complete.
 * Intended to be used with `gnome_desktop_thumbnail_factory_generate_thumbnail_async`.
 *
 * This frees the job when done. The factory will be freed automatically once this method returns.
 *
 */
void
on_thumbnail_refresh_finished(GObject *obj, GAsyncResult *res, gpointer data)
{
    GnomeDesktopThumbnailFactory *factory = GNOME_DESKTOP_THUMBNAIL_FACTORY(obj);
    g_autoptr(AVInfoThumbnailRefreshJob) job = (AVInfoThumbnailRefreshJob *)data;

    g_autoptr(GError) error = NULL;
    g_autoptr(GdkPixbuf) thumbnail = gnome_desktop_thumbnail_factory_generate_thumbnail_finish(factory, res, &error);
    if (error != NULL) {
        g_warning("Failed to generate thumbnail asynchronously for file %s: %s", job->file_uri, error->message);
        return;
    }

    gnome_desktop_thumbnail_factory_save_thumbnail(factory, thumbnail, job->file_uri, job->m_time, NULL, &error);
    if (error != NULL) {
        g_warning("Failed to generate thumbnail for file %s: %s", job->file_uri, error->message);
        return;
    }
    g_info("Refreshed thumbnail for file %s in %.4fs", job->file_uri, g_timer_elapsed(job->timer, NULL));
}

void
avinfo_menu_provider_interface_init(NautilusMenuProviderInterface *interface)
{
    interface->get_file_items = avinfo_menu_provider_get_file_items;
}

GList *
avinfo_menu_provider_get_file_items(NautilusMenuProvider *provider, GList *files)
{
    if (g_list_length(files) == 0) {
        return NULL;
    }
    NautilusMenuItem *menu_item = nautilus_menu_item_new(AVINFO_MENU_ITEM_REFRESH_THUMBNAIL,
                                                         dgettext(PACKAGE, "Refresh Thumbnail"),
                                                         NULL, NULL);

    g_autoptr(GTimer)   timer = g_timer_new();
    GList *copy = NULL;
    for (GList *it = files; it != NULL; it = it->next) {
        NautilusFileInfo *nautilus_file_info = (NautilusFileInfo *)it->data;
        if (is_eligible_for_thumbnail_refresh(nautilus_file_info)) {
            g_autofree char *file_uri = nautilus_file_info_get_uri(nautilus_file_info);
            copy = g_list_prepend(copy, nautilus_file_info_create_for_uri(file_uri));
        }
    }
    if (g_list_length(copy) == 0) {
        return NULL;
    }
    copy = g_list_reverse(copy);

    long signal_id = g_signal_connect_data(menu_item,
                                           "activate",
                                           G_CALLBACK(avinfo_menu_item_on_activate),
                                           copy,
                                           avinfo_menu_item_on_destroy,
                                           G_CONNECT_DEFAULT);
    g_debug("Thumbnail refresh signal %ld created with %d elements!", signal_id, g_list_length(copy));
    return g_list_append(NULL, menu_item);
}

void
avinfo_menu_item_on_activate(NautilusMenuItem *item, gpointer user_data)
{
    GList *files = (GList *)user_data;
    for (GList *it = files; it != NULL; it = it->next) {
        NautilusFileInfo *nautilus_file_info = (NautilusFileInfo *)it->data;

        g_autoptr(GError) error = NULL;
        g_autoptr(GFile) file = nautilus_file_info_get_location(nautilus_file_info);
        g_autofree char *file_path = g_file_get_path(file);
        if (file_path == NULL) {
            g_warning("File path is NULL");
            continue;
        }
        g_autoptr(GFileInfo) file_info = g_file_query_info(file, "thumbnail::*,time::*", 0, NULL, &error);
        if (error != NULL) {
            g_warning("Failed to get info for file %s: %s", file_path, error->message);
            return;
        }
        g_auto(GStrv) file_attrs = g_file_info_list_attributes(file_info, "thumbnail");
        if (error != NULL) {
            g_warning("Failed to list attributes for file %s: %s", file_path, error->message);
            return;
        }
        g_autoptr(GDateTime) m_date_time = g_file_info_get_modification_date_time(file_info);
        if (m_date_time == NULL) {
            g_warning("Failed to get modification time for file %s", file_path);
            return;
        }
        int64_t m_time = g_date_time_to_unix(m_date_time);
        g_autofree char *file_uri = g_file_get_uri(file);
        g_autofree char *mime_type = nautilus_file_info_get_mime_type(nautilus_file_info);

        for (char **attr = file_attrs; *attr != NULL; ++attr) {
            if (!g_str_equal(*attr, G_FILE_ATTRIBUTE_THUMBNAIL_IS_VALID)) {
                continue;
            }
            g_autoptr(GnomeDesktopThumbnailFactory) factory = NULL;
            factory = gnome_desktop_thumbnail_factory_new(GNOME_DESKTOP_THUMBNAIL_SIZE_NORMAL);
            AVInfoThumbnailRefreshJob *job = avinfo_thumbnail_refresh_job_create(file_uri, mime_type, m_time);
            gnome_desktop_thumbnail_factory_generate_thumbnail_async(factory, job->file_uri, job->mime_type,
                                                                     NULL, on_thumbnail_refresh_finished, job);
        }
    }
}

void avinfo_menu_item_on_destroy(gpointer data, GClosure *closure)
{
    g_debug("Freeing GList for thumbnail refresh signal");
    GList *files = (GList *)data;
    nautilus_file_info_list_free(files);
}
