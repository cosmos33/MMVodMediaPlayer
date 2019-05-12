/*
 * Copyright (c) 2003 Fabrice Bellard
 * Copyright (c) 2013-2015 Zhang Rui <bbcallen@gmail.com>
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

#ifndef ff_ffvodplay_def_h
#define ff_ffvodplay_def_h

/**
 * @file
 * simple media player based on the FFmpeg libraries
 */

#include "config.h"
#include <inttypes.h>
#include <math.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>

#include "libavutil/avstring.h"
// FFP_MERGE: #include "libavutil/colorspace.h"
#include "libavutil/eval.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/dict.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/avassert.h"
#include "libavutil/time.h"
#include "libavformat/avformat.h"
// FFP_MERGE: #include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavcodec/avfft.h"
#include "libswresample/swresample.h"

#if CONFIG_AVFILTER
# include "libavfilter/avcodec.h"
# include "libavfilter/avfilter.h"
# include "libavfilter/buffersink.h"
# include "libavfilter/buffersrc.h"
#endif

#include <stdbool.h>
#include "ff_ffvodinc.h"
#include "ff_ffvodplay_config.h"
#include "ff_ffvodmsg_queue.h"
#include "ff_ffvodpipenode.h"
#include "ff_ffplayer_metric.h"

#define DEFAULT_HIGH_WATER_MARK_IN_BYTES        (256 * 1024)

/*
 * START: buffering after prepared/seeked
 * NEXT:  buffering for the second time after START
 * MAX:   ...
 */
#define DEFAULT_START_HIGH_WATER_MARK_IN_MS     (100)
#define DEFAULT_NEXT_HIGH_WATER_MARK_IN_MS      (1 * 1000)
#define DEFAULT_MAX_HIGH_WATER_MARK_IN_MS       (5 * 1000)

#define BUFFERING_CHECK_PER_BYTES               (512)
#define BUFFERING_CHECK_PER_MILLISECONDS        (500)
#define SEEK_CHECK_PER_MILLISECONDS             (100)
#define PROGRESS_NOTIFY_PER_MILLISECONDS             (200)

#define MAX_QUEUE_SIZE (15 * 1024 * 1024)
#define MIN_FRAMES 50000

/* Minimum SDL audio buffer size, in samples. */
#define SDL_AUDIO_MIN_BUFFER_SIZE 512
/* Calculate actual buffer size keeping in mind not cause too frequent audio callbacks */
#define SDL_AUDIO_MAX_CALLBACKS_PER_SEC 30

/* no AV sync correction is done if below the minimum AV sync threshold */
#define AV_SYNC_THRESHOLD_MIN 0.04
/* AV sync correction is done if above the maximum AV sync threshold */
#define AV_SYNC_THRESHOLD_MAX 0.1
/* If a frame duration is longer than this, it will not be duplicated to compensate AV sync */
#define AV_SYNC_FRAMEDUP_THRESHOLD 0.1
/* no AV correction is done if too big error */
#define AV_NOSYNC_THRESHOLD 100.0

/* maximum audio speed change to get correct sync */
#define SAMPLE_CORRECTION_PERCENT_MAX 10

/* external clock speed adjustment constants for realtime sources based on buffer fullness */
#define EXTERNAL_CLOCK_SPEED_MIN  0.900
#define EXTERNAL_CLOCK_SPEED_MAX  1.010
#define EXTERNAL_CLOCK_SPEED_STEP 0.001

/* we use about AUDIO_DIFF_AVG_NB A-V differences to make the average */
#define AUDIO_DIFF_AVG_NB   20

/* polls for possible required screen refresh at least this often, should be less than 1/fps */
#define REFRESH_RATE 0.01

/* NOTE: the size must be big enough to compensate the hardware audio buffersize size */
/* TODO: We assume that a decoded and resampled frame fits into this buffer */
#define SAMPLE_ARRAY_SIZE (8 * 65536)

#ifdef FFP_MERGE
#define CURSOR_HIDE_DELAY 1000000

