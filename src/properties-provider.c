#include <config.h>
#include <glib/gi18n.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/dict.h>
#include <libavutil/pixdesc.h>
#include <nautilus-extension.h>

#include "format.h"

#include "properties-provider.h"

/**
 * add_nautilus_property:
 * 
 * Adds a property to the GListStore of NautilusPropertiesItem.
 * If the prefix is not an empty string, the property name is prefixed with it.
 * 
 */
void
add_nautilus_property(GListStore *items, char *prefix, char *name, const char *value)
{
    size_t name_copy_nb = 64;
    char name_copy[name_copy_nb];

    if (prefix[0] == '\0') {
        g_list_store_append(items, nautilus_properties_item_new(name, value));
    } else {
        g_snprintf(name_copy, name_copy_nb, "%s %s", prefix, name);
        g_list_store_append(items, nautilus_properties_item_new(name_copy, value));
    }
}

/**
 * add_av_stream_nautilus_properties:
 * 
 * Adds properties of an input stream to the list of NautilusPropertiesItem.
 * 
 */
void
add_av_stream_nautilus_properties(GListStore *items, AVStream *stream)
{
    size_t prefix_nb = 32, buf_nb = 128;
    gchar prefix[prefix_nb], buf[buf_nb];

    AVCodecParameters *params = stream->codecpar;
    if (params == NULL) {
        return;
    }
  
    g_snprintf(prefix, prefix_nb, dgettext(PACKAGE, "Stream %d -"), stream->id);
    const char *type = av_get_media_type_string(params->codec_type);

    add_nautilus_property(items, prefix, dgettext(PACKAGE, "Type"), type);
    add_nautilus_property(items, prefix, dgettext(PACKAGE, "Codec"), avcodec_descriptor_get(params->codec_id)->long_name);
    add_nautilus_property(items, prefix, dgettext(PACKAGE, "Codec Name"), avcodec_descriptor_get(params->codec_id)->name);
    add_nautilus_property(items, prefix, dgettext(PACKAGE, "Codec Tag"), av_fourcc2str(params->codec_tag));

    if (strcmp(type, "video") == 0) {
        add_nautilus_property(items, prefix, dgettext(PACKAGE, "Codec Profile"), av_get_profile_name(avcodec_find_decoder(params->codec_id), params->profile));
        if (stream->avg_frame_rate.num && stream->avg_frame_rate.den) {
            add_nautilus_property(items, prefix, dgettext(PACKAGE, "Frame Rate"), format_framerate(buf, buf_nb, av_q2d(stream->avg_frame_rate)));
        }
        if (stream->r_frame_rate.num && stream->r_frame_rate.den) {
            add_nautilus_property(items, prefix, dgettext(PACKAGE, "Time Base Real"), format_string(buf, buf_nb, "%.1f tbr", av_q2d(stream->r_frame_rate)));
        }
        if (stream->time_base.num && stream->time_base.den) {
            add_nautilus_property(items, prefix, dgettext(PACKAGE, "Time Base Number"), format_basenumber(buf, buf_nb, av_q2d(av_inv_q(stream->time_base))));
        }
        add_nautilus_property(items, prefix, dgettext(PACKAGE, "Dimensions"), format_dimensions(buf, buf_nb, params->width, params->height));
        add_nautilus_property(items, prefix, dgettext(PACKAGE, "Video Bit Rate"), format_bitrate(buf, buf_nb, params->bit_rate));
        add_nautilus_property(items, prefix, dgettext(PACKAGE, "Color Format"), av_get_pix_fmt_name(params->format));
        add_nautilus_property(items, prefix, dgettext(PACKAGE, "Color Range"), av_color_range_name(params->color_range));
        add_nautilus_property(items, prefix, dgettext(PACKAGE, "Color Space"), av_color_space_name(params->color_space));
        add_nautilus_property(items, prefix, dgettext(PACKAGE, "Color Chroma Location"), av_chroma_location_name(params->color_primaries));
    } else if (strcmp(type, "audio") == 0) {
        add_nautilus_property(items, prefix, dgettext(PACKAGE, "Codec Profile"), av_get_profile_name(avcodec_find_decoder(params->codec_id), params->profile));
        add_nautilus_property(items, prefix, dgettext(PACKAGE, "Audio Bit Rate"), format_bitrate(buf, buf_nb, params->bit_rate));
        add_nautilus_property(items, prefix, dgettext(PACKAGE, "Channels"), format_string(buf, buf_nb, "%d", params->ch_layout.nb_channels));
        add_nautilus_property(items, prefix, dgettext(PACKAGE, "Sample Format"), av_get_sample_fmt_name(params->format));
        add_nautilus_property(items, prefix, dgettext(PACKAGE, "Sample Rate"), format_string(buf, buf_nb, "%d Hz", params->sample_rate));
    } else if (strcmp(type, "data") == 0) {
    }
}

