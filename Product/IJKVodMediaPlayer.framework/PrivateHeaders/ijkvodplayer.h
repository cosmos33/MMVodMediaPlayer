/*
 * ijkplayer.h
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

#ifndef ijkvodplayer_h
#define ijkvodplayer_h

#include <stdbool.h>
#include "ff_ffvodmsg_queue.h"

#include "ijkvodmeta.h"

#ifndef MPTRACE
#define MPTRACE VODALOGD
#endif

typedef struct IjkVodMediaPlayer IjkVodMediaPlayer;
typedef struct FFPlayer FFPlayer;
typedef struct SDL_Vout SDL_Vout;
typedef struct HttpDetecter HttpDetecter;

/*-
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_IDLE);
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_INITIALIZED);
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_ASYNC_PREPARING);
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_PREPARED);
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_STARTED);
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_PAUSED);
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_COMPLETED);
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_STOPPED);
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_ERROR);
 MPST_CHECK_NOT_RET(mp->mp_state, MP_STATE_END);
 */

/*-
 * ijkmp_set_data_source()  -> MP_STATE_INITIALIZED
 *
 * ijkmp_reset              -> self
 * ijkmp_release            -> MP_STATE_END
 */
#define MP_STATE_IDLE               0

/*-
 * ijkmp_prepare_async()    -> MP_STATE_ASYNC_PREPARING
 *
 * ijkmp_reset              -> MP_STATE_IDLE
 * ijkmp_release            -> MP_STATE_END
 */
#define MP_STATE_INITIALIZED        1

/*-
 *                   ...    -> MP_STATE_PREPARED
 *                   ...    -> MP_STATE_ERROR
 *
 * ijkmp_reset              -> MP_STATE_IDLE
 * ijkmp_release            -> MP_STATE_END
 */
#define MP_STATE_ASYNC_PREPARING    2

/*-
 * ijkmp_seek_to()          -> self
 * ijkmp_start()            -> MP_STATE_STARTED
 *
 * ijkmp_reset              -> MP_STATE_IDLE
 * ijkmp_release            -> MP_STATE_END
 */
#define MP_STATE_PREPARED           3

/*-
 * ijkmp_seek_to()          -> self
 * ijkmp_start()            -> self
 * ijkmp_pause()            -> MP_STATE_PAUSED
 * ijkmp_stop()             -> MP_STATE_STOPPED
 *                   ...    -> MP_STATE_COMPLETED
 *                   ...    -> MP_STATE_ERROR
 *
 * ijkmp_reset              -> MP_STATE_IDLE
 * ijkmp_release            -> MP_STATE_END
 */
#define MP_STATE_STARTED            4

/*-
 * ijkmp_seek_to()          -> self
 * ijkmp_start()            -> MP_STATE_STARTED
 * ijkmp_pause()            -> self
 * ijkmp_stop()             -> MP_STATE_STOPPED
 *
 * ijkmp_reset              -> MP_STATE_IDLE
 * ijkmp_release            -> MP_STATE_END
 */
#define MP_STATE_PAUSED             5

/*-
 * ijkmp_seek_to()          -> self
 * ijkmp_start()            -> MP_STATE_STARTED (from beginning)
 * ijkmp_pause()            -> self
 * ijkmp_stop()             -> MP_STATE_STOPPED
 *
 * ijkmp_reset              -> MP_STATE_IDLE
 * ijkmp_release            -> MP_STATE_END
 */
#define MP_STATE_COMPLETED          6

/*-
 * ijkmp_stop()             -> self
 * ijkmp_prepare_async()    -> MP_STATE_ASYNC_PREPARING
 *
 * ijkmp_reset              -> MP_STATE_IDLE
 * ijkmp_release            -> MP_STATE_END
 */
#define MP_STATE_STOPPED            7

/*-
 * ijkmp_reset              -> MP_STATE_IDLE
 * ijkmp_release            -> MP_STATE_END
 */
#define MP_STATE_ERROR              8

/*-
 * ijkmp_release            -> self
 */
#define MP_STATE_END                9



#define IJKMP_IO_STAT_READ 1


#define IJKMP_OPT_CATEGORY_FORMAT FFP_OPT_CATEGORY_FORMAT
#define IJKMP_OPT_CATEGORY_CODEC  FFP_OPT_CATEGORY_CODEC
#define IJKMP_OPT_CATEGORY_SWS    FFP_OPT_CATEGORY_SWS
#define IJKMP_OPT_CATEGORY_PLAYER FFP_OPT_CATEGORY_PLAYER


void            ijkvodmp_global_init();
void            ijkvodmp_global_uninit();
void            ijkvodmp_global_set_log_report(int use_report);
void            ijkvodmp_global_set_log_level(int log_level);   // log_level = AV_LOG_xxx
void            ijkvodmp_io_stat_register(void (*cb)(const char *url, int type, int bytes));
void            ijkvodmp_io_stat_complete_register(void (*cb)(const char *url,
                                                           int64_t read_bytes, int64_t total_size,
                                                           int64_t elpased_time, int64_t total_duration));