static int64_t sws_flags = SWS_BICUBIC;
#endif

#define MAX_SEI_DATA_SIZE (1280)
#define MAX_SEI_NUM (30)

typedef struct{
    int width;
    int height;
} VideoSize;

typedef struct MyAVPacketList {
    AVPacket pkt;
    struct MyAVPacketList *next;
    int serial;
} MyAVPacketList;

typedef struct PacketQueue {
    MyAVPacketList *first_pkt, *last_pkt;
    int nb_packets;
    int size;
    long total_size;
    int64_t duration;
    int abort_request;
    int serial;
    SDL_mutex *mutex;
    SDL_cond *cond;
    MyAVPacketList *recycle_pkt;
    int recycle_count;
    int alloc_count;

    int is_buffer_indicator;
} PacketQueue;

// #define VIDEO_PICTURE_QUEUE_SIZE 3
#define VIDEO_PICTURE_QUEUE_SIZE_MIN        (3)
#define VIDEO_PICTURE_QUEUE_SIZE_MAX        (16)
#define VIDEO_PICTURE_QUEUE_SIZE_DEFAULT    (VIDEO_PICTURE_QUEUE_SIZE_MIN)
#define SUBPICTURE_QUEUE_SIZE 16
#define SAMPLE_QUEUE_SIZE 9
#define FRAME_QUEUE_SIZE FFMAX(SAMPLE_QUEUE_SIZE, FFMAX(VIDEO_PICTURE_QUEUE_SIZE_MAX, SUBPICTURE_QUEUE_SIZE))

#define VIDEO_MAX_FPS_DEFAULT 30

typedef struct AudioParams {
    int freq;
    int channels;
    int64_t channel_layout;
    enum AVSampleFormat fmt;
    int frame_size;
    int bytes_per_sec;
} AudioParams;

typedef struct Clock {
    double pts;           /* clock base */
    double pts_drift;     /* clock base minus time at which we updated the clock */
    double last_updated;
    double speed;
    int serial;           /* clock is based on a packet with this serial */
    int paused;
    int *queue_serial;    /* pointer to the current packet queue serial, used for obsolete clock detection */
} Clock;

/* Common struct for handling all types of decoded data and allocated render buffers. */
typedef struct Frame {
    AVFrame *frame;
    AVSubtitle sub;
    int serial;
    double pts;           /* presentation timestamp for the frame */
    double duration;      /* estimated duration of the frame */
    int64_t pos;          /* byte position of the frame in the input file */
    SDL_VoutOverlay *bmp;
    int allocated;
    int reallocate;
    int width;
    int height;
    int format;
    AVRational sar;
} Frame;

typedef struct FrameQueue {
    Frame queue[FRAME_QUEUE_SIZE];
    int rindex;
    int windex;
    int size;
    int max_size;
    int keep_last;
    int rindex_shown;
    SDL_mutex *mutex;
    SDL_cond *cond;
    PacketQueue *pktq;
    int total_count;
    int total_size;
    uint64_t first_time;
} FrameQueue;

enum {
    AV_SYNC_AUDIO_MASTER, /* default choice */
    AV_SYNC_VIDEO_MASTER,
    AV_SYNC_EXTERNAL_CLOCK, /* synchronize to an external clock */
};

typedef struct Decoder {
    AVPacket pkt;
    AVPacket pkt_temp;
    PacketQueue *queue;
    //added by tricker on 18/04/10 for ktv stream switch
    PacketQueue *ext_queue;
    AVCodecContext *avctx;
    int pkt_serial;
    int finished;
    int packet_pending;
    int bfsc_ret;
    uint8_t *bfsc_data;

    SDL_cond *empty_queue_cond;
    int64_t start_pts;
    AVRational start_pts_tb;
    int64_t next_pts;
    AVRational next_pts_tb;
    SDL_Thread *decoder_tid;
    SDL_Thread _decoder_tid;

    SDL_Profiler decode_profiler;
} Decoder;

