/*
 * HttpAndLocalCacheTask.h
 *
 *  Created on: 2016年4月25日
 *      Author: evan
 */

#ifndef WEIBOCACHE_VOD_HTTPANDLOCALCACHETASK_H_
#define WEIBOCACHE_VOD_HTTPANDLOCALCACHETASK_H_

#include "VodHttpCache.h"
#include "VodLocalCache.h"
#include "VodWThread.h"
#include "MomoFLVParser.hpp"
#include "ByteBuffer.hpp"

typedef void (*HTTP_CALLBACK)(void*, void*);
typedef void (*NET_STATISTICS_CALLBACK)(void *, char *uuid, char *url, int);
//wait time in ms
#define MAX_WAIT_QUEUE_TIME 300  
class VodHttpAndLocalCacheTask : public VodWThread
{
public:
	VodHttpAndLocalCacheTask();
	virtual ~VodHttpAndLocalCacheTask();

	virtual int routine();
    virtual int stop();
    int stopAll();
    //设置http回调
    int setHttpCallback(void *ptr, void *httpRequestCallback, void *httpResponseCallback, void *httpSetProxyCallback);
    //设置流量统计接口
    int setNetStatisticsCallback(void *ptr, void *statisticsCallback);

    //设置中断回调函数，确认线程是否需要退出
    void registerInterrupCallback(void *ptr, void *fn);
    //初始化缓存模块
    int initCache(const char *url, const char *cfgPath, const char *key, bool forceSync = false);
    //读取数据
    int readCacheData(uint8_t *buf, int size);
    //seek操作
    int64_t seekCacheData(int64_t offset, int whence);
    //开启本地保存缓存线程
    int startSaveCacheTask();
    //check 下载线程是否存在
    bool taskIsRunning();
    //设置缓存下载起始位置
    int setCacheRange(int64_t rangeStart, int rangeSize);
    //缓存同步操作
    void sync();
    //获取通过readCacheData接口下载了多少数据，即播放时下载了多少数据，具有清0操作
    int64_t getDownloadSizeWhilePlaying();
    //获取已缓存数据的大小
    int64_t getCacheSize();
    //获取已缓存的duration
    int getVideoDuration();
    int getAudioDuration();
    int getDefaultDuration();
    int isFLVMetaDataParsed();
    //获取缓存类型
    int getCachetype();
    //设置本缓存网络url
    void setUrl(const char *url) {
        if (url) {
            if (mUrl) {
                free(mUrl);
            }
            mUrl = strdup(url);
        }
    }
    
    void setConfigPath(const char *cfg) {
        if (cfg) {
            mCfgPath = strdup(cfg);
        }
    }
    //获取本任务id
    int getTaskId(){
        return mTaskId;
    }
    //设置本任务id
    void setTaskId(int id) {
        mTaskId = id;
    };
    //设置本任务key
    void setTaskKey(const char *key, const char *userInfo) {
        if (!mKey && key) {
            mKey = strdup(key);
        }
        
        if (!mUserInfo && userInfo) {
            mUserInfo = strdup(userInfo);
        }
        
    }
    //获取本任务key
    char * getTaskKey() {
        return mKey;
    }
    //设置是否为预加载任务
    void setIsPreload(bool flag) {
        mIsPreloadTask = flag;
    }
    //设置加载秒数
    void setPreloadMilliSeconds(int milliSeconds) {
        mPreloadMilliSeconds = milliSeconds;
        mFLVParser.setDuration(mPreloadMilliSeconds);
    }
    
