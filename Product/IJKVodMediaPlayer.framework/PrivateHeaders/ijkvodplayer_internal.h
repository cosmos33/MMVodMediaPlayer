/*
 * ijkplayer_internal.h
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

#ifndef ijkvodplayer_internal_h
#define ijkvodplayer_internal_h

#include <assert.h>
#include "ijkvodsdl/ijkvodsdl.h"
#include "ff_ffvoderror.h"
#include "ff_ffvodplay.h"
#include "ijkvodplayer.h"

typedef struct IjkVodMediaPlayer {
    volatile int ref_count;
    pthread_mutex_t mutex;
    FFPlayer *ffplayer;

    int (*msg_loop)(void*);
    SDL_Thread *msg_thread;
    SDL_Thread _msg_thread;

    int mp_state;
    char *data_source;
    void *weak_thiz;

    int restart_from_beginning;
    int seek_req;
    long seek_msec;
} IjkVodMediaPlayer;

#endif
