//
//  MomoFLVParser.hpp
//  MyFlvParser
//
//  Created by guowei on 10/05/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#ifndef MomoFLVParser_hpp
#define MomoFLVParser_hpp

#include <stdio.h>
#include "ByteBuffer.hpp"
#include <map>

typedef enum : int {
    FLVTagAudio = 8,
    FLVTagVideo = 9,
    FLVTagMeta = 18,
} FLVTagType;

struct FLVHeader {
    char type[3];
    char version;
    char stream_info;
    int header_size;
};

struct FLVTagHeader {
    char type;
    int size; //24bit
    int timestamp; //24bit
    char timestamp_ext;
    int stream_id; //24bit
};

#define FLVTagHeaderSize 11

typedef enum : int {
    FLVParserHeaderFail = -1,
    FLVParserTagFail = -2,
    FLVParserVersionUnknown = -3,
    FLVParserFormatError = -4,
    FLVParserMetaDataFail = -5,
    FLVParserMetaDataMoreData = -6,
    FLVParserOK = 0,
} FLVParseResult;

typedef enum : int {
    FLVMetaTypeNumber = 0,
    FLVMetaTypeBoolean = 1,
    FLVMetaTypeString = 2,
    FLVMetaTypeObject = 3,
    FLVMetaTypeMovieClip = 4,
    FLVMetaTypeNull = 5,
    FLVMetaTypeUndefined = 6,
    FLVMetaTypeReference = 7,
    FLVMetaTypeECMAArray = 8,
    FLVMetaTypeOjbectEndMarker = 9,
    FLVMetaTypeStrictArray = 10,
    FLVMetaTypeDate = 11,
    FLVMetaLongString = 12,
} FLVMetaType;

class MomoFLVParser
{
public:
    MomoFLVParser();
    
    ~MomoFLVParser();
    
    int parse(bb::ByteBuffer &byteBuffer);
    
    int parseHeader(bb::ByteBuffer &byteBuffer);
    
    int parseMetaData(bb::ByteBuffer &byteBuffer, FLVTagHeader *flvTag);
    
    int skipMetaData(bb::ByteBuffer &byteBuffer, FLVTagHeader *flvTag);
    
    int parseAudioData(bb::ByteBuffer &byteBuffer, FLVTagHeader *flvTag);
    
    int parseVideoData(bb::ByteBuffer &byteBuffer, FLVTagHeader *flvTag);
    
    int readTagHeader(bb::ByteBuffer &byteBuffer, FLVTagHeader *flvTag);
    
    void setDuration(int duration);
    
    int isBufferingOK(int cachedSize);
    
    void printInfo();
    
    int getAudioDuration();
    
    int getVideoDuration();
    
    int getDefaultDuration();
    
    int isFLVMetaDataParsed();
    
    int getParsedDataSize();
    
    void setNeededCacheSize(int neededCacheSize);
    
    void reset();
private:
    
    FLVHeader mFLVHeader;
    
    int mFLVHeaderExists;
    
    int mFLVMetaExits;
    
    int mVideoDuration;
    int mAudioDuration;
    int mBufferDuration;
    
    FLVTagHeader mFLVTagHeader;
    int mFLVTagHeaderParsed;
    int mFLVMetaDataParsed;
    std::map<std::string, std::string> mFLVMetaInfo;
    
    int mFLVParsedDataSize;
    int mNeededCacheSize;
};

#endif /* MomoFLVParser_hpp */
