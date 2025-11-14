#pragma once

#include <glib.h>
#include <nautilus-extension.h>

#define AVINFO_DURATION_ATTR "xattr::nautilus-avinfo::duration"
#define AVINFO_DIMENSIONS_ATTR "xattr::nautilus-avinfo::dimensions"
#define AVINFO_FRAMERATE_ATTR "xattr::nautilus-avinfo::framerate"
#define AVINFO_TOTAL_BITRATE_ATTR "xattr::nautilus-avinfo::total_bitrate"
#define AVINFO_AUDIO_BITRATE_ATTR "xattr::nautilus-avinfo::audio_bitrate"
#define AVINFO_VIDEO_BITRATE_ATTR "xattr::nautilus-avinfo::video_bitrate"
#define AVINFO_LAST_COMPUTED_ATTR "xattr::nautilus-avinfo::last_computed"

#define AVINFO_DURATION_COL "xattr::nautilus-avinfo::duration_column"
#define AVINFO_DIMENSIONS_COL "xattr::nautilus-avinfo::dimensions_column"
#define AVINFO_FRAMERATE_COL "xattr::nautilus-avinfo::framerate_column"
#define AVINFO_TOTAL_BITRATE_COL "xattr::nautilus-avinfo::total_bitrate_column"
#define AVINFO_AUDIO_BITRATE_COL "xattr::nautilus-avinfo::audio_bitrate_column"
#define AVINFO_VIDEO_BITRATE_COL "xattr::nautilus-avinfo::video_bitrate_column"

void avinfo_column_provider_interface_init(NautilusColumnProviderInterface *interface);

GList *avinfo_column_provider_get_columns(NautilusColumnProvider *provider);
