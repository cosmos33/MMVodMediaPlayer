/*
 * ijkplayer_ios.h
 *
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

#include "ijkvodplayer.h"
#import "ijkvodplayer_internal.h"

@class IJKVODSDLGLView;

// ref_count is 1 after open
IjkVodMediaPlayer *ijkvodmp_ios_create(int (*msg_loop)(void*));

void            ijkvodmp_ios_set_glview(IjkVodMediaPlayer *mp, IJKVODSDLGLView *glView);
bool            ijkvodmp_ios_is_videotoolbox_open(IjkVodMediaPlayer *mp);
void            ijkvodmp_ios_set_mute(IjkVodMediaPlayer *mp, bool mute);
NSString * const ijkvodmp_ios_get_server_ip_addr(IjkVodMediaPlayer *mp);
long ijkvodmp_ios_get_stream_size(IjkVodMediaPlayer *mp) ;
long ijkvodmp_ios_get_video_receive_size(IjkVodMediaPlayer *mp);
long ijkvodmp_ios_get_audio_receive_size(IjkVodMediaPlayer *mp);
long ijkvodmp_ios_get_video_cache_duration(IjkVodMediaPlayer *mp);
long ijkvodmp_ios_get_audio_cache_duration(IjkVodMediaPlayer *mp);
long ijkvodmp_ios_get_video_decoder_count(IjkVodMediaPlayer *mp);
long ijkvodmp_ios_get_audio_decoder_size(IjkVodMediaPlayer *mp);
long ijkvodmp_ios_get_video_render_count(IjkVodMediaPlayer *mp);
long ijkvodmp_ios_get_audio_render_size(IjkVodMediaPlayer *mp);
uint64_t ijkvodmp_ios_get_tcp_connect_time(IjkVodMediaPlayer *mp);
uint64_t ijkvodmp_ios_get_stream_meta_time(IjkVodMediaPlayer *mp);
uint64_t ijkvodmp_ios_get_video_receive_time(IjkVodMediaPlayer *mp);
uint64_t ijkvodmp_ios_get_audio_receive_time(IjkVodMediaPlayer *mp);
uint64_t ijkvodmp_ios_get_video_decode_time(IjkVodMediaPlayer *mp);
uint64_t ijkvodmp_ios_get_audio_decode_time(IjkVodMediaPlayer *mp);
uint64_t ijkvodmp_ios_get_video_render_time(IjkVodMediaPlayer *mp);
uint64_t ijkvodmp_ios_get_last_video_render_time(IjkVodMediaPlayer *mp);
uint64_t ijkvodmp_ios_get_audio_render_time(IjkVodMediaPlayer *mp);
uint32_t ijkvodmp_ios_get_stream_count(IjkVodMediaPlayer *mp);

int ijkvodmp_ios_get_sei_data(IjkVodMediaPlayer *mp, uint8_t **sei);
VideoSize ijkvodmp_ios_get_videosize(IjkVodMediaPlayer *mp);
double ijkvodmp_ios_get_video_buffer_duration(IjkVodMediaPlayer *mp);

// for screen record module (wang.qiangqiang)
void ijkvodmp_ios_switch_record(IjkVodMediaPlayer* mp, bool enable);
void ijkvodmp_ios_audio_get_record_audio_params(IjkVodMediaPlayer* mp, void **audioParams);
void ijkvodmp_ios_switch_audio_callback(IjkVodMediaPlayer *mp, bool enable);
void ijkvodmp_ios_set_audio_playback_volume(IjkVodMediaPlayer *mp, float volume);

void ijkvodmp_ios_set_fast_find_stream(IjkVodMediaPlayer *mp, bool is_fast);
void ijkvodmp_ios_set_water_mark(IjkVodMediaPlayer *mp, int default_water, int next_water, int check_water);

void ijkvodmp_ios_set_audioQueueRestartEnable(IjkVodMediaPlayer *mp, bool enable);
