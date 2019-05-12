/*
 * AutoLock.h
 *
 *  Created on: 2016年4月25日
 *      Author: evan
 */

#ifndef VOD_WEIBOCACHE_AUTOLOCK_H_
#define VOD_WEIBOCACHE_AUTOLOCK_H_

#include <pthread.h>
class VodAutoLock{
public:
	VodAutoLock(pthread_mutex_t &lock):mLock(lock){
		pthread_mutex_lock(&mLock);
	}
	~VodAutoLock(){
		pthread_mutex_unlock(&mLock);
	}
private:
	pthread_mutex_t &mLock;
};


#endif /* VOD_WEIBOCACHE_AUTOLOCK_H_ */
