/*
 * LocalCache.h
 *
 *  Created on: 2016年4月26日
 *      Author: evan
 */

#ifndef WEIBOCACHE_VOD_LOCALCACHE_H_
#define WEIBOCACHE_VOD_LOCALCACHE_H_
#include "VodWThread.h"
#include "VodHttpCache.h"
#include <list>
using namespace std;

typedef struct CacheSegment
{
	char *segName;
	int64_t segStartPos;
	int64_t segFileSize;
	CacheSegment *segPrev;
	CacheSegment *segNext;
} CacheSegment;

class VodLocalCache : public VodWThread
{
public :
	VodLocalCache();
	~VodLocalCache();

	virtual int routine();
    //初始化
	int init(const char *cfgPath, const char *key);
    //读取数据
	int readData(uint8_t* buf, int64_t offset, int size);
    //获取空数据大小
	int emptyGap();
    //获取文件大小
	int64_t getFileSize();
    //判断缓存是否完整
	bool isComplete();
    //开启save线程
    int startSaveThread(std::shared_ptr<VodHttpCache> source, int64_t &rangeStart, int &rangeSize, bool isNeedCheckWritePos = true);
    //停止save线程
	int stopWriteThread();
    //是否已开启save线程
	bool isStartSaveThread() {
        return mRunFlag;
    };
    //获取已缓存数据大小
	int64_t getDownloadSize() {
        return mRealSize;
    };
    //获取缓存类型
    int getCachetype() {
        return mPlayingPreloadData ? 2 : 1; mPlayingPreloadData = false;
    };
    //设置预加载标志
	void setIsPlayingPreload(bool is)
    {
        mPlayingPreloadData = is;
    };
    //保存配置文件
    void syncConfig();
    //检查发现错误时，重置文件
    int resetLocalConfig();
    //检测缓存目录是否已被删除
    int checkCacheFolderExist(const char *cfgPath, const char *key);
public:
    //check 某目录下缓冲类型
    static int checkCacheType(const char *path);
    static int checkCacheSize(const char *path);
private:
    //初始化config
	int localConfigInit();
    //保存配置文件
	int saveConfig();
    //从本地缓存中读取数据
	int readFromLocalFile(uint8_t* buf, int size);
    //写入到本地缓存中
	int writeToLocalFile(uint8_t* buf, int size);
    //查找可以读取数据的数据段
	int findReadSegment(int64_t offset, int &size);
    //将新建数据段插入到列表当中
	int insertSegmentToList(CacheSegment *segment, CacheSegment *preSegment = NULL);
    //打开数据段
	int openSegment(CacheSegment *segment);
    //在数据段内进行seek操作
	int seekSegment(CacheSegment *segment, int64_t offset);
    //设置访问位置，返回可读取的数据位置及大小
	int checkDownloadAccessPos(int64_t &startRange, int &size);
    //save数据线程
	int writeDataTask();
    //读取配置文件
	char* readJsonTextFromConfig();
    //写配置文件
	int writeJsonTextToConfig(char *json, int size);
    
private:
    void setErrorCode(int code) {
        mErrorCode = code;
    }

private:
    //缓存路径
	char *mCfgPath;
    //缓存md5key
    char mKeyMd5[40];
    //缓存key
	char *mKey;
    //数据段个数
	int mSegNum;
    //当前数据段与下一个数据段之间的空白大小
	int mEmptyGap;
    //文件大小
	int64_t mFileSize;
    //真实缓存数据大小
	int64_t mRealSize;
    //网络获取数据指针
    std::shared_ptr<VodHttpCache> mHttpSource;
    //数据段结构
	CacheSegment *mCurUsedSeg;
	CacheSegment *mSegmentHead;
	CacheSegment *mSegmentTail;
    //url互斥锁
	pthread_mutex_t mLock;
    //文件访问指针
	URLContext* mUrlContex;
    //缓存预加载标志
	bool mPlayingPreloadData;
    //记录错误信息
    int mErrorCode;
};

#endif /* WEIBOCACHE_LOCALCACHE_H_ */
