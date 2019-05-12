/*
 * ff_ffmsg.h
 *      based on PacketQueue in ffplay.c
 *
 * Copyright (c) 2013 Zhang Rui <bbcallen@gmail.com>
 *
 * This file is part of ijkPlayer.
 *
 * ijkPlayer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * ijkPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with ijkPlayer; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef ff_ffvodmsg_h
#define ff_ffvodmsg_h

#define FFP_MSG_FLUSH                       0
#define FFP_MSG_ERROR                       100     /* arg1 = error */
#define FFP_MSG_PREPARED                    200
#define FFP_MSG_AUDIODATECALLBACK           201
#define FFP_MSG_VIDEODATECALLBACK           202
#define FFP_MSG_JSONPOSDATECALLBACK         203
#define FFP_MSG_COMPLETED                   300
#define FFP_MSG_WINDOW_CHANGED              399     /* arg1 = width, arg2 = height */
#define FFP_MSG_VIDEO_SIZE_CHANGED          400     /* arg1 = width, arg2 = height */
#define FFP_MSG_SAR_CHANGED                 401     /* arg1 = sar.num, arg2 = sar.den */
#define FFP_MSG_VIDEO_RENDERING_START       402
#define FFP_MSG_AUDIO_RENDERING_START       403
#define FFP_MSG_VIDEO_ROTATION_CHANGED      404     /* arg1 = degree */
#define FFP_MSG_VIDEO_NEED_RESEEK			405
#define FFP_MSG_VIDEO_CRECODEC_ERROR        406
#define FFP_MSG_VIDEO_RENDER_INFO           407
#define FFP_MSG_VIDEO_RESOLUTION_CHANGED    408

#define FFP_MSG_BUFFERING_START             500
#define FFP_MSG_BUFFERING_END               501
#define FFP_MSG_BUFFERING_UPDATE            502     /* arg1 = buffering head position in time, arg2 = minimum percent in time or bytes */
#define FFP_MSG_BUFFERING_BYTES_UPDATE      503     /* arg1 = cached data in bytes,            arg2 = high water mark */
#define FFP_MSG_BUFFERING_TIME_UPDATE       504     /* arg1 = cached duration in milliseconds, arg2 = high water mark */
#define FFP_MSG_INITBUFFERING_START         505
#define FFP_MSG_INITBUFFERING_STOP          506
#define FFP_MSG_DURATION_TIME_UPDATE        507
#define FFP_MSG_PLAYING_TIME_UPDATE         508

// 流控相关的通知
#define FFP_MSG_FLOW_CONTROL_START             510
#define FFP_MSG_FLOW_CONTROL_END               511

#define FFP_MSG_SEEK_COMPLETE               600
#define FFP_MSG_PLAYBACK_STATE_CHANGED      700
#define FFP_MSG_FASTPLAY_START              800
#define FFP_MSG_FASTPLAY_END              801

#define FFP_MSG_VIDEO_DECODER_OPEN          10001

#define FFP_REQ_START                       20001
#define FFP_REQ_PAUSE                       20002
#define FFP_REQ_SEEK                        20003

#define FFP_PROP_FLOAT_VIDEO_DECODE_FRAMES_PER_SECOND   10001
#define FFP_PROP_FLOAT_VIDEO_OUTPUT_FRAMES_PER_SECOND   10002
#define FFP_PROP_FLOAT_PLAYBACK_RATE                    10003
#define FFP_PROP_FLOAT_AVDELAY                          10004
#define FFP_PROP_FLOAT_AVDIFF                           10005
#define FFP_PROP_FLOAT_CONNECT_TIME                         10006
#define FFP_PROP_FLOAT_FIRST_IFTAME                           10007
#define FFP_PROP_FLOAT_FIRST_AFTAME                           10008
#define FFP_PROP_FLOAT_FIRST_APACKET                           10009
#define FFP_PROP_FLOAT_FIRST_VPACKET                           10010
#define FFP_PROP_FLOAT_FIRST_VRENDER                           10011
#define FFP_PROP_FLOAT_FIRST_ARENDER                           10012
#define FFP_PROP_FLOAT_DECE_RATE                           10013
#define FFP_PROP_FLOAT_ACCE_RATE                           10014
#define FFP_PROP_FLOAT_SAMPLE_RATE                           10015

