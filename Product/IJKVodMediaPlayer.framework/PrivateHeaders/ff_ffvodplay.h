/*
 * ff_ffplay.h
 *
 * Copyright (c) 2003 Fabrice Bellard
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

#ifndef ff_ffvodplay_h
#define ff_ffvodplay_h

#include "ff_ffvodplay_def.h"
#include "ff_ffvoderror.h"
#include "ff_ffvodmsg.h"

void      ffvodp_global_init();
void      ffvodp_global_uninit();
void      ffvodp_global_set_log_report(int use_report);
void      ffvodp_global_set_log_level(int log_level);
void      ffvodp_io_stat_register(void (*cb)(const char *url, int type, int bytes));
void      ffvodp_io_stat_complete_register(void (*cb)(const char *url,
                                                   int64_t read_bytes, int64_t total_size,
                                                   int64_t elpased_time, int64_t total_duration));

FFPlayer *ffvodp_create();
void      ffvodp_destroy(FFPlayer *ffp);
void      ffvodp_destroy_p(FFPlayer **pffp);
void      ffvodp_reset(FFPlayer *ffp);

/* set options before ffvodp_prepare_async_l() */
void      ffvodp_set_format_callback(FFPlayer *ffp, ijk_format_control_message cb, void *opaque);

void      ffvodp_set_option(FFPlayer *ffp, int opt_category, const char *name, const char *value);
void      ffvodp_set_option_int(FFPlayer *ffp, int opt_category, const char *name, int64_t value);

int       ffvodp_get_video_codec_info(FFPlayer *ffp, char **codec_info);
int       ffvodp_get_audio_codec_info(FFPlayer *ffp, char **codec_info);
int       ffvodp_get_video_rotate_degrees(FFPlayer *ffp);
/* playback controll */
int       ffvodp_prepare_async_l(FFPlayer *ffp, const char *file_name);
int       ffvodp_start_from_l(FFPlayer *ffp, long msec);
int       ffvodp_start_l(FFPlayer *ffp);
int       ffvodp_pause_l(FFPlayer *ffp);
int       ffvodp_is_paused_l(FFPlayer *ffp);
int       ffvodp_stop_l(FFPlayer *ffp);
int       ffvodp_wait_stop_l(FFPlayer *ffp);
void      ffvodp_enable_toggle_audio(FFPlayer *ffp, int enable);
int       ffvodp_toggle_audio(FFPlayer *ffp, int pending_stream);

void      ffvodp_set_enable_render_info_callback(FFPlayer *ffp,  int enable);
/* all in milliseconds */
int       ffvodp_seek_to_l(FFPlayer *ffp, long msec);
long      ffvodp_get_current_position_l(FFPlayer *ffp);
long      ffvodp_get_duration_l(FFPlayer *ffp);
long      ffvodp_get_playable_duration_l(FFPlayer *ffp);

/* for internal usage */
void      ffvodp_packet_queue_init(PacketQueue *q);
void      ffvodp_packet_queue_destroy(PacketQueue *q);
void      ffvodp_packet_queue_abort(PacketQueue *q);
void      ffvodp_packet_queue_start(PacketQueue *q);
void      ffvodp_packet_queue_flush(PacketQueue *q);
int       ffvodp_packet_queue_get(PacketQueue *q, AVPacket *pkt, int block, int *serial);
int       ffvodp_packet_queue_get_or_buffering(FFPlayer *ffp, PacketQueue *q, AVPacket *pkt, int *serial, int *finished);
int       ffvodp_packet_queue_put(PacketQueue *q, AVPacket *pkt);
bool      ffvodp_is_flush_packet(AVPacket *pkt);

Frame    *ffvodp_frame_queue_peek_writable(FrameQueue *f);
void      ffvodp_frame_queue_push(FrameQueue *f);

int       ffvodp_get_master_sync_type(VideoState *is);
double    ffvodp_get_master_clock(VideoState *is);

