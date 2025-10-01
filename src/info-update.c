#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/dict.h>
#include <libavutil/pixdesc.h>

#include "column-provider.h"
#include "format.h"
#include "info-provider.h"
#include "avinfo-object.h"

#include "info-update.h"


#define AVINFO_STREAM_TYPE_AUDIO "audio"
#define AVINFO_STREAM_TYPE_VIDEO "video"

static const char *EXT_ATTR_ALL[6] = {
    EXT_DURATION_ATTR,
    EXT_DIMENSIONS_ATTR,
    EXT_FRAMERATE_ATTR,
    EXT_TOTAL_BITRATE_ATTR,
    EXT_AUDIO_BITRATE_ATTR,
    EXT_VIDEO_BITRATE_ATTR
};
static const int NUM_EXT_ATTR_ALL = G_N_ELEMENTS(EXT_ATTR_ALL);

void set_file_info_attributes_from_nautilus_info(GFileInfo *file_info, NautilusFileInfo *nautilus_info)
{
    for (size_t i = 0; i < NUM_EXT_ATTR_ALL; ++i) {
        gchar *val = nautilus_file_info_get_string_attribute(nautilus_info, EXT_ATTR_ALL[i]);
        if (val == NULL) {
            continue;
        }
        g_file_info_set_attribute_string(file_info, EXT_ATTR_ALL[i], val);
        g_free(val);
    }
    gchar *val = g_date_time_format_iso8601 (g_date_time_new_now_utc());
    g_file_info_set_attribute_string(file_info, EXT_LAST_COMPUTED_ATTR, val);
    g_free(val);
    return;
}

void set_nautilus_info_attributes_from_file_info(NautilusFileInfo *nautilus_info, GFileInfo *file_info)
{
    for (size_t i = 0; i < NUM_EXT_ATTR_ALL; ++i) {
        const char *val = g_file_info_get_attribute_string(file_info, EXT_ATTR_ALL[i]);
        if (val == NULL) {
            continue;
        }
        nautilus_file_info_add_string_attribute(nautilus_info, EXT_ATTR_ALL[i], val);
        // TODO: Investigate why g_free may segfault here
    }
}

int has_cached_file_info_attributes(GFileInfo *file_info)
{
    if (g_file_info_has_attribute(file_info, EXT_LAST_COMPUTED_ATTR)) {
        const gchar *val = g_file_info_get_attribute_string(file_info, EXT_LAST_COMPUTED_ATTR);
        GDateTime *last_computed = g_date_time_new_from_iso8601(val, NULL);
        GDateTime *last_modified = g_file_info_get_modification_date_time(file_info);
        int is_cache_outdated = last_computed == NULL || last_modified == NULL || g_date_time_compare(last_computed, last_modified) < 0;
        if (last_computed != NULL) {
            g_date_time_unref(last_computed);
        }
        if (last_modified != NULL) {
            g_date_time_unref(last_modified);
        }
        if (is_cache_outdated) {
            return FALSE;
        }
    }
    for (size_t i = 0; i < NUM_EXT_ATTR_ALL; ++i) {
        if (g_file_info_has_attribute(file_info, EXT_ATTR_ALL[i])) {
            return TRUE;
        }
    }
    return FALSE;
}

