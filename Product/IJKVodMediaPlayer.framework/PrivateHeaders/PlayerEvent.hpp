//
//  PlayerEvent.hpp
//  IJKMedia
//
//  Created by guowei on 02/05/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#ifndef PlayerEvent_hpp
#define PlayerEvent_hpp

#include <stdio.h>
#include <list>
#include <tuple>
#include <string>

typedef struct {
    char mEventName[1024];
    int mEventId;
    uint64_t mTimeStamp;
} Event;

class PlayerTracker
{
public:
    
    PlayerTracker(int playerId, char *playURL);
    
    ~PlayerTracker();
    
    int getPlayerId();
    
    void printEvents();
    
    void trackNormalEvent(char *eventName);
    
    void trackErrorEvent(int errorCode);
    
private:
    
    int mPlayerId;
    
    char *mPlayURL;
    
    std::list<Event> mEvents;
    
};

#endif /* PlayerEvent_hpp */