// ref_count is 1 after open
IjkVodMediaPlayer *ijkvodmp_create(int (*msg_loop)(void*));
void            ijkvodmp_set_format_callback(IjkVodMediaPlayer *mp, ijk_format_control_message cb, void *opaque);

void            ijkvodmp_set_option(IjkVodMediaPlayer *mp, int opt_category, const char *name, const char *value);
void            ijkvodmp_set_option_int(IjkVodMediaPlayer *mp, int opt_category, const char *name, int64_t value);

int             ijkvodmp_get_video_codec_info(IjkVodMediaPlayer *mp, char **codec_info);
int             ijkvodmp_get_audio_codec_info(IjkVodMediaPlayer *mp, char **codec_info);

// must be freed with free();
IjkVodMediaMeta   *ijkvodmp_get_meta_l(IjkVodMediaPlayer *mp);

// preferred to be called explicity, can be called multiple times
// NOTE: ijkvodmp_shutdown may block thread
void            ijkvodmp_shutdown(IjkVodMediaPlayer *mp);

void            ijkvodmp_inc_ref(IjkVodMediaPlayer *mp);

// call close at last release, also free memory
// NOTE: ijkmp_dec_ref may block thread
void            ijkvodmp_dec_ref(IjkVodMediaPlayer *mp);
void            ijkvodmp_dec_ref_p(IjkVodMediaPlayer **pmp);

int             ijkvodmp_set_data_source(IjkVodMediaPlayer *mp, const char *url);
void            ijkvodmp_set_enable_render_info_callback(IjkVodMediaPlayer *mp, int enable);
int             ijkvodmp_prepare_async(IjkVodMediaPlayer *mp);
int             ijkvodmp_start(IjkVodMediaPlayer *mp);
int             ijkvodmp_pause(IjkVodMediaPlayer *mp);
int             ijkvodmp_stop(IjkVodMediaPlayer *mp);
int             ijkvodmp_seek_to(IjkVodMediaPlayer *mp, long msec);
int             ijkvodmp_get_state(IjkVodMediaPlayer *mp);
bool            ijkvodmp_is_playing(IjkVodMediaPlayer *mp);
long            ijkvodmp_get_current_position(IjkVodMediaPlayer *mp);
long            ijkvodmp_get_duration(IjkVodMediaPlayer *mp);
long            ijkvodmp_get_playable_duration(IjkVodMediaPlayer *mp);
void            ijkvodmp_enable_toggle_audio(IjkVodMediaPlayer *mp, int enable);
int             ijkvodmp_toggle_audio(IjkVodMediaPlayer *mp, int pending_stream);

void           *ijkvodmp_get_weak_thiz(IjkVodMediaPlayer *mp);
void           *ijkvodmp_set_weak_thiz(IjkVodMediaPlayer *mp, void *weak_thiz);

/* return < 0 if aborted, 0 if no packet and > 0 if packet.  */
int             ijkvodmp_get_msg(IjkVodMediaPlayer *mp, AVMessage *msg, int block);

void            ijkvodmp_get_metric(IjkVodMediaPlayer *mp);

int64_t         ijkvodmp_get_property_int64(IjkVodMediaPlayer *mp, int id, int64_t default_value);
float           ijkvodmp_get_property_float(IjkVodMediaPlayer *mp, int id, float default_value);

int             ijkvodmp_get_server_ip_addr(IjkVodMediaPlayer *mp, char **ip_addr);
int64_t         ijkvodmp_get_first_video_cost(IjkVodMediaPlayer *mp);
int64_t         ijkvodmp_get_stream_info_cost(IjkVodMediaPlayer *mp);
int             ijkvodmp_get_buffering_count(IjkVodMediaPlayer *mp);
int             ijkvodmp_get_buffering_time(IjkVodMediaPlayer *mp);
int             ijkvodmp_get_bitrate(IjkVodMediaPlayer *mp);

void ijkvodmp_set_playerkey(IjkVodMediaPlayer *mp, const char* player_key);
void ijkvodmp_set_cache_root(IjkVodMediaPlayer *mp, const char* cache_root);
float ijkvodmp_get_rate(IjkVodMediaPlayer *mp);

void ijkvodmp_set_rate(IjkVodMediaPlayer *mp, float rate);

uint64_t ijkvodmp_get_current_pts(IjkVodMediaPlayer *mp);

void            ijkvodmp_set_loop(IjkVodMediaPlayer *mp, int loop);
int             ijkvodmp_get_loop(IjkVodMediaPlayer *mp);

int ijkvodmp_start_from_audio_interrupt(IjkVodMediaPlayer *mp);
void ijkvodmp_ios_set_ignore_vtd_err(IjkVodMediaPlayer *mp, bool ignore_vtd_err);
int ijkvodmp_get_http_detect_info(IjkVodMediaPlayer *mp, HttpDetecter *httpDetecter);

//added by tricker 2018/07/31
void ijkvodmp_set_audio_render(IjkVodMediaPlayer *mp, int is_open);

#endif
