/*****************************************************************************
 * ijksdl_mutex.h
 *****************************************************************************
 *
 * copyright (c) 2013 Zhang Rui <bbcallen@gmail.com>
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

#ifndef ijkvodsdl_mutex_h
#define ijkvodsdl_mutex_h

#include <stdint.h>
#include <pthread.h>

#define SDL_MUTEX_TIMEDOUT  1
#define SDL_MUTEX_MAXWAIT   (~(uint32_t)0)

typedef struct SDL_mutex {
    pthread_mutex_t id;
} SDL_mutex;

SDL_mutex  *VODSDL_CreateMutex(void);
void        VODSDL_DestroyMutex(SDL_mutex *mutex);
void        VODSDL_DestroyMutexP(SDL_mutex **mutex);
int         VODSDL_LockMutex(SDL_mutex *mutex);
int         VODSDL_UnlockMutex(SDL_mutex *mutex);

typedef struct SDL_cond {
    pthread_cond_t id;
} SDL_cond;

SDL_cond   *VODSDL_CreateCond(void);
void        VODSDL_DestroyCond(SDL_cond *cond);
void        VODSDL_DestroyCondP(SDL_cond **mutex);
int         VODSDL_CondSignal(SDL_cond *cond);
int         VODSDL_CondBroadcast(SDL_cond *cond);
int         VODSDL_CondWaitTimeout(SDL_cond *cond, SDL_mutex *mutex, uint32_t ms);
int         VODSDL_CondWait(SDL_cond *cond, SDL_mutex *mutex);

#endif