typedef struct VideoState {
    SDL_Thread *read_tid;
    SDL_Thread _read_tid;
    AVInputFormat *iformat;
    int abort_request;
    int force_refresh;
    int paused;
    int last_paused;
    int queue_attachments_req;
    int seek_req;
    int seek_flags;
    int64_t seek_pos;
    int64_t seek_rel;
    int seek_req_buffer_check;
#ifdef FFP_MERGE
    int read_pause_return;
#endif
    AVFormatContext *ic;
    int realtime;

    Clock audclk;
    Clock vidclk;
    Clock extclk;

    FrameQueue pictq;
#ifdef FFP_MERGE
    FrameQueue subpq;
#endif
    FrameQueue sampq;

    Decoder auddec;
    Decoder viddec;
#ifdef FFP_MERGE
    Decoder subdec;
#endif

    int audio_stream;
    //added by tricker on 18/04/10 for ktv stream switch
    int audio_stream_count;
    int ext_audio_stream;
    int cur_audio_stream;

    int av_sync_type;

    double audio_clock;
    int audio_clock_serial;
    double audio_diff_cum; /* used for AV difference average computation */
    double audio_diff_avg_coef;
    double audio_diff_threshold;
    int audio_diff_avg_count;
    AVStream *audio_st, *ext_audio_st;
    PacketQueue audioq;
    //added by tricker on 18/04/10 for ktv stream switch
    PacketQueue ext_audioq;
    int64_t audioq_duration;
    int audio_hw_buf_size;
    uint8_t silence_buf[SDL_AUDIO_MIN_BUFFER_SIZE];
    uint8_t *audio_buf;
    uint8_t *audio_buf1;
    unsigned int audio_buf_size; /* in bytes */
    unsigned int audio_buf1_size;
    int audio_buf_index; /* in bytes */
    int audio_write_buf_size;
    struct AudioParams audio_src;
#if CONFIG_AVFILTER
    struct AudioParams audio_filter_src;
#endif
    struct AudioParams audio_tgt;
    struct SwrContext *swr_ctx;
    int frame_drops_early;
    int frame_drops_late;
    int continuous_frame_drops_early;

    enum ShowMode {
        SHOW_MODE_NONE = -1, SHOW_MODE_VIDEO = 0, SHOW_MODE_WAVES, SHOW_MODE_RDFT, SHOW_MODE_NB
    } show_mode;
    int16_t sample_array[SAMPLE_ARRAY_SIZE];
    int sample_array_index;
    int last_i_start;
#ifdef FFP_MERGE
    RDFTContext *rdft;
    int rdft_bits;
    FFTSample *rdft_data;
    int xpos;
#endif
    double last_vis_time;

#ifdef FFP_MERGE
    int subtitle_stream;
    AVStream *subtitle_st;
    PacketQueue subtitleq;
#endif
    
    int subtitle_stream;
    double frame_timer;
    double frame_last_returned_time;
    double frame_last_filter_delay;
    int video_stream;
    AVStream *video_st;
    PacketQueue videoq;
    int64_t videoq_duration;
    double max_frame_duration;      // maximum duration of a frame - above this, we consider the jump a timestamp discontinuity
#if !CONFIG_AVFILTER
    struct SwsContext *img_convert_ctx;
#endif
#ifdef FFP_MERGE
    SDL_Rect last_display_rect;
#endif
    int eof;

    char filename[4096];
    int width, height, xleft, ytop;
    int step;
    int64_t display_frame_counter;

#if CONFIG_AVFILTER
    int vfilter_idx;
    AVFilterContext *in_video_filter;   // the first filter in the video chain
    AVFilterContext *out_video_filter;  // the last filter in the video chain
    AVFilterContext *in_audio_filter;   // the first filter in the audio chain
    AVFilterContext *out_audio_filter;  // the last filter in the audio chain
    AVFilterGraph *agraph;              // audio filter graph
#endif

    int last_video_stream, last_audio_stream, last_subtitle_stream;

    //added by tricker on 18/04/10 for ktv stream switch
    int ext_last_audio_stream;

    SDL_cond *continue_read_thread;

    /* extra fields */
    SDL_mutex  *play_mutex; // only guard state, do not block any long operation
    SDL_Thread *video_refresh_tid;
    SDL_Thread _video_refresh_tid;

    int buffering_on;
    int pause_req;

    int dropping_frame;
    int is_video_high_fps; // above 30fps
    int is_video_high_res; // above 1080p

    PacketQueue *buffer_indicator_queue;
    
    bool play_from_audio_interrupt;
    bool is_post_audio_pcm;
    bool is_post_audio_callback;
    float audio_volume;  //播放音量（0~1）默认为1
    
//    SDL_Thread *progress_refresh_tid;
//    SDL_Thread _progress_refresh_tid;

} VideoState;


