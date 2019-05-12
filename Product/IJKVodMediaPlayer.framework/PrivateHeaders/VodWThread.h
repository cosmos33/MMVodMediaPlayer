/*
 * WThread.h
 *
 *  Created on: 2016年4月26日
 *      Author: evan
 */

#ifndef WEIBOCACHE_VOD_WTHREAD_H_
#define WEIBOCACHE_VOD_WTHREAD_H_
#include <stddef.h>
#include <stdint.h>
#include <pthread.h>

class VodWThread{
public:
	VodWThread();
	~VodWThread();
	virtual int routine() = 0;
	virtual int stop();
	static void *taskRun(void *data);
	int taskWork();
	int start();
	int wait();
    int getThreadStatus();
    bool getRunStatus(){return mRunFlag;};
public :
	volatile bool mRunFlag;
	pthread_mutex_t mThreadLock;

private:
	pthread_t mThreadId;
    int mThreadStatus;
    
    //线程条件锁
    pthread_cond_t mConLock;
    volatile bool mWaitFlag;
};



#endif /* WEIBOCACHE_WTHREAD_H_ */