#define FFP_PROP_INT64_SELECTED_VIDEO_STREAM            20001
#define FFP_PROP_INT64_SELECTED_AUDIO_STREAM            20002
#define FFP_PROP_INT64_VIDEO_DECODER                    20003
#define FFP_PROP_INT64_STREAM_COUNT                    20004
#define     FFP_PROPV_DECODER_UNKNOWN                   0
#define     FFP_PROPV_DECODER_AVCODEC                   1
#define     FFP_PROPV_DECODER_MEDIACODEC                2
#define     FFP_PROPV_DECODER_VIDEOTOOLBOX              3
#define FFP_PROP_INT64_VIDEO_CACHED_DURATION            20005
#define FFP_PROP_INT64_AUDIO_CACHED_DURATION            20006
#define FFP_PROP_INT64_VIDEO_CACHED_BYTES               20007
#define FFP_PROP_INT64_AUDIO_CACHED_BYTES               20008
#define FFP_PROP_INT64_VIDEO_CACHED_PACKETS             20009
#define FFP_PROP_INT64_AUDIO_CACHED_PACKETS             20010
#define FFP_PROP_INT64_AUDIO_READ_BYTES			20011
#define FFP_PROP_INT64_VIDEO_READ_BYTES			20012
#define FFP_PROP_INT64_AUDIO_RENDER_BYTES			20013
#define FFP_PROP_INT64_AUDIO_DECODER_BYTES			20014
#define FFP_PROP_INT64_VIDEO_DECODE_FRAMES          20015
#define FFP_PROP_INT64_VIDEO_RENDER_FRAMES          20016
#define FFP_PROP_INT64_CREAT_TIME                   20017
#define FFP_PROP_INT64_SERVERSYSTEM_TIME            20018
#define FFP_PROP_INT64_DECE_CACHE_DURATION          20019
#define FFP_PROP_INT64_ACCE_CACHE_DURATION          20020
#define FFP_PROP_INT64_SPLIT_AUDIO_FMT              20021
#define FFP_PROP_INT64_SPLIT_AUDIO_CHANNELS         20022
#define FFP_PROP_INT64_SPLIT_AUDIO_CHANNEL_LAYOUT   20023
#define FFP_PROP_INT64_SPLIT_AUDIO_CHANNEL_FRQ      20024
#define FFP_PROP_INT64_IFRAME_DELAY                 20025

#define FFP_PROP_INT64_CONNECT_TIME                         20026
#define FFP_PROP_INT64_FIRST_VFTAME                           20027
#define FFP_PROP_INT64_FIRST_AFTAME                           20028
#define FFP_PROP_INT64_FIRST_APACKET                           20029
#define FFP_PROP_INT64_FIRST_VPACKET                           20030
#define FFP_PROP_INT64_FIRST_VRENDER                           20031
#define FFP_PROP_INT64_FIRST_ARENDER                           20032
#define FFP_PROP_INT64_BYTES_COUNT                      20033
#define FFP_PROP_INT64_STREAM_COST                      20034

#define FFP_PROP_INT64_VIDEO_CODECID                     20035
#define FFP_PROP_INT64_AUDIO_CODECID                    20036

#define FFP_PROP_INT64_VIDEO_LEVEL                     20037
#define FFP_PROP_INT64_VIDEO_PROFILE                    20038

#define FFP_PROP_INT64_AUDIO_RECV_BYTES                     20039
#define FFP_PROP_INT64_VIDEO_RECV_BYTES                    20040

#define FFP_PROP_INT64_AUDIO_DURATION                     20041
#define FFP_PROP_INT64_VIDEO_DURATION                    20042
#define FFP_PROP_INT64_BUFFER_DURATION                    20043

#define FFP_PROP_INT64_VIDEO_MEDIACODEC_COST                    20044
#define FFP_PROP_INT64_VIDEO_FFPLAY_COST                   20045
#define FFP_PROP_INT64_VIDEO_VIDEOTOOLBOX_COST             20046

#define FFP_PROP_INT64_BIT_RATE                         20100

#define FFP_PROP_INT64_TCP_SPEED                        20200

#define FFP_MSG_FAST_START                  30001
#define FFP_MSG_FAST_STOP                   30002
#define FFP_MSG_VIDEO_RENDER                30003
#define FFP_MSG_VTB_ERROR                   30004

#define FFP_MSG_UPDATE_H264_SEI             40000  //wang.guijie, to indicate we got a new sei,
//we use sei to carry user_define data to notify subview and CDN delay

#define FFP_MSG_POST_AUDIO_PACKET           40001  //wang.qiangqiang, post pcm data to screen record
#define FFP_MSG_POST_AUDIO_CALLBACK         40002  //wang.qiangqiang, post pcm data to remote media

#define FFP_MSG_LOOP_START                  50001  //王旭, 即将开始循环播放

#define FFP_MSG_ERROR_404                  -1001
#define FFP_MSG_ERROR_DNS                  -1002
#define FFP_MSG_ERROR_CONN_CDN             -1003
#define FFP_MSG_ERROR_CONN_REFUSE          -1004
#define FFP_MSG_ERROR_403				   -1005
#define FFP_MSG_ERROR_5XX				   -1006
#define FFP_MSG_ERROR_400				   -1007
#define HttpClientErrorExit                -1008
#define HttpClientErrorEOF                 -1009
#define FFP_MSG_ERROR_TIMEOUT			   -1010

#define  HttpClientErrorSeek                -1011
#define  HttpClientErrorNetUnreach          -1012
#define  HttpClientErrorConnectReset        -1013
#define  HttpClientErrorFormatNotSupport    -1014
#define  HttpClientError401                 -1015


#define FFP_MSG_ERROR_AUDIO_CODEC          -2001
#define FFP_MSG_ERROR_VIDEO_CODEC          -2002
#define FFP_MSG_ERROR_PROTCOL              -2003
#define FFP_MSG_ERROR_FORMAT               -2004
#define FFP_MSG_ERROR_VIDEO_MEDIACODEC     -2005


#define FFP_MSG_ERROR_VIDEO_MEDIACODEC_ENQUEUE -20051  // mediacodec enqueue 出错
#define FFP_MSG_ERROR_VIDEO_MEDIACODEC_ENQUEUE_TIMEOUT -20052 // mediacodec enqueue 超时
#define  FFP_MSG_ERROR_VIDEO_MEDIACODEC_DEQUEUE - 20053 // mediacodec dequeue

#define FFP_MSG_ERROR_LOAD_SO              -4001

#endif