void
update_file_info(NautilusFileInfo *nautilus_info)
{
    size_t buf_nb = 128;
    char buf[buf_nb];
    int set_audio = FALSE, set_video = FALSE;
    int retcode = 0;

    GFile *file = nautilus_file_info_get_location(nautilus_info);
    char *file_path = g_file_get_path(file);
    if (file_path == NULL) {
        g_object_unref(file);
        return;
    }

    GError *error = NULL;
    GFileInfo *file_info = g_file_query_info(file, EXT_ATTR_WILDCARD, 0, NULL, &error);
    if (error != NULL) {
        g_warning("Failed to get attributes for file %s: %s", file_path, error->message);
        g_error_free(error);
        g_object_unref(file_info);
        g_object_unref(file);
        g_free(file_path);
        return;
    }
    if (has_cached_file_info_attributes(file_info)) {
        g_info("Using cached info for file %s", file_path);
        set_nautilus_info_attributes_from_file_info(nautilus_info, file_info);
        g_object_unref(file_info);
        g_object_unref(file);
        g_free(file_path);
        return;
    }
    g_info("Updating info for file %s", file_path);

    AVFormatContext *ctx = avformat_alloc_context();
    retcode = avformat_open_input(&ctx, file_path, NULL, NULL);
    if (retcode != 0) {
        g_warning("Failed to open file %s: %s", file_path, av_err2str(retcode));
        g_object_unref(file_info);
        g_object_unref(file);
        g_free(file_path);
        return;
    }
    avformat_find_stream_info(ctx, NULL);

    nautilus_file_info_add_string_attribute(nautilus_info, EXT_TOTAL_BITRATE_ATTR, format_bitrate(buf, buf_nb, ctx->bit_rate));
    nautilus_file_info_add_string_attribute(nautilus_info, EXT_DURATION_ATTR, format_duration(buf, buf_nb, ctx->duration));
    for (size_t i = 0; i < ctx->nb_streams; ++i) {
        AVStream *stream = ctx->streams[i];
        AVCodecParameters *params = stream->codecpar;
        if (params) {
            const char *type = av_get_media_type_string(params->codec_type);
            if (strcmp(type, AVINFO_STREAM_TYPE_AUDIO) == 0 && !set_audio) {
                nautilus_file_info_add_string_attribute(nautilus_info, EXT_AUDIO_BITRATE_ATTR, format_bitrate(buf, buf_nb, params->bit_rate));
                set_audio = TRUE;
            } else if (strcmp(type, AVINFO_STREAM_TYPE_VIDEO) == 0 && !set_video) {
                nautilus_file_info_add_string_attribute(nautilus_info, EXT_VIDEO_BITRATE_ATTR, format_bitrate(buf, buf_nb, params->bit_rate));
                nautilus_file_info_add_string_attribute(nautilus_info, EXT_DIMENSIONS_ATTR, format_dimensions(buf, buf_nb, params->width, params->height));
                nautilus_file_info_add_string_attribute(nautilus_info, EXT_FRAMERATE_ATTR, format_framerate(buf, buf_nb, av_q2d(stream->r_frame_rate)));
                set_video = TRUE;
            }
        }
    }
    avformat_close_input(&ctx);
    avformat_free_context(ctx);

    set_file_info_attributes_from_nautilus_info(file_info, nautilus_info);
    g_file_set_attributes_from_info(file, file_info, 0, NULL, &error);
    if (error != NULL){
        g_warning("Failed to set attributes for file %s: %s", file_path, error->message);
        g_error_free(error);
        g_object_unref(file_info);
        g_object_unref(file);
        g_free(file_path);
        return;
    }
    g_object_unref(file_info);
    g_object_unref(file);
    g_free(file_path);
}

void *
update_file_info_worker(void *data)
{
    struct UpdateFileInfoArgs *args = (struct UpdateFileInfoArgs *)data;
    while TRUE {
        pthread_mutex_lock(args->lk);
        while (g_queue_is_empty(args->queue)) {
            pthread_cond_wait(args->cv, args->lk);
        }
        struct UpdateFileInfoHandle *handle = (struct UpdateFileInfoHandle *)g_queue_pop_head(args->queue);
        pthread_mutex_unlock(args->lk);

        update_file_info(handle->file);
        
        pthread_mutex_lock(args->lk);
        if (!handle->cancelled) {
            nautilus_info_provider_update_complete_invoke(handle->update_complete,
                                                          handle->provider,
                                                          (NautilusOperationHandle *)handle,
                                                          NAUTILUS_OPERATION_COMPLETE);
        }
        pthread_mutex_unlock(args->lk);

        g_closure_unref(handle->update_complete);
        g_object_unref(handle->file);
        g_free(handle);
    }
    return NULL;
}
