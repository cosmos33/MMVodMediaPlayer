/*****************************************************************************
 * ijksdl_thread.h
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

#ifndef ijkvodsdl_thread_h
#define ijkvodsdl_thread_h

#include <stdint.h>
#include <pthread.h>

typedef enum {
    SDL_THREAD_PRIORITY_LOW,
    SDL_THREAD_PRIORITY_NORMAL,
    SDL_THREAD_PRIORITY_HIGH
} SDL_ThreadPriority;

typedef struct SDL_Thread
{
    pthread_t id;
    int (*func)(void *);
    void *data;
    char name[32];
    int retval;
    
#if defined(__APPLE__)
    // ios使用线程条件锁代替pthread_join，规避ios10下崩溃
    pthread_mutex_t thread_lock;
    pthread_cond_t wait_cond;
    volatile bool need_wait;
#endif
} SDL_Thread;

SDL_Thread *VODSDL_CreateThreadEx(SDL_Thread *thread, int (*fn)(void *), void *data, const char *name);
int         VODSDL_SetThreadPriority(SDL_ThreadPriority priority);
void        VODSDL_WaitThread(SDL_Thread *thread, int *status);
void        VODSDL_DestroyThread(SDL_Thread *thread);

#endif
