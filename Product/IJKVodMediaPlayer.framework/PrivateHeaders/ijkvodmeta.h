/*
 * ijkvodmeta.h
 *
 * Copyright (c) 2014 Zhang Rui <bbcallen@gmail.com>
 *
 * This file is part of ijkPlayer.
 *
 * ijkPlayer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * ijkPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with ijkPlayer; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef ijkvodmeta_h
#define ijkvodmeta_h

#include <stdint.h>
#include <stdlib.h>

// media meta
#define IJKM_KEY_FORMAT         "format"
#define IJKM_KEY_DURATION_US    "duration_us"
#define IJKM_KEY_START_US       "start_us"
#define IJKM_KEY_BITRATE        "bitrate"
#define IJKM_KEY_VIDEO_STREAM   "video"
#define IJKM_KEY_AUDIO_STREAM   "audio"
#define IJKM_KEY_DATA_SIZE      "datasize"
//added by tricker on 18/04/10 for ktv stream switch
#define IJKM_KEY_AUDIO_STREAM_EXT   "audio_ext"

// stream meta
#define IJKM_KEY_TYPE           "type"
#define IJKM_VAL_TYPE__VIDEO    "video"
#define IJKM_VAL_TYPE__AUDIO    "audio"
#define IJKM_VAL_TYPE__UNKNOWN  "unknown"
#define IJKM_KEY_LANGUAGE       "language"

#define IJKM_KEY_ISCACHED       "is_cached"

#define IJKM_KEY_CODEC_NAME      "codec_name"
#define IJKM_KEY_CODEC_PROFILE   "codec_profile"
#define IJKM_KEY_CODEC_LEVEL     "codec_level"
#define IJKM_KEY_CODEC_LONG_NAME "codec_long_name"
#define IJKM_KEY_CODEC_PIXEL_FORMAT "codec_pixel_format"

// stream: video
#define IJKM_KEY_WIDTH          "width"
#define IJKM_KEY_HEIGHT         "height"
#define IJKM_KEY_FPS_NUM        "fps_num"
#define IJKM_KEY_FPS_DEN        "fps_den"
#define IJKM_KEY_TBR_NUM        "tbr_num"
#define IJKM_KEY_TBR_DEN        "tbr_den"
#define IJKM_KEY_SAR_NUM        "sar_num"
#define IJKM_KEY_SAR_DEN        "sar_den"
#define IJKM_KEY_ROTATION        "rotation"
// stream: audio
#define IJKM_KEY_SAMPLE_RATE    "sample_rate"
#define IJKM_KEY_CHANNEL_LAYOUT "channel_layout"

// reserved for user
#define IJKM_KEY_STREAMS        "streams"


typedef struct IjkVodMediaMeta IjkVodMediaMeta;
typedef struct AVFormatContext AVFormatContext;

IjkVodMediaMeta *ijkvodmeta_create();
void ijkvodmeta_destroy(IjkVodMediaMeta *meta);
void ijkvodmeta_destroy_p(IjkVodMediaMeta **meta);

void ijkvodmeta_lock(IjkVodMediaMeta *meta);
void ijkvodmeta_unlock(IjkVodMediaMeta *meta);

void ijkvodmeta_append_child_l(IjkVodMediaMeta *meta, IjkVodMediaMeta *child);
void ijkvodmeta_set_int64_l(IjkVodMediaMeta *meta, const char *name, int64_t value);
void ijkvodmeta_set_string_l(IjkVodMediaMeta *meta, const char *name, const char *value);
void ijkvodmeta_set_avformat_context_l(IjkVodMediaMeta *meta, AVFormatContext *ic);

// must be freed with free();
const char   *ijkvodmeta_get_string_l(IjkVodMediaMeta *meta, const char *name);
int64_t       ijkvodmeta_get_int64_l(IjkVodMediaMeta *meta, const char *name, int64_t defaultValue);
size_t        ijkvodmeta_get_children_count_l(IjkVodMediaMeta *meta);
// do not free
IjkVodMediaMeta *ijkvodmeta_get_child_l(IjkVodMediaMeta *meta, size_t index);

#endif//ijkvodmeta_h