/**
 * add_av_metdata_nautilus_properties:
 * 
 * Adds properties of the input metdata to the list of NautilusPropertiesItem.
 * 
 */
void
add_av_metdata_nautilus_properties(GListStore *items, AVDictionary *metadata)
{
    size_t prefix_nb = 32;
    gchar prefix[prefix_nb];

    g_snprintf(prefix, prefix_nb, dgettext(PACKAGE, "Metadata -"));

    AVDictionaryEntry *it = NULL;
    while ((it = av_dict_get(metadata, "", it, AV_DICT_IGNORE_SUFFIX))) {
        add_nautilus_property(items, prefix, it->key, it->value);
    }
}

/**
 * add_av_format_context_nautilus_properties:
 *
 * Adds properties of the input context to the list of NautilusPropertiesItem.
 *
 */
void
add_av_format_context_nautilus_properties(GListStore *items, AVFormatContext *ctx)
{
    size_t buf_nb = 16;
    gchar buf[buf_nb];

    if (ctx->iformat) {
        g_list_store_append(items, nautilus_properties_item_new(dgettext(PACKAGE, "Format"), ctx->iformat->long_name));
    }
    g_list_store_append(items, nautilus_properties_item_new(dgettext(PACKAGE, "Duration"), format_duration(buf, buf_nb, ctx->duration)));
    g_list_store_append(items, nautilus_properties_item_new(dgettext(PACKAGE, "Total Bit Rate"), format_bitrate(buf, buf_nb, ctx->bit_rate)));

    if (ctx->metadata) {
        g_list_store_append(items, nautilus_properties_item_new("", ""));
        add_av_metdata_nautilus_properties(items, ctx->metadata);
    }
    for (size_t i= 0; i < ctx->nb_streams; ++i) {
        g_list_store_append(items, nautilus_properties_item_new("", ""));
        add_av_stream_nautilus_properties(items, ctx->streams[i]);
    }
}

void
avinfo_properties_model_provider_interface_init(NautilusPropertiesModelProviderInterface *interface)
{
    interface->get_models = avinfo_properties_model_provider_get_models;
}

GList *
avinfo_properties_model_provider_get_models(NautilusPropertiesModelProvider *provider, GList *files)
{
    NautilusFileInfo *file_info = NAUTILUS_FILE_INFO(g_list_first(files)->data);
    g_autofree char *mime_type = nautilus_file_info_get_mime_type(file_info);
    if (strncmp(mime_type, "audio/", 6) != 0 && strncmp(mime_type, "video/", 6) != 0) {
        return NULL;
    }

    g_autoptr(GFile) file = nautilus_file_info_get_location(file_info);
    g_autofree char *file_path = g_file_get_path(file);
    if (file_path == NULL) {
        return NULL;
    }

    AVFormatContext *ctx = avformat_alloc_context();
    int retcode = avformat_open_input(&ctx, file_path, NULL, NULL);
    if (retcode != 0) {
        avformat_free_context(ctx);
        return NULL;
    }
    avformat_find_stream_info(ctx, NULL);

    GListStore *items = g_list_store_new(NAUTILUS_TYPE_PROPERTIES_ITEM);
    add_av_format_context_nautilus_properties(items, ctx);

    avformat_close_input(&ctx);
    avformat_free_context(ctx);

    NautilusPropertiesModel *model = nautilus_properties_model_new(dgettext(PACKAGE, "AVInfo Properties"), G_LIST_MODEL(items));
    return g_list_append(NULL, model);
}