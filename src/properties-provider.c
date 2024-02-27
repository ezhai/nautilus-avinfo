#include <config.h>
#include <glib/gi18n.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/dict.h>
#include <libavutil/pixdesc.h>

#include "format.h"

#include "properties-provider.h"

void
add_nautilus_properties_item(GListStore *items, gchar *prefix, gchar *name, const gchar *value)
{
    size_t name_copy_nb = 32;
    gchar name_copy[name_copy_nb];

    if (prefix[0] == '\0') {
        g_list_store_append(items, nautilus_properties_item_new(name, value));
    } else {
        g_snprintf(name_copy, name_copy_nb, "%s %s", prefix, name);
        g_list_store_append(items, nautilus_properties_item_new(name_copy, value));
    }
}

void
add_av_stream_nautilus_properties_info(GListStore *items,
                                       AVStream *stream)
{
    size_t prefix_nb = 32, buf_nb = 128;
    gchar prefix[prefix_nb], buf[buf_nb];

    AVCodecParameters *params = stream->codecpar;
    if (params) {
        g_snprintf(prefix, prefix_nb, dgettext(PACKAGE, "Stream %d -"), stream->id);
        const char *type = av_get_media_type_string(params->codec_type);

        add_nautilus_properties_item(items, prefix, dgettext(PACKAGE, "Type"), type);
        add_nautilus_properties_item(items, prefix, dgettext(PACKAGE, "Codec"), avcodec_descriptor_get(params->codec_id)->name);
        add_nautilus_properties_item(items, prefix, dgettext(PACKAGE, "Codec"), avcodec_descriptor_get(params->codec_id)->long_name);
        add_nautilus_properties_item(items, prefix, dgettext(PACKAGE, "Codec Tag"), av_fourcc2str(params->codec_tag));
        add_nautilus_properties_item(items, prefix, dgettext(PACKAGE, "Codec Profile"), av_get_profile_name(avcodec_find_decoder(params->codec_id), params->profile));

        if (strcmp(type, "video") == 0) {
            if (stream->avg_frame_rate.num && stream->avg_frame_rate.den) {
                add_nautilus_properties_item(items, prefix, dgettext(PACKAGE, "Frame Rate"), format_framerate(buf, buf_nb, av_q2d(stream->avg_frame_rate)));
            }
            if (stream->r_frame_rate.num && stream->r_frame_rate.den) {
                add_nautilus_properties_item(items, prefix, dgettext(PACKAGE, "Time Base Real"), format_string(buf, buf_nb, "%.1f tbr", av_q2d(stream->r_frame_rate)));
            }
            if (stream->time_base.num && stream->time_base.den) {
                add_nautilus_properties_item(items, prefix, dgettext(PACKAGE, "Time Base Number"), format_basenumber(buf, buf_nb, av_q2d(av_inv_q(stream->time_base))));
            }
            add_nautilus_properties_item(items, prefix, dgettext(PACKAGE, "Dimensions"), format_dimensions(buf, buf_nb, params->width, params->height));
            add_nautilus_properties_item(items, prefix, dgettext(PACKAGE, "Video Bit Rate"), format_bitrate(buf, buf_nb, params->bit_rate));
            add_nautilus_properties_item(items, prefix, dgettext(PACKAGE, "Color Format"), av_get_pix_fmt_name(params->format));
            add_nautilus_properties_item(items, prefix, dgettext(PACKAGE, "Color Range"), av_color_range_name(params->color_range));
            add_nautilus_properties_item(items, prefix, dgettext(PACKAGE, "Color Space"), av_color_space_name(params->color_space));
            add_nautilus_properties_item(items, prefix, dgettext(PACKAGE, "Color Chroma Location"), av_chroma_location_name(params->color_primaries));
        } else if (strcmp(type, "audio") == 0) {
            add_nautilus_properties_item(items, prefix, dgettext(PACKAGE, "Audio Bit Rate"), format_bitrate(buf, buf_nb, params->bit_rate));
            add_nautilus_properties_item(items, prefix, dgettext(PACKAGE, "Channels"), format_string(buf, buf_nb, "%d", params->ch_layout.nb_channels));
            add_nautilus_properties_item(items, prefix, dgettext(PACKAGE, "Sample Format"), av_get_sample_fmt_name(params->format));
            add_nautilus_properties_item(items, prefix, dgettext(PACKAGE, "Sample Rate"), format_string(buf, buf_nb, "%d Hz", params->sample_rate));
        }
    }
}

void
add_av_format_context_nautilus_properties_info(GListStore *items,
                                               AVFormatContext *ctx)
{
    size_t buf_nb = 16;
    gchar buf[buf_nb];


    if (ctx->iformat) {
        g_list_store_append(items, nautilus_properties_item_new(dgettext(PACKAGE, "Format"), ctx->iformat->long_name));
    }
    g_list_store_append(items, nautilus_properties_item_new(dgettext(PACKAGE, "Duration"), format_duration(buf, buf_nb, ctx->duration)));
    g_list_store_append(items, nautilus_properties_item_new(dgettext(PACKAGE, "Total Bit Rate"), format_bitrate(buf, buf_nb, ctx->bit_rate)));

    for (unsigned int i = 0; i < ctx->nb_streams; ++i) {
        g_list_store_append(items, nautilus_properties_item_new("", ""));
        add_av_stream_nautilus_properties_info(items, ctx->streams[i]);
    }
}


GList *
avinfo_extension_get_models(NautilusPropertiesModelProvider *provider,
                            GList *files)
{
    g_autofree GTimer* timer = g_timer_new();
    g_timer_start(timer);

    NautilusFileInfo *file = NAUTILUS_FILE_INFO(g_list_first(files)->data);
    char *mime_type = nautilus_file_info_get_mime_type(file);
    if (strncmp(mime_type, "audio/", 6) != 0 && strncmp(mime_type, "video/", 6) != 0) {
        return NULL;
    }

    AVFormatContext *ctx = avformat_alloc_context();
    avformat_open_input(&ctx, g_file_get_path(nautilus_file_info_get_location(file)), NULL, NULL);
    avformat_find_stream_info(ctx, NULL);

    GListStore *items = g_list_store_new(NAUTILUS_TYPE_PROPERTIES_ITEM);
    add_av_format_context_nautilus_properties_info(items, ctx);

    avformat_close_input(&ctx);
    avformat_free_context(ctx);

    NautilusPropertiesModel *model = nautilus_properties_model_new(dgettext(PACKAGE, "AVInfo Properties"), G_LIST_MODEL(items));
    return g_list_append(NULL, model);
}


void
avinfo_extension_properties_model_provider_interface_init(NautilusPropertiesModelProviderInterface *interface)
{
    interface->get_models = avinfo_extension_get_models;
}
