/*
 * CacheLog.h
 *
 *  Created on: 2016年4月25日
 *      Author: evan
 */

#ifndef VOD_WEIBOCACHE_CACHELOG_H_
#define VOD_WEIBOCACHE_CACHELOG_H_
extern "C" {
#include "libavutil/error.h"
}

//#define _SYSTEM_IOS_
//#define _SYSTEM_ANDROID_
#ifdef __ANDROID__
#include <android/log.h>
#endif

#define TAG "TRUE"

#ifdef __ANDROID__

#define LOG_LEVEL_ERROR ANDROID_LOG_ERROR
#define LOG_LEVEL_DEBUG ANDROID_LOG_DEBUG
#define LOG_LEVEL_INFO ANDROID_LOG_INFO

#else
#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO 2

#endif

//#ifdef WB_DEBUG
int vod_weibo_log_print(int level, const char *tag, ...);

#define CACHELOG_ERR(...)  vod_weibo_log_print(LOG_LEVEL_ERROR, TAG, __VA_ARGS__)
#define CACHELOG_DEG(...)  vod_weibo_log_print(LOG_LEVEL_ERROR, TAG, __VA_ARGS__)
#define CACHELOG_INFO(...)  vod_weibo_log_print(LOG_LEVEL_ERROR, TAG, __VA_ARGS__)
//#else
//
//#define CACHELOG_ERR(...)
//#define CACHELOG_DEG(...)
//#define CACHELOG_INFO(...)
//#endif


#define EIJK_CACHE_INIT_CREATE_FOLDER_FAILED               0x1101
#define EIJK_CACHE_INIT_SEEK_CONFIG_FAILED                 0x1102
#define EIJK_CACHE_INIT_READ_CONFIG_FAILED                 0x1103
#define EIJK_CACHE_INIT_CONFIG_INVALID                      0x114
#define EIJK_CACHE_INIT_PARSE_CONFIG_FAILED                 0x1105
#define EIJK_CACHE_INIT_NET_SYNC_CONNECT_FAILED             0x1106
#define EIJK_CACHE_INIT_NET_ASYNC_CONNECT_FAILED            0x1107

//#define EIJK_CACHE_READ_FAILED                               FFERRTAG(0xF8, 'C', 'R', 'F')
#define EIJK_CACHE_READ_FILESIZE_INVALID                    0x1108
#define EIJK_CACHE_READ_SEGMENT_OPEN_FAILED                 0x1109
#define EIJK_CACHE_READ_SEGMENT_SEEK_FAILED                 0x110a
#define EIJK_CACHE_READ_SEGMENT_READ_FAILED                 0x110b

#define EIJK_CACHE_READ_NET_NOCONNECT                       0x110c
#define EIJK_CACHE_READ_NET_SEEK_FAILED                     0x110d
#define EIJK_CACHE_READ_NET_READ_FAILED                     0x110e
#define EIJK_CACHE_SAVE_DATA_WRITE_FAILED                   0x110f

#define SIJK_CACHE_WORKFLOW_START_READ                      0x1001
#define SIJK_CACHE_WORKFLOW_START_INIT                      0x1002
#define SIJK_CACHE_WORKFLOW_AFTER_LOCAL_INIT                0x1003
#define SIJK_CACKE_WORKFLOW_AFTER_NET_INIT                  0x1004
#define SIJK_CACHE_WORKFLOW_STOP_PRELOAD                    0x1005
#define SIJK_CACHE_WORKFLOW_START_SAVE_THREAD               0x1006
#define SIJK_CACHE_WORKFLOW_READ_FROM_LOCAL                 0x1007
#define SIJK_CACHE_WORKFLOW_READ_FROM_NET                   0x1008

#define SIJK_CACHE_WORKFLOW_READ_OPEN_NET                   0x1009






#endif /* VOD_WEIBOCACHE_CACHELOG_H_ */
