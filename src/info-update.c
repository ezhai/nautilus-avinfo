#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/dict.h>
#include <libavutil/pixdesc.h>

#include "column-provider.h"
#include "format.h"
#include "info-provider.h"
#include "avinfo-object.h"

#include "info-update.h"


void
update_file_info(NautilusFileInfo *file)
{
    size_t buf_nb = 128;
    char buf[buf_nb];
    int set_audio = FALSE, set_video = FALSE;

    char *file_path = g_file_get_path(nautilus_file_info_get_location(file));
    if (file_path == NULL) {
        return;
    }

    AVFormatContext *ctx = avformat_alloc_context();
    int retcode = avformat_open_input(&ctx, file_path, NULL, NULL);
    if (retcode != 0) {
        avformat_free_context(ctx);
        return;
    }
    avformat_find_stream_info(ctx, NULL);

    nautilus_file_info_add_string_attribute(file, EXT_TOTAL_BITRATE_ATTR, format_bitrate(buf, buf_nb, ctx->bit_rate));
    nautilus_file_info_add_string_attribute(file, EXT_DURATION_ATTR, format_duration(buf, buf_nb, ctx->duration));
    for (unsigned int i = 0; i < ctx->nb_streams; ++i) {
        AVStream *stream = ctx->streams[i];
        AVCodecParameters *params = stream->codecpar;
        if (params) {
            const char *type = av_get_media_type_string(params->codec_type);
            if (strcmp(type, "audio") == 0 && !set_audio) {
                nautilus_file_info_add_string_attribute(file, EXT_AUDIO_BITRATE_ATTR, format_bitrate(buf, buf_nb, params->bit_rate));
                set_audio = TRUE;
            } else if (strcmp(type, "video") == 0 && !set_video) {
                nautilus_file_info_add_string_attribute(file, EXT_VIDEO_BITRATE_ATTR, format_bitrate(buf, buf_nb, params->bit_rate));
                nautilus_file_info_add_string_attribute(file, EXT_DIMENSIONS_ATTR, format_dimensions(buf, buf_nb, params->width, params->height));
                nautilus_file_info_add_string_attribute(file, EXT_FRAMERATE_ATTR, format_framerate(buf, buf_nb, av_q2d(stream->r_frame_rate)));
                set_video = TRUE;
            }
        }
    }
    avformat_close_input(&ctx);
    avformat_free_context(ctx);
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


