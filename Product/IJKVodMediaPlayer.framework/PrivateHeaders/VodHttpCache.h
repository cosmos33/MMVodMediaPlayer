/*
 * HttpCache.h
 *
 *  Created on: 2016年4月26日
 *      Author: evan
 */

#ifndef WEIBOCACHE_VOD_HTTPCACHE_H_
#define WEIBOCACHE_VOD_HTTPCACHE_H_
#include "VodObjectQueue.h"
#include "VodWThread.h"
extern "C" {
#include "libavformat/url.h"
#include "libavformat/avio.h"
#include "libavutil/dict.h"
#include "libavutil/error.h"
}
#include "ByteBuffer.hpp"

#define VOD_CACHE_ERROR_EARLY_EXIT -10 // 王旭, 开始前异步调用了关闭。用于正确放弃开始流程

#define QUEUE_SIZE 3
#define QUEUE_BUFFER_SIZE 32768
#define GET_QUEUE_ELEMENT_ERROR ('d' << 24 | 'e'<<16|'q'<<8|'u')
struct QueueData
{
	uint8_t* buf;
	int bufSize;
	int dataSize;

};

typedef void (*TASK_FUNC)(void *);

class VodTaskThread:public VodWThread
{
public:
    VodTaskThread();
    virtual ~VodTaskThread();
    virtual int routine() ;
    int doTask();
    void setTask(void *ptr, TASK_FUNC fn);
private:
    void *mTaskPtr;
    TASK_FUNC mTaskFun;
};

class VodHttpCache
{
public:
	VodHttpCache();
	~VodHttpCache();
    //http 连接中设置的回调函数
    static int decodeInterruptCb(void* ptr) {
        VodHttpCache *http = (VodHttpCache*)ptr;
        return http->decodeInterrupCallback();
    };
    
    int decodeInterrupCallback() {
        MUTEXLOGD("%s called", __func__);
        if(mOuterInterruptCallback && mOuterInterruptePtr)
            return ((int (*)(void*))mOuterInterruptCallback)(mOuterInterruptePtr);
        return 0;
    }
    //注册回调函数
    int registerInterruptCallback(void*ptr, void *fn);
    //设置http options
    int setHttpOptions(const char *name, const char *value)
    {
        av_dict_set(&mOptions, name, value, 0);
        return 0;
    };
    int setHttpOptionsLong(const char *name, int value) {
        av_dict_set_int(&mOptions, name, value, 0);
        return 0;
    };
    int setHttpOptionDictionary(void *src) {
        av_dict_copy(&mOptions, (AVDictionary*)src, 0);
        return 0;
    };
    //设置http回调函数
	int setHttpCallback(void *ptr, void *httpRequestCallback, void* httpResponseCallback, void *httpProxyCallback = NULL){
        mInterruptCallback.opaque = this;
//        mInterruptCallback.url_info_callback.request_callback =  NULL;//(void(*)(void*, const char *, const char*))httpResponseCallback;
//        mInterruptCallback.url_info_callback.response_callback = (void(*)(void*, const char *, const char*))httpResponseCallback;
//        mInterruptCallback.url_info_callback.change_location_callback = (void(*)(void*, char **, char**))httpRequestCallback;
//        mInterruptCallback.url_info_callback.http_proxy_callback = (void(*)(void *, char **))httpProxyCallback;
//        mInterruptCallback.url_info_callback.opaque = ptr;
        mInterruptCallback.callback = decodeInterruptCb;

		return 0;
	}
    //打开连接
	int openUrl(const char *url, bool isAsync = true);
    //读取数据
	int readData(uint8_t* buf, uint64_t offset, int size, bool needSeek);
    //读取数据
    int readDataSafe(uint8_t* buf, uint64_t offset, int size, bool needSeek);
    //获取文件大小
	int getFileSize();
    //发送终止信号，使阻塞到队列的线程退出
	int sendStopSignal();
    //获取数据
	int getPacketData(uint8_t* &buf, int &size, uint64_t &bufId);
    //释放数据
	int releasePacket(uint64_t bufId);
    //关闭连接
	int closeUrl();
    
    //判断数据队列中是否仍有数据
	int getTheRestOfQueue() {
        if (mExitSignal) {
            return 0;
        } else {
            return mDataQue->getQueueObjSize();
        }
	}
    //设置需要从网络获取数据的位置
	int seekDownloadPos(int64_t offset);
    //下载数据
	int downloadPacketData(int rangeSize);
    int downloadPacketData(int rangeSize, bb::ByteBuffer& byteBuffer);
    int downloadPacketData(uint8_t *buffer, int bufferSize, int rangeSize);
    int updatePacketQueue(uint8_t *buffer, int bufferSize);
    //初始化下载队列
	int initQueue();
	int deleteQueue();
    //设置是否需要放入到队列
	void setPushLocalErrFlag(bool flag) {
        mPushDataToLocalError = flag;
    };
    //异步连接任务
    void doAsyncConnect();
    //获取http log信息
    void setHttpLogOptions(void* options);
    //获取init 状态
    int isInitOK();
private:
    int doHttpOpen();
    //将一帧数据放入到队列中
	int pushDataToQueue(uint8_t *buf, int size);
    //队列reset
	int queueDataReset();
    //创建队列
	int queueDataDelete();
    //销毁队列
	int queueDataCreate();
    
    void setErrorCode(int code) {
        mErrorCode = code;
    }
private:
    //url
	char *mUrl;
    //终止标志
    volatile bool mExitSignal;
    //读取数据的上下文
	URLContext *mUrlContex;
    //http回调结构
    AVIOInterruptCB mInterruptCallback;
    //http url的option选项
    AVDictionary *mOptions;
    AVDictionary **mLogOptions;
    //互斥锁
	pthread_mutex_t mLock;
    int mLockStatus;
    //队列数据结构
    std::shared_ptr<VodObjectQueue> mDataQue;
    std::shared_ptr<VodObjectQueue> mSlotQue;
	QueueData *mQueData[QUEUE_SIZE]; //for recycle;

    //是否需要向save线程输出数据
	bool mPushDataToLocalError;
    //设置中断函数及ptr
    void *mOuterInterruptePtr;
    void *mOuterInterruptCallback;
    //是否异步连接
    bool mAsyncConnect;
    //异步连接是否完成
    bool mIsAsyncConDone;
    //异步连接返回值
    int mAsyncRetValue;
    //异步连接任务线程
    VodTaskThread mAsyncTask;
    //记录错误信息
    int mErrorCode;
};

#endif /* WEIBOCACHE_HTTPCACHE_H_ */
