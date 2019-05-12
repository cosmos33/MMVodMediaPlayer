//
//  PlayerEventManagerInterface.hpp
//  IJKMedia
//
//  Created by guowei on 03/05/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#ifndef PlayerEventManagerInterface_hpp
#define PlayerEventManagerInterface_hpp

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
    int PECreatePlayerEvent(char *playURL);
    
    void PEDestroyPlayerEvent(int playerId);
    
    void PETrackNormalEvent(int playerId, char *eventName);
    
    void PETrackErrorEvent(int playerId, int errorCode);
    
    void PEPrintEvent(int playerId);
    
#ifdef __cplusplus
}
#endif

#endif /* PlayerEventManagerInterface_hpp */
