//
//  PlayerEventManager.hpp
//  IJKMedia
//
//  Created by guowei on 02/05/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#ifndef PlayerEventManager_hpp
#define PlayerEventManager_hpp

#include <stdio.h>
#include <list>
#include <pthread.h>

class PlayerTracker;

class PlayerEventManager
{
public:
    static PlayerEventManager *getInstance();
    
    ~PlayerEventManager();
    
    int createPlayerTracker(char *playURL);
    
    void destroyPlayerTracker(int playerId);
    
    void trackEvent(int playerId, char *eventName);
    
    void trackErrorEvent(int playerId, int errorCode);
    
    void printEvent(int playerId);
    
private:
    static PlayerEventManager *sPlayerEventManager;
    
    PlayerEventManager();
    
    PlayerTracker *findPlayerTracker(int playerId);
    
    std::list<PlayerTracker*> mPlayerTrackerLists;
    
    int mPlayerNo;
    
    pthread_mutex_t mMutex;
};

#endif /* PlayerEventManager_hpp */