typedef struct FFTrackCacheStatistic
{
    int64_t duration;
    int64_t bytes;
    int64_t packets;
} FFTrackCacheStatistic;

typedef struct FFStatistic
{
    int64_t vdec_type;

    float vfps;
    float vdps;
    float avdelay;
    float avdiff;
    int64_t bit_rate;

    FFTrackCacheStatistic video_cache;
    FFTrackCacheStatistic audio_cache;

    int64_t buf_backwards;
    int64_t buf_forwards;
    int64_t buf_capacity;
    int64_t latest_seek_load_duration;
    int64_t cache_physical_pos;
    int64_t cache_file_forwards;
    int64_t cache_file_pos;
    int64_t cache_count_bytes;
    int64_t logical_file_size;
    int drop_frame_count;
    int decode_frame_count;
    float drop_frame_rate;
} FFStatistic;

inline static void ffvodp_reset_statistic(FFStatistic *dcc)
{
    memset(dcc, 0, sizeof(FFStatistic));
}

/* options specified by the user */
#ifdef FFP_MERGE
static AVInputFormat *file_iformat;
static const char *input_filename;
static const char *window_title;
static int fs_screen_width;
static int fs_screen_height;
static int default_width  = 640;
static int default_height = 480;
static int screen_width  = 0;
static int screen_height = 0;
static int audio_disable;
static int video_disable;
static int subtitle_disable;
static const char* wanted_stream_spec[AVMEDIA_TYPE_NB] = {0};
static int seek_by_bytes = -1;
static int display_disable;
static int show_status = 1;
static int av_sync_type = AV_SYNC_AUDIO_MASTER;
static int64_t start_time = AV_NOPTS_VALUE;
static int64_t duration = AV_NOPTS_VALUE;
static int fast = 0;
static int genpts = 0;
static int lowres = 0;
static int decoder_reorder_pts = -1;
static int autoexit;
static int exit_on_keydown;
static int exit_on_mousedown;
static int loop = 1;
static int framedrop = -1;
static int infinite_buffer = -1;
static enum ShowMode show_mode = SHOW_MODE_NONE;
static const char *audio_codec_name;
static const char *audio_codec_name_ext;
static const char *subtitle_codec_name;
static const char *video_codec_name;
double rdftspeed = 0.02;
static int64_t cursor_last_shown;
static int cursor_hidden = 0;
#if CONFIG_AVFILTER
static const char **vfilters_list = NULL;
static int nb_vfilters = 0;
static char *afilters = NULL;
#endif
static int autorotate = 1;

/* current context */
static int is_full_screen;
static int64_t audio_callback_time;

static AVPacket flush_pkt;
static AVPacket eof_pkt;

#define FF_ALLOC_EVENT   (SDL_USEREVENT)
#define FF_QUIT_EVENT    (SDL_USEREVENT + 2)

static SDL_Surface *screen;
#endif

/*****************************************************************************
 * end at line 330 in ffplay.c
 * near packet_queue_put
 ****************************************************************************/

