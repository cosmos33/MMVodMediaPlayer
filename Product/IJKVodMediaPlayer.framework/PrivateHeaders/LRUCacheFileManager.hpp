//
//  LRUCacheFileManager.hpp
//  LeastRecentUseCache
//
//  Created by guowei on 25/04/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#ifndef LRUCacheFileManager_hpp
#define LRUCacheFileManager_hpp

#include <stdio.h>
#include <list>
#include <string>
#include <tuple>
#include <vector>

typedef enum : int {
    LRUCacheFileManagerReadFileError = -7,
    LRUCacheFileManagerFileNotExist = -6,
    LRUCacheFileManagerPathNotConfig = -5,
    LRUCacheFileManagerPathNotRead = -4,
    LRUCacheFileManagerPathNotExist = -3,
    LRUCacheFileManagerCreatePathError = -2,
    LRUCacheFileManagerPathUnknown = -1,
    LRUCacheFileManagerOK = 0,
} LRUCacheFileManagerStatus;

class LRUCacheFileManager
{
public:
    LRUCacheFileManager();
    ~LRUCacheFileManager();
    
    static LRUCacheFileManager *getInstance();
    
    static void destroy();
    
    int createCacheManagerPath();
    
//    int listFoders();
    int listFoders(const char* configPath, std::list<std::string>& configFiles);
    
    int listFoders(const char* configPath, int &fileSize);
    
    void setCfgPath(const char* path);
    
    void setCacheLimit(int64_t cacheLimit);
    
    int reload();
    
    int clearAllCache();
    
    int clearExpireCache();
    
    int clearExpireCacheWithPath(const char *path);
        
    int readToJson(char *fileName, char *key, std::vector<std::tuple<std::string, int, int>>& vistorTuple);
    
    int readToJson(char *fileName);
    
private:
    
    static LRUCacheFileManager *sCacheFileManager;
    
    char *mCfgPath;
    
    char *mFileName;
    
    int64_t mCachedSize;
    
    int64_t mCacheLimitSize;
    
    std::list<std::string> mConfigFiles;
    
    std::vector<std::tuple<std::string, int, int>> mVistorTuple;
    
    int writeBackToFile(char *fileName, char *data, int length);
    
    int removeExpiredFiles(const char* config, char *path);
    
    int64_t calculateCachedSize(const char* configPath, int filesCount, std::list<std::string>& configFiles, std::vector<std::tuple<std::string, int, int>>& vistorTuple);
    
    void checkIfClearCache(const char* configPath, std::vector<std::tuple<std::string, int, int>>& vistorTuple);
    
    void checkIfClearCache(const char* configPath);
};

#endif /* LRUCacheFileManager_hpp */