void      ffvodp_toggle_buffering_l(FFPlayer *ffp, int start_buffering);
void      ffvodp_toggle_buffering(FFPlayer *ffp, int start_buffering);
void      ffvodp_check_buffering_l(FFPlayer *ffp);

int       ffvodp_video_thread(FFPlayer *ffp);
int       ffvodp_video_refresh_thread(FFPlayer *ffp);

void      ffvodp_set_video_codec_info(FFPlayer *ffp, const char *module, const char *codec);
void      ffvodp_set_audio_codec_info(FFPlayer *ffp, const char *module, const char *codec);

// must be freed with free();
IjkVodMediaMeta *ffvodp_get_meta_l(FFPlayer *ffp);

int       ffvodp_get_server_ip_addr(FFPlayer *ffp, char **ip_addr);
int       ffvodp_get_tcp_conn_time(FFPlayer *ffp);

int ffvodp_h264_sei_data(FFPlayer* ffp, uint8_t **sei);
VideoSize ffvodp_h264_videosize(FFPlayer* ffp);
float ffvodp_get_rate(FFPlayer *ffp);
void ffvodp_set_rate(FFPlayer *ffp, float rate);
uint64_t ffvodp_get_current_pts_l(FFPlayer *ffp);

void ffvodp_set_loop(FFPlayer *ffp, int loop);
int ffvodp_get_loop(FFPlayer *ffp);

long ffvodp_video_read_size(FFPlayer* ffp);
long ffvodp_audio_read_size(FFPlayer* ffp);

long ffvodp_video_cache_duration(FFPlayer* ffp);
long ffvodp_audio_cache_duration(FFPlayer* ffp);

long ffvodp_video_decode_count(FFPlayer* ffp);
long ffvodp_audio_decode_size(FFPlayer* ffp);

long ffvodp_video_render_count(FFPlayer* ffp);
long ffvodp_audio_render_size(FFPlayer* ffp);

uint64_t ffvodp_tcp_connect_time(FFPlayer* ffp);
uint64_t ffvodp_stream_meta_time(FFPlayer* ffp);

uint64_t ffvodp_audio_receive_time(FFPlayer* ffp);
uint64_t ffvodp_video_receive_time(FFPlayer* ffp);

uint64_t ffvodp_video_decode_time(FFPlayer* ffp);
uint64_t ffvodp_audio_decode_time(FFPlayer* ffp);

uint64_t ffvodp_video_render_time(FFPlayer* ffp);
uint64_t ffvodp_video_last_render_time(FFPlayer* ffp);
uint64_t ffvodp_audio_render_time(FFPlayer* ffp);
uint32_t ffvodp_stream_count(FFPlayer* ffp);
double ffvodp_get_video_buffer_duration(FFPlayer* ffp);
void ffvodp_audio_switch_record(FFPlayer *ffp, bool enable);

void ffvodp_audio_get_record_audio_params(FFPlayer *ffp, void **audioParams);
void ffvodp_audio_switch_audio_callback(FFPlayer *ffp, bool enable);

int ffvodp_get_http_detect_info(FFPlayer* ffp, HttpDetecter *httpDetecter);
void ffvodp_set_audio_render(FFPlayer *ffp, int is_open);
void updateRealDisplayAspectRatio(FFPlayer *ffp, int fWidth, int fHeight);


void      ffvodp_track_statistic_l(FFPlayer *ffp, AVStream *st, PacketQueue *q, FFTrackCacheStatistic *cache);
void      ffvodp_audio_statistic_l(FFPlayer *ffp);
void      ffvodp_video_statistic_l(FFPlayer *ffp);
void      ffvodp_statistic_l(FFPlayer *ffp);

int64_t ffvodp_get_property_int64(FFPlayer *ffp, int id, int64_t default_value);
float     ffvodp_get_property_float(FFPlayer *ffp, int id, float default_value);

void      ffvodp_set_video_codec_level(FFPlayer *ffp, int level);
void      ffvodp_set_video_codec_profile(FFPlayer *ffp, int profile);



#endif