/* ffplayer */
typedef struct IjkVodMediaMeta IjkVodMediaMeta;
typedef struct IJKFFVod_Pipeline IJKFFVod_Pipeline;
typedef struct FFPlayerMetric FFPlayerMetric;
typedef struct FFPlayer {
    const AVClass *av_class;

    /* ffplay context */
    VideoState *is;

    /* format/codec options */
    AVDictionary *format_opts;
    AVDictionary *codec_opts;
    AVDictionary *sws_opts;
    AVDictionary *player_opts;

    /* ffplay options specified by the user */
#ifdef FFP_MERGE
    AVInputFormat *file_iformat;
#endif
    char *input_filename;
#ifdef FFP_MERGE
    const char *window_title;
    int fs_screen_width;
    int fs_screen_height;
    int default_width;
    int default_height;
    int screen_width;
    int screen_height;
#endif
    int audio_disable;
    int video_disable;
#ifdef FFP_MERGE
    int subtitle_disable;
#endif
    //for ktv to select audio stream on init
    int is_audio_selected_by_user;
    //期望的音频流 1 默认的音频流 2 非默认音频流
    int pending_audio;
    const char* wanted_stream_spec[AVMEDIA_TYPE_NB];
    int seek_by_bytes;
    int display_disable;
    int show_status;
    int av_sync_type;
    int64_t start_time;
    int64_t duration;
    int fast;
    int genpts;
    int lowres;
    int decoder_reorder_pts;
    int autoexit;
#ifdef FFP_MERGE
    int exit_on_keydown;
    int exit_on_mousedown;
#endif
    int loop;
    int framedrop;
    int infinite_buffer;
    enum ShowMode show_mode;
    char *audio_codec_name;
    char *audio_codec_name_ext;
    int enable_render_info_callback;
#ifdef FFP_MERGE
    char *subtitle_codec_name;
#endif
    char *video_codec_name;
    double rdftspeed;
#ifdef FFP_MERGE
    int64_t cursor_last_shown;
    int cursor_hidden;
#endif
#if CONFIG_AVFILTER
    const char **vfilters_list;
    int nb_vfilters;
    char *afilters;
#endif
    int autorotate;

    int sws_flags;

    /* current context */
#ifdef FFP_MERGE
    int is_full_screen;
#endif
    int64_t audio_callback_time;
#ifdef FFP_MERGE
    SDL_Surface *screen;
#endif

    /* extra fields */
    SDL_Aout *aout;
    SDL_Vout *vout;
    IJKFFVod_Pipeline *pipeline;
    IJKFF_Pipenode *node_vdec;
    int sar_num;
    int sar_den;

    char *video_codec_info;
    char *audio_codec_info;
    Uint32 overlay_format;

    int last_error;
    int prepared;
    int auto_resume;
    int error;
    int error_count;
    int start_on_prepared;
    int first_video_frame_pushed;
    int ignore_duration;
    int enable_ijk_cache;

    MessageQueue msg_queue;

    int max_buffer_size;
    int high_water_mark_in_bytes;

    int start_high_water_mark_in_ms;
    int next_high_water_mark_in_ms;
    int max_high_water_mark_in_ms;
    int current_high_water_mark_in_ms;
    int check_buffer_mark_in_ms;

    int64_t playable_duration_ms;

    int pictq_size;
    int max_fps;

    int videotoolbox;
    int vtb_max_frame_width;
    int vtb_async;
    int vtb_wait_async;

    int mediacodec;
    int opensles;

    IjkVodMediaMeta *meta;

    ijk_format_control_message format_control_message;
    void *format_control_opaque;

    SDL_mutex   *cache_mutex;
    void        *cache_task;
    char        *file_key;
    char        *user_info;
    
    FFPlayerMetric *player_metric;
    int64_t ffp_start_time_ms;
    int64_t first_video_cost_ms;
    int64_t ffp_find_stream_info_ms;
    int ffp_buffering_count;
    int is_first_buffering;
    
    int ffp_buffering_start_time;
    int ffp_buffering_end_time;
    int ffp_buffering_time;
    
    char *ffp_player_key;
    char *ffp_cache_root;
    
    bool mute;
    int is_render_audio;
    
    bool is_fast_find_stream; //设置是否使用fastFindStream, 默认true
    
    int width;
    int height;
    
    uint8_t h264_sei_data[MAX_SEI_DATA_SIZE];
    size_t h264_data_size;
    float rate;
    
    int64_t audio_render_size;
    int64_t video_render_count;
    
    uint64_t audio_receive_ts;
    uint64_t video_receive_ts;
    uint64_t video_render_ts;
    uint64_t video_last_render_ts;
    uint64_t audio_render_ts;
    
    double lastestRecvPtsInSec;
    double lastestDisplayPtsSec; //buffer durationInSecs=lastestRecvPtsInSec - lastestDisplayPtsSec
    
    int ffp_player_id;

    int is_ijk_cache;
    bool ignore_vtd_err; //是否忽略硬解码错误，默认true
    
    int  buffering_enter_mode;  // 设置buffering状态进入 方式 0 auto 1 音视频流都检查
    int  enable_vdec_fast_recovery;     // 视频解码快速恢复，用于在后台暂停后，返回前台恢复播放时画面流畅 for ios

    int enable_cache_duration;	//是否开始缓存控制策略
    int max_cache_duration_inms; //设置最大缓存体积
    int min_watch_time_in_ms;	//设置改变缓存体积时间点
    
    float audio_volume; //播放音量（0~1）默认为1
    
    bool audioQueueRestartEnable; //是否重启audioQueue,在暂停播放以后

    AVRational sar;

    int needAudioDevices;

    FFStatistic stat;

    int64_t total_recv_bytes;

    int64_t video_recv_bytes;

    int64_t audio_recv_bytes;

    int64_t video_duration;

    int64_t audio_duration;

    // tcp连接时长
    int64_t  tcp_connect_time;

    // 获取流信息耗时
    int64_t find_stream_info_cost;

    // ffp 创建时间戳
    int64_t ffp_create_time;

    // 第一个video 时间戳
    int64_t first_video_packet_time;

    // 第一个视频帧时间戳
    int64_t first_video_frame_time;

    // 第一个视频帧渲染
    int64_t first_video_rendered_time;

    // 第一个音频包
    int64_t first_audio_packet_time;

    // 第一个音频帧
    int64_t first_audio_frame_time;

    // 第一个音频帧渲染
    int64_t first_audio_rendered_time;

    // 视频解码器
    int64_t video_codec_id;

    // 音频解码器
    int64_t audio_codec_id;

    int64_t video_codec_level;
    int64_t video_codec_profile;

    int64_t start_buffering_time;
    int64_t stop_buffering_time;
    int64_t buffering_duration;

    int64_t video_media_codec_cost;
    int64_t video_ffplay_codec_cost;
    int64_t video_videotoolbox_cost;

    int is_flow_control_started;
    int is_play_completed;

} FFPlayer;

