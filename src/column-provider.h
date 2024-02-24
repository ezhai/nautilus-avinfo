#pragma once

#include <glib.h>
#include <nautilus-extension.h>

#define EXT_DURATION_ATTR "AVInfo::duration"
#define EXT_DIMENSIONS_ATTR "AVInfo::dimensions"
#define EXT_FRAMERATE_ATTR "AVInfo::framerate"
#define EXT_TOTAL_BITRATE_ATTR "AVInfo::total_bitrate"
#define EXT_AUDIO_BITRATE_ATTR "AVInfo::audio_bitrate"
#define EXT_VIDEO_BITRATE_ATTR "AVInfo::video_bitrate"

#define EXT_DURATION_COL "AVInfo::duration_column"
#define EXT_DIMENSIONS_COL "AVInfo::dimensions_column"
#define EXT_FRAMERATE_COL "AVInfo::framerate_column"
#define EXT_TOTAL_BITRATE_COL "AVInfo::total_bitrate_column"
#define EXT_AUDIO_BITRATE_COL "AVInfo::audio_bitrate_column"
#define EXT_VIDEO_BITRATE_COL "AVInfo::video_bitrate_column"

void avinfo_extension_column_provider_interface_init(NautilusColumnProviderInterface *interface);

GList *avinfo_extension_get_columns(NautilusColumnProvider *provider);
