//
//  ff_ffplayer_metric.h
//  IJKMediaPlayer
//
//  Created by guowei on 22/03/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#ifndef ff_ffplayer_metric_h
#define ff_ffplayer_metric_h

#include <stdio.h>
#include "ff_ffvodplay_def.h"

typedef struct FFPlayerMetric {
    char *server_ip;
    double tcp_connect_time;
    double loading_time;
    int buffering_count;
    double buffering_time;
    int bitrate_realtime;
    int bitrate_average;
    char **dns_list_str;
    int dns_list_num;
    double dns_parse_time;
    double playing_time;
    int video_width;
    int video_height;
    double video_buffered_time;
    double video_decoded_fps;
    double video_output_fps;
    
    double start_time;
} FFPlayerMetric;

FFPlayerMetric *ffp_metric_create();

void ffp_metric_reset(FFPlayerMetric *metric);

void ffp_metric_free(FFPlayerMetric **metric);

uint64_t ffp_metric_get_tcp_connect_time(FFPlayerMetric *metric, FFPlayer* ffp);

char *ffp_metric_get_server_ip(FFPlayerMetric *metric, FFPlayer* ffp);

int64_t ffp_metric_get_time();

#endif /* ff_ffplayer_metric_h */
