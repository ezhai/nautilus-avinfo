#include <config.h>
#include <glib/gi18n.h>

#include "column-provider.h"


void
avinfo_column_provider_interface_init(NautilusColumnProviderInterface *interface)
{
    interface->get_columns = avinfo_column_provider_get_columns;
}


GList *
avinfo_column_provider_get_columns(NautilusColumnProvider *provider)
{
    GList *ret = NULL;
    ret = g_list_append(ret, nautilus_column_new(AVINFO_DURATION_COL,
                                                 AVINFO_DURATION_ATTR,
                                                 dgettext(PACKAGE, "Duration"),
                                                 dgettext(PACKAGE, "Duration of the audio/video stream.")));
    ret = g_list_append(ret, nautilus_column_new(AVINFO_DIMENSIONS_COL,
                                                 AVINFO_DIMENSIONS_ATTR,
                                                 dgettext(PACKAGE, "Dimensions"),
                                                 dgettext(PACKAGE, "Dimensions of the video stream.")));
    ret = g_list_append(ret, nautilus_column_new(AVINFO_FRAMERATE_COL,
                                                 AVINFO_FRAMERATE_ATTR,
                                                 dgettext(PACKAGE, "Frame Rate"),
                                                 dgettext(PACKAGE, "Frame rate of the video stream.")));
    ret = g_list_append(ret, nautilus_column_new(AVINFO_TOTAL_BITRATE_COL,
                                                 AVINFO_TOTAL_BITRATE_ATTR,
                                                 dgettext(PACKAGE, "Total Bit Rate"),
                                                 dgettext(PACKAGE, "Total bit rate of all streams.")));
    ret = g_list_append(ret, nautilus_column_new(AVINFO_AUDIO_BITRATE_COL,
                                                 AVINFO_AUDIO_BITRATE_ATTR,
                                                 dgettext(PACKAGE, "Audio Bit Rate"),
                                                 dgettext(PACKAGE, "Bit rate of the audio stream.")));
    ret = g_list_append(ret, nautilus_column_new(AVINFO_VIDEO_BITRATE_COL,
                                                 AVINFO_VIDEO_BITRATE_ATTR,
                                                 dgettext(PACKAGE, "Video Bit Rate"),
                                                 dgettext(PACKAGE, "Bit rate of the video stream.")));
    return ret;
}