#define fftime_to_milliseconds(ts) (av_rescale(ts, 1000, AV_TIME_BASE));
#define milliseconds_to_fftime(ms) (av_rescale(ms, AV_TIME_BASE, 1000));

inline static void ffvodp_reset_internal(FFPlayer *ffp)
{
    /* ffp->is closed in stream_close() */

    /* format/codec options */
    av_dict_free(&ffp->format_opts);
    av_dict_free(&ffp->codec_opts);
    av_dict_free(&ffp->sws_opts);
    av_dict_free(&ffp->player_opts);

    /* ffplay options specified by the user */
    av_freep(&ffp->input_filename);
    ffp->audio_disable          = 0;
    ffp->video_disable          = 0;
    ffp->is_audio_selected_by_user   = 0;
    ffp->pending_audio          = 1;
    memset(ffp->wanted_stream_spec, 0, sizeof(ffp->wanted_stream_spec));
    ffp->seek_by_bytes          = -1;
    ffp->display_disable        = 0;
    ffp->show_status            = 0;
    ffp->av_sync_type           = AV_SYNC_AUDIO_MASTER;
    ffp->start_time             = AV_NOPTS_VALUE;
    ffp->duration               = AV_NOPTS_VALUE;
    ffp->fast                   = 1;
    ffp->genpts                 = 0;
    ffp->lowres                 = 0;
    ffp->decoder_reorder_pts    = -1;
    ffp->autoexit               = 0;
    ffp->loop                   = 1;
    ffp->is_render_audio        = 1;
    ffp->framedrop              = 0; // option
    ffp->infinite_buffer        = -1;
    ffp->show_mode              = SHOW_MODE_NONE;
    av_freep(&ffp->audio_codec_name);
    av_freep(&ffp->audio_codec_name_ext);
    av_freep(&ffp->video_codec_name);
    ffp->rdftspeed              = 0.02;
#if CONFIG_AVFILTER
    ffp->vfilters_list          = NULL;
    ffp->nb_vfilters            = 0;
    ffp->afilters               = NULL;
#endif
    ffp->autorotate             = 1;

    ffp->sws_flags              = SWS_FAST_BILINEAR;

    /* current context */
    ffp->audio_callback_time    = 0;

    /* extra fields */
    ffp->aout                   = NULL; /* reset outside */
    ffp->vout                   = NULL; /* reset outside */
    ffp->pipeline               = NULL;
    ffp->node_vdec              = NULL;
    ffp->sar_num                = 0;
    ffp->sar_den                = 0;

    av_freep(&ffp->video_codec_info);
    av_freep(&ffp->audio_codec_info);
    ffp->overlay_format         = SDL_FCC__GLES2;

    ffp->last_error             = 0;
    ffp->prepared               = 0;
    ffp->auto_resume            = 0;
    ffp->error                  = 0;
    ffp->error_count            = 0;
    ffp->start_on_prepared      = 1;
    ffp->first_video_frame_pushed = 0;
    ffp->enable_render_info_callback = 0;

    ffp->max_buffer_size                = MAX_QUEUE_SIZE;
    ffp->high_water_mark_in_bytes       = DEFAULT_HIGH_WATER_MARK_IN_BYTES;

    ffp->start_high_water_mark_in_ms    = DEFAULT_START_HIGH_WATER_MARK_IN_MS;
    ffp->next_high_water_mark_in_ms     = DEFAULT_NEXT_HIGH_WATER_MARK_IN_MS;
    ffp->max_high_water_mark_in_ms      = DEFAULT_MAX_HIGH_WATER_MARK_IN_MS;
    ffp->current_high_water_mark_in_ms  = DEFAULT_START_HIGH_WATER_MARK_IN_MS;
    ffp->check_buffer_mark_in_ms        = BUFFERING_CHECK_PER_MILLISECONDS;

    ffp->playable_duration_ms           = 0;

    ffp->pictq_size                     = VIDEO_PICTURE_QUEUE_SIZE_DEFAULT; // option
    ffp->max_fps                        = 31; // option

    ffp->videotoolbox                   = 0; // option
    ffp->vtb_max_frame_width            = 0; // option
    ffp->vtb_async                      = 0; // option
    ffp->vtb_wait_async                 = 0; // option

    ffp->mediacodec                     = 0; // option
    ffp->opensles                       = 0; // option

    ffp->format_control_message = NULL;
    ffp->format_control_opaque  = NULL;

    ffp->meta = NULL;
    msg_queue_flush(&ffp->msg_queue);
    
    ffp->cache_task = NULL;
    ffp->file_key = NULL;
    ffp->user_info = NULL;
    
    ffp->ffp_start_time_ms = AV_NOPTS_VALUE;
    ffp->first_video_cost_ms = AV_NOPTS_VALUE;
    ffp->ffp_buffering_count = 0;
    ffp->is_first_buffering = 1;
    ffp->ffp_buffering_start_time = 0;
    ffp->ffp_buffering_end_time = 0;
    
    ffp->ffp_player_key = NULL;
    ffp->ffp_player_id = -1;
    ffp->is_ijk_cache = 1;
    ffp->ignore_vtd_err = true;
    ffp->buffering_enter_mode = 0;
    ffp->enable_vdec_fast_recovery = 0;
    ffp->enable_cache_duration = 0;
    ffp->max_cache_duration_inms = 5000;
    ffp->min_watch_time_in_ms = 3000;
    ffp->audioQueueRestartEnable = false;
    ffp->needAudioDevices = 1;
    
    memset(&ffp->sar, 0 , sizeof(ffp->sar));
    ffvodp_reset_statistic(&ffp->stat);

    ffp->total_recv_bytes = 0;
    ffp->video_recv_bytes = 0;
    ffp->audio_recv_bytes = 0;
    ffp->video_duration = 0;
    ffp->audio_duration = 0;
    ffp->tcp_connect_time = -1;
    ffp->find_stream_info_cost = -1;
    ffp->ffp_create_time = -1;
    ffp->first_video_packet_time = -1;
    ffp->first_video_frame_time = -1;
    ffp->first_video_rendered_time = -1;
    ffp->video_codec_id = -1;

    ffp->first_audio_packet_time = -1;
    ffp->first_audio_frame_time = -1;
    ffp->first_audio_rendered_time = -1;
    ffp->audio_codec_id = -1;

    ffp->video_codec_level = -1;
    ffp->video_codec_profile = -1;

    ffp->start_buffering_time = - 1;
    ffp->stop_buffering_time = -1;
    ffp->buffering_duration = 0;
    ffp->video_media_codec_cost = -1;
    ffp->video_ffplay_codec_cost = -1;
    ffp->video_videotoolbox_cost = -1;

    ffp->is_flow_control_started = 0;
    ffp->is_play_completed = 0;
}

