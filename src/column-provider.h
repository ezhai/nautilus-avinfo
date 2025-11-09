#pragma once

#include <glib.h>
#include <nautilus-extension.h>

#define EXT_ATTR_WILDCARD "xattr::*"
#define EXT_DURATION_ATTR "xattr::nautilus-avinfo::duration"
#define EXT_DIMENSIONS_ATTR "xattr::nautilus-avinfo::dimensions"
#define EXT_FRAMERATE_ATTR "xattr::nautilus-avinfo::framerate"
#define EXT_TOTAL_BITRATE_ATTR "xattr::nautilus-avinfo::total_bitrate"
#define EXT_AUDIO_BITRATE_ATTR "xattr::nautilus-avinfo::audio_bitrate"
#define EXT_VIDEO_BITRATE_ATTR "xattr::nautilus-avinfo::video_bitrate"
#define EXT_LAST_COMPUTED_ATTR "xattr::nautilus-avinfo::last_computed"

#define EXT_DURATION_COL "xattr::nautilus-avinfo::duration_column"
#define EXT_DIMENSIONS_COL "xattr::nautilus-avinfo::dimensions_column"
#define EXT_FRAMERATE_COL "xattr::nautilus-avinfo::framerate_column"
#define EXT_TOTAL_BITRATE_COL "xattr::nautilus-avinfo::total_bitrate_column"
#define EXT_AUDIO_BITRATE_COL "xattr::nautilus-avinfo::audio_bitrate_column"
#define EXT_VIDEO_BITRATE_COL "xattr::nautilus-avinfo::video_bitrate_column"

void avinfo_column_provider_interface_init(NautilusColumnProviderInterface *interface);

GList *avinfo_column_provider_get_columns(NautilusColumnProvider *provider);
