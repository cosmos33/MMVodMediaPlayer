/*****************************************************************************
 * IJKVideoToolBox.h
 *****************************************************************************
 *
 * copyright (c) 2014 Zhou Quan <zhouqicy@gmail.com>
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

#ifndef IJKVodMediaPlayer_videotoolbox_core_h
#define IJKVodMediaPlayer_videotoolbox_core_h


#import <VideoToolbox/VideoToolbox.h>
#include "ff_ffvodinc.h"
#include "ff_ffvoderror.h"
#include "ff_ffvodmsg.h"
#include "ff_ffvodplay.h"


#define MAX_PKT_QUEUE_DEEP   350
#define VTB_MAX_DECODING_SAMPLES 16

typedef struct VodVTBPicture {
    double              pts;
    double              dts;
    double              sort;
    CVPixelBufferRef    cvBufferRef;
    uint64_t            width;
    uint64_t            height;
} VodVTBPicture;


typedef struct vod_sample_info {
    int     sample_id;
    
    double  sort;
    double  dts;
    double  pts;
    int     serial;
    
    volatile int is_decoding;
} vod_sample_info;


typedef struct vod_sort_queue {
    double              dts;
    double              pts;
    int                 serial;
    double              sort;
    int64_t             width;
    int64_t             height;
    CVPixelBufferRef    pixel_buffer_ref;
    volatile struct vod_sort_queue  *nextframe;
} vod_sort_queue;


typedef struct VodVideoToolBoxContext {
    FFPlayer                   *ffp;
    int                         width;
    int                         height;
    volatile bool               refresh_request;
    volatile bool               new_seg_flag;
    volatile bool               idr_based_identified;
    int64_t                     last_keyframe_pts;
    volatile bool               refresh_session;
    volatile bool               recovery_drop_packet;
    VTDecompressionSessionRef   m_vt_session;
    CMFormatDescriptionRef      m_fmt_desc;
    const char                 *m_pformat_name;
    VodVTBPicture                  m_videobuffer;
    double                      m_sort_time_offset;
    pthread_mutex_t             m_queue_mutex;
    volatile vod_sort_queue        *m_sort_queue;
    volatile int32_t            m_queue_depth;
    int32_t                     m_max_ref_frames;
    bool                        m_convert_bytestream;
    bool                        m_convert_3byteTo4byteNALSize;
    int                         serial;
    volatile double             last_sort;
    bool                        dealloced;
    int                         m_buffer_deep;
    AVPacket                    m_buffer_packet[MAX_PKT_QUEUE_DEEP];
    
    SDL_mutex                  *sample_info_mutex;
    SDL_cond                   *sample_info_cond;
    vod_sample_info                 sample_info_array[VTB_MAX_DECODING_SAMPLES];
    volatile int                sample_info_index;
    volatile int                sample_info_id_generator;
    volatile int                sample_infos_in_decoding;
    
    int                         enable_vdec_fast_recovery;
    int                         max_fast_decode_frame;
} VodVideoToolBoxContext ;


VodVideoToolBoxContext* vod_init_videotoolbox(FFPlayer* ffp, AVCodecContext* ic);

int vod_videotoolbox_decode_video(VodVideoToolBoxContext* context, AVCodecContext *avctx, const AVPacket *avpkt,int* got_picture_ptr);

void vod_dealloc_videotoolbox(VodVideoToolBoxContext* context);

#endif
