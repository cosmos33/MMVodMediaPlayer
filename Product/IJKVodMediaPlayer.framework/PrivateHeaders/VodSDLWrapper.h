//
//  VodSDLWrapper.h
//  IJKMedia
//
//  Created by guowei on 30/06/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#ifndef VodSDLWrapper_h
#define VodSDLWrapper_h

#include <stdio.h>
#include "ijkvodsdl/ijkvodsdl_mutex.h"
#include "ijkvodsdl/ijkvodsdl_vout.h"
#include "ijkvodsdl/ijkvodsdl_thread.h"
#include "ijkvodsdl/ijkvodsdl_aout.h"
#include "ijkvodsdl/ffmpeg/ijkvodsdl_vout_overlay_ffmpeg.h"
#include "ijkvodsdl/ijkvodsdl_error.h"
#include "ijkvodsdl/ijkvodsdl_timer.h"
#ifdef __ANDROID__
#include "../androidCache/CInterface.h"
#include "ijkvodsdl/android/ijkvodsdl_android.h"
#else
#include "cache/VodCInterface.h"
#endif

uint64_t VOD_GetTickHR(void);

int VOD_CondSignal(SDL_cond *cond);

int VOD_LockMutex(SDL_mutex *mutex);

int VOD_UnlockMutex(SDL_mutex *mutex);

void VOD_DestroyMutex(SDL_mutex *mutex);

void VOD_DestroyMutexP(SDL_mutex **mutex);

SDL_mutex *VOD_CreateMutex(void);

int VOD_VoutDisplayYUVOverlay(SDL_Vout *vout, SDL_VoutOverlay *overlay);

void VOD_WaitThread(SDL_Thread *thread, int *status);

void VOD_DestroyThread(SDL_Thread *thread);

void VOD_AoutCloseAudio(SDL_Aout *aout);

void VOD_AoutPauseAudio(SDL_Aout *aout, int pause_on);

void VOD_AoutFlushAudio(SDL_Aout *aout);

double VOD_AoutGetLatencySeconds(SDL_Aout *aout);

void VOD_DestroyCond(SDL_cond *cond);

void VOD_DestroyCondP(SDL_cond **cond);

void VOD_VoutFreeYUVOverlay(SDL_VoutOverlay *overlay);

void VOD_VoutUnrefYUVOverlay(SDL_VoutOverlay *overlay);

void VOD_VoutFree(SDL_Vout *vout);

void VOD_VoutFreeP(SDL_Vout **pvout);

void VOD_EglFree(SDL_Vout *vout);

void VOD_EglFreeP(SDL_Vout *vout);

void VOD_AoutFree(SDL_Aout *aout);

void VOD_AoutFreeP(SDL_Aout **paout);

int VOD_CondWait(SDL_cond *cond, SDL_mutex *mutex);

int VOD_CondWaitTimeout(SDL_cond *cond, SDL_mutex *mutex, uint32_t ms);

SDL_cond *VOD_CreateCond(void);

SDL_Thread *VOD_CreateThreadEx(SDL_Thread *thread, int (*fn)(void *), void *data, const char *name);

SDL_VoutOverlay *VOD_Vout_CreateOverlay(int width, int height, Uint32 format, SDL_Vout *vout);

int VOD_VoutLockYUVOverlay(SDL_VoutOverlay *overlay);

int VOD_VoutFFmpeg_ConvertFrame(SDL_VoutOverlay *overlay, AVFrame *frame,
                                struct SwsContext **p_sws_ctx, int sws_flags);


int VOD_VoutUnlockYUVOverlay(SDL_VoutOverlay *overlay);

char *VOD_getenv(const char *name);

const char *VOD_GetError(void);

int VOD_AoutOpenAudio(SDL_Aout *aout, const SDL_AudioSpec *desired, SDL_AudioSpec *obtained);

void VOD_AoutSetDefaultLatencySeconds(SDL_Aout *aout, double latency);

void VOD_ProfilerReset(SDL_Profiler* profiler, int max_sample);

void VOD_Delay(Uint32 ms);


void *VODGetWorkedTask(char *fileName, char *key, char *userInfo, void *data);
void VODRegisterInterruptCallback(void *task, void *ptr, void *fn);
void VODReleaseCache(void* ptr);
int VODGetIsCached(void* ptr);

int VODReadDataFromCache(void *ptr, uint8_t *buf, int bufSize);
int64_t VODSeekDataFromCache(void *ptr, int64_t offset, int whence);
int VODGetHttpDectInfo(void* task, void *data);

#endif /* VodSDLWrapper_h */
