//
//  PlayerLogger.hpp
//  IJKMediaPlayer
//
//  Created by guowei on 26/05/2017.
//  Copyright © 2017 Momo. All rights reserved.
//

#ifndef PlayerLogger_hpp
#define PlayerLogger_hpp

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
    
void PLSetLoggerPath(char* path);
    
void PLSetLoggerTitle(char* title);
    
void PLAppendLogType(int what);

void PLAppendLogString(char *log);

void PLCloseLogger();
    
#ifdef __cplusplus
}
#endif

#endif /* PlayerLogger_hpp */
