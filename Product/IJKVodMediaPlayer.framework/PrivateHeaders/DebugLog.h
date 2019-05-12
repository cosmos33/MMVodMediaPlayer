//
//  DebugLog.h
//  IJKMediaPlayer
//
//  Created by guowei on 01/06/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#ifndef DebugLog_h
#define DebugLog_h

#include <stdio.h>

//#define __DEBUG_CACHE_LOG__
//#define DOWNLOAD_WORK_LOG
//#define __IJK_VOD_PERF_LOG__

#ifndef __FILE_INFO__
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#ifdef __ANDROID__
#include <android/log.h>
#include <mdlogcpp.h>

#define __USE_MDLOG__

#define IJK_VOD_LOG_UNKNOWN     ANDROID_LOG_UNKNOWN
#define IJK_VOD_LOG_DEFAULT     ANDROID_LOG_DEFAULT

#define IJK_VOD_LOG_VERBOSE     ANDROID_LOG_VERBOSE
#define IJK_VOD_LOG_DEBUG       ANDROID_LOG_DEBUG
#define IJK_VOD_LOG_INFO        ANDROID_LOG_INFO
#define IJK_VOD_LOG_WARN        ANDROID_LOG_WARN
#define IJK_VOD_LOG_ERROR       ANDROID_LOG_ERROR
#define IJK_VOD_LOG_FATAL       ANDROID_LOG_FATAL
#define IJK_VOD_LOG_SILENT      ANDROID_LOG_SILENT


#ifdef __DEBUG_CACHE_LOG__
#define VODVLOG(level, TAG, ...)    ((void)__android_log_vprint(level, TAG, __VA_ARGS__))
#define VODALOG(level, TAG, ...)    ((void)__android_log_print(level, TAG, __VA_ARGS__))
#else
#define VODVLOG(level, TAG, ...)
#define VODALOG(level, TAG, ...)
#endif
#else

#ifdef __APPLE__
#if __has_include(<MDLog/mdlogcpp.h>)
#import <MDLog/mdlogcpp.h>
#define __USE_MDLOG__
#endif
#endif

#define IJK_VOD_LOG_UNKNOWN     0
#define IJK_VOD_LOG_DEFAULT     1

#define IJK_VOD_LOG_VERBOSE     2
#define IJK_VOD_LOG_DEBUG       3
#define IJK_VOD_LOG_INFO        4
#define IJK_VOD_LOG_WARN        5
#define IJK_VOD_LOG_ERROR       6
#define IJK_VOD_LOG_FATAL       7
#define IJK_VOD_LOG_SILENT      8

#ifdef __DEBUG_CACHE_LOG__
#define VODVLOG(level, TAG, ...)    ((void)printf(__VA_ARGS__))
#define VODALOG(level, TAG, ...)    ((void)printf(__VA_ARGS__))
#else
#define VODVLOG(level, TAG, ...)
#define VODALOG(level, TAG, ...)
#endif
#endif

#define IJK_VOD_LOG_TAG "IJKVODMEDIA"
#define IJK_VOD_PLOG_TAG "IJKVODPLAYER"
#define IJK_VOD_TASK_TAG "HTTPTASK"
#define IJK_VOD_CACHE_TAG "IJKVODCACHE"
#define IJK_VOD_MUTEX_TAG "IJKVODMUTEX"
#define IJK_VOD_THREAD_TAG "IJKVODTHREAD"

#ifdef __USE_MDLOG__
#define VODVLOGV(FMT, ...)  __MDLOGV_TAG(IJK_VOD_LOG_TAG, FMT, ##__VA_ARGS__)
#define VODVLOGD(FMT, ...)  __MDLOGD_TAG(IJK_VOD_LOG_TAG, FMT, ##__VA_ARGS__)
#define VODVLOGI(FMT, ...)  __MDLOGI_TAG(IJK_VOD_LOG_TAG, FMT, ##__VA_ARGS__)
#define VODVLOGW(FMT, ...)  __MDLOGW_TAG(IJK_VOD_LOG_TAG, FMT, ##__VA_ARGS__)
#define VODVLOGE(FMT, ...)  __MDLOGE_TAG(IJK_VOD_LOG_TAG, FMT, ##__VA_ARGS__)

