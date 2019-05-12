/*
 * ff_cmdutils.h
 *      based on ffmpeg/cmdutils.h
 *
 * Copyright (c) 2000-2003 Fabrice Bellard
 * Copyright (c) 2013 Zhang Rui <bbcallen@gmail.com>
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

#ifndef ff_vodcmdutils
#define ff_vodcmdutils

#include "ff_ffvodinc.h"

void            vod_print_error(const char *filename, int err);
AVDictionary  **vod_setup_find_stream_info_opts(AVFormatContext *s, AVDictionary *codec_opts);
AVDictionary   *vod_filter_codec_opts(AVDictionary *opts, enum AVCodecID codec_id,
                                  AVFormatContext *s, AVStream *st, AVCodec *codec);
double vod_get_rotation(AVStream *st);
#endif
