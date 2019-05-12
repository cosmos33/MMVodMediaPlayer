/*
 * CacheManager.h
 *
 *  Created on: 2016年4月25日
 *      Author: evan
 */

#ifndef VOD_WEIBOCACHE_CACHEMANAGER_H_
#define VOD_WEIBOCACHE_CACHEMANAGER_H_
#include <stddef.h>
#include <stdint.h>
#include <pthread.h>
#include <list>
#include <map>
#include <mutex>
#include <shared_mutex>
#include "VodHttpAndLocalCacheTask.h"
#include "VodWThread.h"
extern "C"{
#include "libavutil/dict.h"
}

typedef std::mutex SharedMutex;
typedef std::lock_guard<SharedMutex> ReadLock;
typedef std::lock_guard<SharedMutex> WriteLock;

using namespace  std;

class CacheMessageTask;
class HttpTaskReleaser;
class MMPlayerCacheBridge;

class VodCacheManager : public VodWThread {
public :
	VodCacheManager(const char *cfgPath, int(*msg_loop)(void*));
	~VodCacheManager();

	static VodCacheManager *getInstance();
    //创建manager
	static int createCacheManager(const char *cfgPath, int(*msg_loop)(void*));
    //销毁cache manager
	static void destoryCachemanger();
    //创建新的缓存任务
	int createNewCacheTask(const char *url, const char *key, const char *userInfo, int64_t rangeStart, int rangeSize, const char *header, const char *value, int preloadMilliSeconds = 2000);
    //销毁某一任务
	int destroyCacheTask(int taskId, const char *key);
    //销毁某一任务
    int destroyCacheTaskByHandle(std::shared_ptr<VodHttpAndLocalCacheTask> node);
    //获取任务
    MMPlayerCacheBridge* popCacheTaskNode(int taskId, const char *url, const char *key, const char *userInfo, void *options);
    //设置缓存路径
	int setCacheCfgPath(const char *path);
    //
	virtual int routine();
    //清除所有任务
	int clearAllTask();
    //删除尚未预加载的任务
    int clearNeedCacheTask();
    //注入http相关的callback , request callback and response callback
    int injectHttpCallback(void *ptr, void *requestback, void* responseback, void *proxyback){
        if(!requestback || !responseback){
            return -1;
        }
        mCallbackPtr = ptr;
        mRequestCallback = requestback;
        mResponseCallback = responseback;
        mSetProxyCallback = proxyback;
        return 0;
    };
    //设置http相关options
    void setHttpOptions(const char *key, const char *value){
        av_dict_set(&mHttpOptions, key, value, 0);
    }
    void setHttpOptionsLong(const char *key, int value){
        av_dict_set_int(&mHttpOptions, key, value, 0);
    }
    
    //检测缓存是否存在
    int checkCacheExist(const char *cachePath, const char *key);
    //
    int readCacheStatus(const char *cachePath, const char *key);
    // 删除指定URL的disk cache
    int removeDiskCache(const char *cachePath, const char *key);
    // 暂停预加载任务
    void suspendTaskList();
    // 恢复预加载任务
    void resumeTaskList();

    int injectNetStatisticsCallback(void *ptr, void *statisticsCallback){
        if(!statisticsCallback){
            return -1;
        }
        
        mNetStatisticsCallback = statisticsCallback;
        mNetStatisticPtr = ptr;
        return 0;
    };
private:
    //获任务节点
    std::shared_ptr<VodHttpAndLocalCacheTask> getCacheTaskNode(int taskId, const char *key);
    //从队列中按id查找任务
    std::shared_ptr<VodHttpAndLocalCacheTask> findTaskById(int taskId);
    //从队列中按key查找任务
    std::shared_ptr<VodHttpAndLocalCacheTask> findTaskByKey(const char *key);
    //将任务从队列中删除
    int deleteTaskNode(std::shared_ptr<VodHttpAndLocalCacheTask> node);
    //预加载任务队列
	int workQueueTask();
    //将由播放时创建的工作任务加入到down 队列
    int pushTaskToBindPlayerPool(std::shared_ptr<VodHttpAndLocalCacheTask> taskNode);
    
    //持有锁时间过长，则触发alarm提示
    int lockWithTimeOut(const char *message);
    int unLock(const char *message);
    static void handleAlarm(int num);
    
private:
    //当前预加载任务队列
    list<std::shared_ptr<VodHttpAndLocalCacheTask>> mNeedCacheTaskPool;
    //已完成预加载任务和当前播放任务队列
    list<std::shared_ptr<VodHttpAndLocalCacheTask>> mDoneCacheTaskPool;
    //播放任务引用个数
    map<int, int >   mPlayerRefernce;
    //任务个数,用于设置id
    int mTaskNum;
    //当前预加载任务
    std::shared_ptr<VodHttpAndLocalCacheTask> mCurWorkedTask;
    //互斥锁
    SharedMutex mMutex;
    //配置文件路径
	char *mConfigPath;

    //http请求的callback
    void *mCallbackPtr;
    void *mRequestCallback;
    void *mResponseCallback;
    void *mSetProxyCallback;
    void *mNetStatisticsCallback;
    void *mNetStatisticPtr;
    
    //http options
    AVDictionary *mHttpOptions;
private:
	static VodCacheManager *mInstance;
    pthread_t cacheMessageThread;
#ifdef __ANDROID__
    CacheMessageTask* mCacheMessageTask;
#endif
    bool mPausePreloadTask;
};



#endif /* VOD_WEIBOCACHE_CACHEMANAGER_H_ */
