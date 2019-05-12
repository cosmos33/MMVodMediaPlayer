//
//  MomoLogger.hpp
//  IJKMedia
//
//  Created by guowei on 26/05/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#ifndef MomoLogger_hpp
#define MomoLogger_hpp

#include <stdio.h>
#include <vector>
#include <string>

class MomoLogger
{
public:
    
    static MomoLogger* getInstance();
    
    ~MomoLogger();
    
    void destoryInstance();
    
    void setLoggerPath(char *path);
    
    void setLoggerTitle(char *title);
    
    void appendLog(char *log);
    
    void appendLog(int what);
    
    void saveLog();
    
private:
    
    MomoLogger();
    
    char *mLoggerPath;
    
    char *mLoggerTitle;
    
    static MomoLogger* sMomoLogger;
    
    std::vector<std::string> mLogArray;
};

#endif /* MomoLogger_hpp */