#define VODALOGV(FMT, ...)  __MDLOGV_TAG(IJK_VOD_LOG_TAG, FMT, ##__VA_ARGS__)
#define VODALOGD(FMT, ...)  __MDLOGD_TAG(IJK_VOD_LOG_TAG, FMT, ##__VA_ARGS__)
#define VODALOGI(FMT, ...)  __MDLOGI_TAG(IJK_VOD_LOG_TAG, FMT, ##__VA_ARGS__)
#define VODALOGW(FMT, ...)  __MDLOGW_TAG(IJK_VOD_LOG_TAG, FMT, ##__VA_ARGS__)
#define VODALOGE(FMT, ...)  __MDLOGE_TAG(IJK_VOD_LOG_TAG, FMT, ##__VA_ARGS__)
#define VODPLOGI(FMT, ...)  __MDLOGI_TAG(IJK_VOD_PLOG_TAG, FMT, ##__VA_ARGS__)
#define VODPLOGE(FMT, ...)  __MDLOGE_TAG(IJK_VOD_PLOG_TAG, FMT, ##__VA_ARGS__)
#define VODTLOGI(FMT, ...)  __MDLOGI_TAG(IJK_VOD_TASK_TAG, FMT, ##__VA_ARGS__)
#define VODTLOGE(FMT, ...)  __MDLOGE_TAG(IJK_VOD_TASK_TAG, FMT, ##__VA_ARGS__)

#define VOD_LOG_ALWAYS_FATAL(FMT, ...)   do { __MDLOGE_TAG(IJK_VOD_LOG_TAG, FMT, ##__VA_ARGS__); exit(1); } while (0)

#define VODCLOGD(FMT, ...)  __MDLOGD_TAG(IJK_VOD_CACHE_TAG, FMT, ##__VA_ARGS__)

#define MUTEXLOGD(FMT, ...)  __MDLOGD_TAG(IJK_VOD_MUTEX_TAG, FMT, ##__VA_ARGS__)
#define MUTEXLOGE(FMT, ...)  __MDLOGE_TAG(IJK_VOD_MUTEX_TAG, FMT, ##__VA_ARGS__)

#define THREADLOGD(FMT, ...) __MDLOGD_TAG(IJK_VOD_THREAD_TAG, FMT, ##__VA_ARGS__)

#else

#define VODVLOGV(...)  VODVLOG(IJK_VOD_LOG_VERBOSE,   IJK_VOD_LOG_TAG, __VA_ARGS__)
#define VODVLOGD(...)  VODVLOG(IJK_VOD_LOG_DEBUG,     IJK_VOD_LOG_TAG, __VA_ARGS__)
#define VODVLOGI(...)  VODVLOG(IJK_VOD_LOG_INFO,      IJK_VOD_LOG_TAG, __VA_ARGS__)
#define VODVLOGW(...)  VODVLOG(IJK_VOD_LOG_WARN,      IJK_VOD_LOG_TAG, __VA_ARGS__)
#define VODVLOGE(...)  VODVLOG(IJK_VOD_LOG_ERROR,     IJK_VOD_LOG_TAG, __VA_ARGS__)

#define VODALOGV(...)  VODALOG(IJK_VOD_LOG_VERBOSE,   IJK_VOD_LOG_TAG, __VA_ARGS__)
#define VODALOGD(...)  VODALOG(IJK_VOD_LOG_DEBUG,     IJK_VOD_LOG_TAG, __VA_ARGS__)
#define VODALOGI(...)  VODALOG(IJK_VOD_LOG_INFO,      IJK_VOD_LOG_TAG, __VA_ARGS__)
#define VODALOGW(...)  VODALOG(IJK_VOD_LOG_WARN,      IJK_VOD_LOG_TAG, __VA_ARGS__)
#define VODALOGE(...)  VODALOG(IJK_VOD_LOG_ERROR,     IJK_VOD_LOG_TAG, __VA_ARGS__)
#define VODPLOGI(...)  VODALOG(IJK_VOD_LOG_INFO,      IJK_VOD_PLOG_TAG, __VA_ARGS__)
#define VODPLOGE(...)  VODALOG(IJK_VOD_LOG_ERROR,     IJK_VOD_PLOG_TAG, __VA_ARGS__)
#define VODTLOGI(...)  VODALOG(IJK_VOD_LOG_INFO,      IJK_VOD_TASK_TAG, __VA_ARGS__)
#define VODTLOGE(...)  VODALOG(IJK_VOD_LOG_ERROR,     IJK_VOD_TASK_TAG, __VA_ARGS__)
#define VOD_LOG_ALWAYS_FATAL(...)   do { VODALOGE(__VA_ARGS__); exit(1); } while (0)

#define VODCLOGD(...)  VODALOG(IJK_VOD_LOG_DEBUG,     IJK_VOD_CACHE_TAG, __VA_ARGS__)

#define MUTEXLOGD(...)  VODALOG(IJK_VOD_LOG_DEBUG,     IJK_VOD_MUTEX_TAG, __VA_ARGS__)
#define MUTEXLOGE(...)  VODALOG(IJK_VOD_LOG_ERROR,     IJK_VOD_MUTEX_TAG, __VA_ARGS__)

#define THREADLOGD(...) VODALOG(IJK_VOD_LOG_DEBUG,     IJK_VOD_THREAD_TAG, __VA_ARGS__)
#endif
#define __MUTEX_ENABLE__ 

#endif /* DebugLog_h */
