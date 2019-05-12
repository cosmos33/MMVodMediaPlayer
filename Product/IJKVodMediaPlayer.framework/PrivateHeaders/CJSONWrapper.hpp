//
//  CJSONWapper.hpp
//  LeastRecentUseCache
//
//  Created by guowei on 26/04/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#ifndef CJSONWapper_hpp
#define CJSONWapper_hpp

#include <stdio.h>
extern "C" {
#include "VodCJSON.h"
}

class CJSONWrapper
{
public:
    
    CJSONWrapper(char *jsonString);
    
    ~CJSONWrapper();
    
    cJSON *getRootNode();
    
    char *getStringNodeValue(cJSON *node, char *nodeName);
    
    int getIntNodeValue(cJSON *node, char *nodeName);
    
    double getDoubleNodeValue(cJSON *node, char *nodeName);
    
    int getArraySize(cJSON *array);
    
    cJSON *getArrayNodeValue(cJSON *node, char *nodeName);
    
    void printJSON();
    
    char *getJSONString();
    
    void addIntNodeValue(cJSON *node, char *key, int value);

private:
    
    char *mJSONString;
    
    cJSON *mJSONRoot;
    
};

#endif /* CJSONWapper_hpp */
