//
//  ff_avio.h
//  Pods
//
//  Created by Evan on 16/5/9.
//
//

#ifndef ff_avio_h
#define ff_avio_h
#include "libavformat/url.h"
#include "libavformat/avformat.h"
int ff_avformat_open_input(AVFormatContext **ic, const char *url, char *key, AVInputFormat *fmt, AVDictionary **options, void *handle);
int ff_avformat_close_input(AVFormatContext **ps,
                            void* handle, int playSuccess);
#endif /* ff_avio_h */
