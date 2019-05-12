/*
 * CInterface.h
 *
 *  Created on: 2016年4月28日
 *      Author: evan
 */

#ifndef VOD_WEIBOCACHE_CINTERFACE_H_
#define VOD_WEIBOCACHE_CINTERFACE_H_
//http callback
#include <stdint.h>
//cache i/o
//extern "C" void *getWorkedTask(char* url, char *key);
//extern "C" int readDataFromCache(void *ptr, uint8_t* buf, int size);
//extern "C" int seekDataFromCache(void *ptr, int64_t offset, int whence);
//extern "C" void releaseCache(void* ptr);
typedef enum {
    CacheManagerNetworkStateUnknown = -1,
    CacheManagerNetworkStateLocalMode = 1,
    CacheManagerNetworkStateAccessNetwork = 2,
} CacheManagerNetworkState;

typedef enum {
    WBVideoHttpRequestTypePlayer = 0,
    WBVideoHttpRequestTypePreload = 1,
} WBVideoHttpRequestType;

typedef void (*ReleaseOCMemory)(void *memPtr);
typedef struct WBCHttpRequestCallBackStruct{
    char *requestUrl;
    char *requestHeader;
    char *responseHeader;
    char *key;
    char *userInfo;
    WBVideoHttpRequestType requestType;
    
    char *newRequestUrl;
    char *newRequestHeader;
    
    ReleaseOCMemory releaseMemFunc;
}WBCHttpRequestCallBackStruct;

typedef struct WBCHttpResponseCallBackStruct{
    char *requestUrl;
    char *requestHeader;
    char *responseHeader;
    char *key;
    char *userInfo;
    int result;
}WBCHttpResponseCallBackStruct;

typedef void (*WBCHttpRequestCallback)(void *ptr, WBCHttpRequestCallBackStruct *httpCallback);
typedef void (*WBCHttpResponseCallback)(void *ptr, WBCHttpResponseCallBackStruct *httpCallback);
typedef char* (*WBCHttpSetProxyCallback)(void *ptr);
typedef void (*MOMONetworkStatisticsCallback)(void *ptr, char *uuid, char *url, int size);
#ifdef __cplusplus
extern "C" {
#endif
    
    void *VodWBCGetWorkedTask(char* url, char *key, char *userInfo, void *options);
    int VodWBCReadDataFromCache(void *ptr, uint8_t* buf, int size);
    int64_t VodWBCSeekDataFromCache(void *ptr, int64_t offset, int whence);
    int VodWBCGetCacheType(void *ptr);
    int64_t VodWBCGetCachedSize(void *ptr);
    void VodWBCReleaseCache(void* ptr);
    void VodWBCRegisterInterruptCallback(void *task, void *ptr, void *fn);
    
    
    void VodWBCSetHttpCallback(void *ptr, WBCHttpRequestCallback requestCallback,
                            WBCHttpResponseCallback responseCallback, WBCHttpSetProxyCallback proxyCallback);

    void MOMOSetNetStatisticsCallback(void *ptr, MOMONetworkStatisticsCallback netStatisticCallback);
    
    void VodWBCCreateCacheManager(const char *cfgPath, int(*msg_loop)(void*));
    int VodWBCCreateNewCacheTask(const char *url, const char *key, const char *userInfo, int64_t rangeStart, int rangeSize, const char *header, const char *value);
    
    int VodWBCCreateNewCacheTaskWithPreloadMilliSeconds(const char *url, const char *key, const char *userInfo, int64_t rangeStart, int rangeSize, const char *header, const char *value, int preloadMilliSeconds);
    
    int VodWBCClearAllTask();
    //删除尚未预加载任务
    int VodWBCClearNeedcacheTask();
    void VodWBCDestoryCachemanger();
    void VodWBCDestroyCacheTask(int taskId, const char *key);
    int VodWBCCheckCacheType(const char *path);
    void VodWBCSetHttpOptions(const char *key, const char *value);
    void VodWBCSetHttpOptionsLong(const char *key, int value);
    
    int VodWBCCheckCacheExist(const char *cachePath, const char *key);
    
    int VodWBCReadCacheStatus(const char *cachePath, const char *key);
    
    int VodWBCRemoveDiskCache(const char *cachePath, const char *key);
    
    void VodWBCSetCacheLimitSize(int64_t cacheLimit);
    
    void VodWBCListExistCaches(const char *cachePath);
    
    void VodWBCClearExpiredCache();
    
    void VodWBCClearExpiredCacheWithPath(const char *cachePath);
    
#ifdef __ANDROID__
    void MomoCacheManagerNetworkState(int state);
    
    void MomoCacheResponseCallback(WBCHttpResponseCallBackStruct *data);
#endif
    void VodSupsendAllPreloadTask();
    
    void VodResumeAllPreloadTask();
    
#ifdef __cplusplus
}
#endif

#endif /* VOD_WEIBOCACHE_CINTERFACE_H_ */
