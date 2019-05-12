//
//  ReleaseTask.hpp
//  IJKMediaPlayer
//
//  Created by guowei on 02/06/2017.
//  Copyright © 2017 bilibili. All rights reserved.
//

#ifndef ReleaseTask_hpp
#define ReleaseTask_hpp

#include <stdio.h>
#include <list>
#include "VodWThread.h"

class VodHttpAndLocalCacheTask;

class HttpTaskReleaser : public VodWThread
{
    
public:
    HttpTaskReleaser();
    
    ~HttpTaskReleaser();
    
    virtual int routine();
    
    virtual int stop();
    
    void appendTask(VodHttpAndLocalCacheTask *task);
    
private:
    
    std::list<VodHttpAndLocalCacheTask *> mTaskDeleteList;
    
    pthread_mutex_t mTaskDeleteListMutex;
};

#endif /* ReleaseTask_hpp */
