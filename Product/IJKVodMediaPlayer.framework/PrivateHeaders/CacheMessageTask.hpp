//
//  CacheMessageTask.hpp
//  IJKMediaPlayer
//
//  Created by guowei on 01/06/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#ifndef CacheMessageTask_hpp
#define CacheMessageTask_hpp

#include <stdio.h>
#include "VodWThread.h"

class CacheMessageTask : public VodWThread
{
public:
    CacheMessageTask(int(*msg_loop)(void*));
    
    ~CacheMessageTask();
    
    virtual int routine();
    
    virtual int stop();
    
private:
    int(*MsgLoop)(void*);
    
};

#endif /* CacheMessageTask_hpp */
