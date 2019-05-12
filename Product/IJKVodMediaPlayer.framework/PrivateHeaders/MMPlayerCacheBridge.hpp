//
//  MMPlayerCacheBridge.hpp
//  IJKMedia
//
//  Created by guowei on 11/07/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#ifndef MMPlayerCacheBridge_hpp
#define MMPlayerCacheBridge_hpp

#include <stdio.h>
#include <iostream>

class VodHttpAndLocalCacheTask;

class MMPlayerCacheBridge
{
public:
    MMPlayerCacheBridge();
    
    ~MMPlayerCacheBridge();
    
    void setHttpAndLocalCacheTask(std::shared_ptr<VodHttpAndLocalCacheTask> task);
    
    std::shared_ptr<VodHttpAndLocalCacheTask> getHttpAndLocalCacheTask();
    
private:

    std::shared_ptr<VodHttpAndLocalCacheTask> mHttpAndLocalCacheTask;
};

#endif /* MMPlayerCacheBridge_hpp */
