/*
 * ObjectQueue.h
 *
 *  Created on: 2016年4月28日
 *      Author: evan
 */

#ifndef WEIBOCACHE_VOD_OBJECTQUEUE_H_
#define WEIBOCACHE_VOD_OBJECTQUEUE_H_
#include <stddef.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include "DebugLog.h"

#define MAX_QUEUE_SIZE 30

class VodObjectQueue {
    
public:
	VodObjectQueue(int queueSize);
	~VodObjectQueue();

	int getQueueObjSize() {
		return mObjectSize;
	}
	int getQueueSize() {
		return mQueuSize;
	}

	int pushDataToQ(void *data, bool isLock = false);
    
    void* topDataFromQ();

	void* popDataFrmQ(bool &aborFlag, bool isLock = false);

	int abortQueue();

	int resetQueue();
    
    int isInitOK();
    
private:
	int mQueuSize;
	volatile int mObjectSize;
	void* mQueueBuf[MAX_QUEUE_SIZE];

	int mReadPos;
	int mWritePos;
    
#ifdef __MUTEX_ENABLE__
	pthread_mutex_t mLock;
    int mLockStatus;
#else
    sem_t mSemaphore;
    int mSemaphoreStatus;
#endif
};

#endif /* WEIBOCACHE_OBJECTQUEUE_H_ */