inline static void ffvodp_notify_msg1(FFPlayer *ffp, int what) {
    msg_queue_put_simple3(&ffp->msg_queue, what, 0, 0);
}

inline static void ffvodp_notify_msg2(FFPlayer *ffp, int what, int arg1) {
    msg_queue_put_simple3(&ffp->msg_queue, what, arg1, 0);
}

inline static void ffvodp_notify_msg3(FFPlayer *ffp, int what, int arg1, int arg2) {
    msg_queue_put_simple3(&ffp->msg_queue, what, arg1, arg2);
}

inline static void ffvodp_notify_msg4(FFPlayer *ffp, int what, int arg1, int arg2, void * arg3) {
    msg_queue_put_simple4(&ffp->msg_queue, what, arg1, arg2, arg3);
}

inline static void ffvodp_notify_msg5(FFPlayer *ffp, int what, double arg4, int64_t arg5) {
    msg_queue_put_simple5(&ffp->msg_queue, what, arg4, arg5);
}

inline static int ffvodp_notify_msg_frame(FFPlayer *ffp, int what, int arg1, int arg2, void * frame) {
    return msg_queue_put_frame(&ffp->msg_queue, what, arg1, arg2, frame);
}

inline static void ffvodp_remove_msg(FFPlayer *ffp, int what) {
    msg_queue_remove(&ffp->msg_queue, what);
}

inline static const char *ffvodp_get_error_string(int error) {
    switch (error) {
        case AVERROR(ENOMEM):       return "AVERROR(ENOMEM)";       // 12
        case AVERROR(EINVAL):       return "AVERROR(EINVAL)";       // 22
        case AVERROR(EAGAIN):       return "AVERROR(EAGAIN)";       // 35
        case AVERROR(ETIMEDOUT):    return "AVERROR(ETIMEDOUT)";    // 60
        case AVERROR_EOF:           return "AVERROR_EOF";
        case AVERROR_EXIT:          return "AVERROR_EXIT";
    }
    return "unknown";
}

#define FFTRACE ALOGW

#define AVCODEC_MODULE_NAME    "avcodec"
#define MEDIACODEC_MODULE_NAME "MediaCodec"

#endif
