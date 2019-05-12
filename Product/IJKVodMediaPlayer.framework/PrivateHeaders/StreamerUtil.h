//
//  StreamerUtil.hpp
//  IJKMedia
//
//  Created by guowei on 17/04/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#ifndef StreamerUtil_hpp
#define StreamerUtil_hpp

#include <stdio.h>

typedef struct AVDictionary AVDictionary;

struct FLVInfo {
    char type[3];
    char version;
    char stream_info;
    char size[4];
    int header_size;
    int prev_tag_length;
};

typedef enum : int {
    FlvTagAudio = 8,
    FlvTagVideo = 9,
    FlvTagMeta = 18,
} FlvTagType;

struct FLVTagInfo {
    char type;
    char length[3];
    char timestamp[3];
    char timestamp_ext;
    char stream_id[3];
    char *data;
    int data_length;
    int prev_tag_length;
};

void flv_init(FLVInfo *info);

int flv_parse_header(FLVInfo *info, const char* flv_buffer, int flv_buffer_size);

int flv_has_video(FLVInfo *info);

int flv_has_audio(FLVInfo *info);

class StreamerUtil
{
public:
    static int getBestPreloadSizeWithURL(const char* loadURL, AVDictionary **options);
    
    static int getBestPreloadSizeWithFLVBuffer(const char* flvBuffer, int flvBufferSize);
    
    static int getBestPreloadSizeFromBitrate(const char* flvBuffer, int flvBufferSize, int milliSeconds = 2000);
    
};

#endif /* StreamerUtil_hpp */
