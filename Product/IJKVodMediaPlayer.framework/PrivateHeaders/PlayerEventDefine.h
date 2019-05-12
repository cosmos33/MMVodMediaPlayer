//
//  PlayerEventDefine.h
//  IJKMedia
//
//  Created by guowei on 03/05/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#ifndef PlayerEventDefine_h
#define PlayerEventDefine_h

#include "ff_ffvodmsg.h"

extern const char* kPlayerEventFirstVideoRendering;
extern const char* kPlayerEventSeek;
extern const char* kPlayerEventBufferingStart;
extern const char* kPlayerEventBufferingEnd;

extern const char* kPlayerEventStart;
extern const char* kPlayerEventPause;
extern const char* kPlayerEventShutdown;

extern const char* kPlayerEventVideoDecorderInit;
extern const char* kPlayerEventVideoDecorderSetupDone;
extern const char* kPlayerEventAudioDecoderInit;
extern const char* kPlayerEventAudioDecoderSetupDone;

extern const char* kPlayerEventHttpError404;


typedef enum {
    PlayerErrorFileNotFound = FFP_MSG_ERROR_404,
    PlayerErrorDNS = FFP_MSG_ERROR_DNS,
    PlayerErrorCDNDown = FFP_MSG_ERROR_CONN_CDN,
    PlayerErrorConnectionRefuse = FFP_MSG_ERROR_CONN_REFUSE,

    PlayerError403  = FFP_MSG_ERROR_403,
    PlayerError5XX  = FFP_MSG_ERROR_5XX,
    PlayerError400  = FFP_MSG_ERROR_400,
    PlayerErrorTimeOut = FFP_MSG_ERROR_TIMEOUT,
    
    PlayerErrorAudioCodec = FFP_MSG_ERROR_AUDIO_CODEC,
    PlayerErrorVideoCodec = FFP_MSG_ERROR_VIDEO_CODEC,
    PlayerErrorProtocol = FFP_MSG_ERROR_PROTCOL,
    PlayerErrorFileFormat = FFP_MSG_ERROR_FORMAT,
} PlayerErrorCode;

static int TranslateErrorCodec(int errorCode)
{
    int result = errorCode;
    
    if (errorCode == -875574520) {
        return PlayerErrorFileNotFound;
    }
    
    if (errorCode == -5) {
        return PlayerErrorDNS;
    }
    
    if (errorCode == -1094995529) {
        return PlayerErrorFileFormat;
    }
    
    if (errorCode == -60) {
        return PlayerErrorCDNDown;
    }
    
    if (errorCode == -65) {
        return PlayerErrorConnectionRefuse;
    }
    
    if (errorCode == -1330794744) {
        return PlayerErrorProtocol;
    }
    
    if (errorCode == -858797304) {
        return PlayerError403;
    }
    
    if (errorCode == -1482175992) {
        return PlayerError5XX;
    }
    
    if (errorCode == -808465656) {
        return PlayerError400;
    }
    if(errorCode == -110) {
        return PlayerErrorTimeOut;
    }

    return result;
}

#endif /* PlayerEventDefine_h */