    //调用http请求回调
    int callInjectRequestCallback(void *url, void *header);
    //调用http请求响应回调
    int callInjectResponseCallback(void *url, void *data);
    //调用设置http 代表回调
    int callInjectSetProxyCallback(void *proxyPath);
    //设置http相关options
    void setHttpOptions(void *options) {
        av_dict_copy(&mHttpOptions, (AVDictionary *)options, 0);
        mHttpClient->setHttpOptionDictionary(options);
    };
    uint32_t getErrorCode(uint32_t errorCode) {
        return mWorkFlowCode << 16 | mWorkErrCode;
    }
    //初始化本地缓存
    int localCacheInit(const char *cfgPath, const char *key);
    //由预加载任务转换为播放任务时，检查本地缓存文件是否已被删除，如果已删除，则重置内存数据
    int checkCacheData();
    //获取http相关的打点信息
    int setHttpLogOptions(void*options);
public :
    //Http回调函数
    static void httpRequestCallback(void *, void *, void *);
    //http回调函数
    static void httpResponseCallback(void *, void *url, void *);
    //设置http代理
    static void httpSetProxyCallback(void *, void *proxy);
    //check当前目录下的缓存类型
    static int checkCacheType(const char *path);
    
    //check当前目录缓存大小
    static int checkCacheSize(const char *path);
    
    //设置表明是否删除预加载任务
    void setStopByDestroyTask();
    
    int doDownloadWork1();

private:
    
    void traceWorkflow(int code) {
        mWorkFlowCode = code;
    };
    
    //等待网络数据写入到本地文件成功
    int waitCacheFlushToLocal();
private:
    //记录任务id
    int mTaskId;
    //对应key
    char *mKey;
    //user info
    char *mUserInfo;
    //对应url
    char *mUrl;
    //缓存目录
    char *mCfgPath;
    //网络下载模块
    std::shared_ptr<VodHttpCache> mHttpClient;
    //本地缓存模块
    std::shared_ptr<VodLocalCache> mLocalManager;
    //缓存读offset
	int64_t mReadOffset;
    //文件大小
	int64_t mFileSize;
    //网络下载任务起始位置
	int64_t mRangeStart;
    //网络下载任务大小
	int 	mRangeSize;
    //已缓存数据大小
	int64_t mAlreadyDownloadSize;
    //标志获取到的数据是否为预加载缓存
	bool mIsPlayingPreload;
    //标志是否只从网络读取数据
	bool mOnlyNetData;
    //标志是否需要seek操作
    bool mNeedSeekFlag;
    //标志是否为预加载任务
    bool mIsPreloadTask;
    //记录需要从网络读取数据的大小
	int mDownloadFromNet;
    //url互斥锁
	pthread_mutex_t mLock;
    //标志本任务是否已启动
	bool mTaskIsStart;
    //标志是否已初始化
    bool mInitFlag;
    //标志本地缓存是否已初始化
    bool mLocalInitFlag;
    //重置local cache
    bool mNeedResetLocalCache;
    //标志当前是否被播放
    bool mIsPlaying;
    
    //http回调ptr
    void *mHttpCallbackPtr;
    //http request请求回调
    void *mHttpRequestCallback;
    //http response回调
    void *mHttpResponseCallback;
    //http 设置代理回调
    void *mHttpSetProxyCallback;
    //流量统计回调
    void *mNetStatisticsCallback;
    void *mNetStatisticPtr;
    char mCacheUUID[256];
    
    //记录程序运行位置
    int mWorkFlowCode;
    //记录程序运行错误
    int mWorkErrCode;
    //记录下载的状态
    int mPreloadResult;
    //http 选项
    AVDictionary *mHttpOptions;
    //first receive buffer
    int mFirstFLVBuffer;
    
    pthread_mutex_t mThreadMutex;
    pthread_cond_t mThreadCond;
    int mThreadExit;
    //加载视频的毫秒秒数
    int mPreloadMilliSeconds;
    MomoFLVParser mFLVParser;
    int mIsFLVFormat;
    
    FILE *mFileLog;
    void writeLogHeader();
    void writeLogTailer(int cachedSize);
    int mStopByReadCache;
    int mStopByDestroyTask;
};

typedef enum : int {
    HttpResponseSuccess = 0,
    HttpResponseOpenError = -1,
    HttpResponseReadError = -2,
} HttpResponseStatus;

#endif /* WEIBOCACHE_VOD_HTTPANDLOCALCACHETASK_H_ */
