//
//  IJKVodFFMoviePlayerController.h
//  IJKMedia
//
//  Created by guowei on 11/04/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#import "IJKVodMediaPlayback.h"
#import "IJKFFVodOptions.h"

// media meta
#define k_IJKM_KEY_FORMAT         @"format"
#define k_IJKM_KEY_DURATION_US    @"duration_us"
#define k_IJKM_KEY_START_US       @"start_us"
#define k_IJKM_KEY_BITRATE        @"bitrate"

// stream meta
#define k_IJKM_KEY_TYPE           @"type"
#define k_IJKM_VAL_TYPE__VIDEO    @"video"
#define k_IJKM_VAL_TYPE__AUDIO    @"audio"
#define k_IJKM_VAL_TYPE__UNKNOWN  @"unknown"

#define k_IJKM_KEY_CODEC_NAME      @"codec_name"
#define k_IJKM_KEY_CODEC_PROFILE   @"codec_profile"
#define k_IJKM_KEY_CODEC_LONG_NAME @"codec_long_name"

// stream: video
#define k_IJKM_KEY_WIDTH          @"width"
#define k_IJKM_KEY_HEIGHT         @"height"
#define k_IJKM_KEY_FPS_NUM        @"fps_num"
#define k_IJKM_KEY_FPS_DEN        @"fps_den"
#define k_IJKM_KEY_TBR_NUM        @"tbr_num"
#define k_IJKM_KEY_TBR_DEN        @"tbr_den"
#define k_IJKM_KEY_SAR_NUM        @"sar_num"
#define k_IJKM_KEY_SAR_DEN        @"sar_den"
// stream: audio
#define k_IJKM_KEY_SAMPLE_RATE    @"sample_rate"
#define k_IJKM_KEY_CHANNEL_LAYOUT @"channel_layout"
#define k_IJKM_KEY_CHANNELS       @"channels"

#define kk_IJKM_KEY_STREAMS       @"streams"

typedef enum IJKVodLogLevel {
    k_IJKVOD_LOG_UNKNOWN = 0,
    k_IJKVOD_LOG_DEFAULT = 1,
    
    k_IJKVOD_LOG_VERBOSE = 2,
    k_IJKVOD_LOG_DEBUG   = 3,
    k_IJKVOD_LOG_INFO    = 4,
    k_IJKVOD_LOG_WARN    = 5,
    k_IJKVOD_LOG_ERROR   = 6,
    k_IJKVOD_LOG_FATAL   = 7,
    k_IJKVOD_LOG_SILENT  = 8,
} IJKVodLogLevel;

@interface IJKFFVodMoviePlayerController : NSObject <IJKVodMediaPlayback>

- (id)initWithContentURL:(NSURL *)aUrl
             withOptions:(IJKFFVodOptions *)options;

- (id)initWithContentURL:(NSURL *)aUrl
             withOptions:(IJKFFVodOptions *)options
     withSegmentResolver:(id<IJKVodMediaSegmentResolver>)segmentResolver;

- (id)initWithContentURLString:(NSString *)aUrlString
                   withOptions:(IJKFFVodOptions *)options
           withSegmentResolver:(id<IJKVodMediaSegmentResolver>)segmentResolver;

- (id)initWithContentURLString:(NSString *)aUrlString
                   withConfigs:(int)configs
           withSegmentResolver:(id<IJKVodMediaSegmentResolver>)segmentResolver
                      withView:(UIView*)view;

- (id)initWithContentURLString:(NSString *)aUrlString
                   withOptions:(IJKFFVodOptions *)options
                   withConfigs:(int)configs
           withSegmentResolver:(id<IJKVodMediaSegmentResolver>)segmentResolver
                      withView:(UIView*)view DEPRECATED_ATTRIBUTE;

- (id)initWithContentURLString:(NSString *)aUrlString
                   withOptions:(IJKFFVodOptions *)options
           withSegmentResolver:(id<IJKVodMediaSegmentResolver>)segmentResolver
                      withView:(UIView*)view;

- (void)prepareToPlay;
- (void)play;
- (void)pause;
- (void)stop;
- (BOOL)isPlaying;

- (void)unregisterApplicationObserverAll;

- (void)setPauseInBackground:(BOOL)pause;
- (BOOL)isVideoToolboxOpen;

+ (void)setLogReport:(BOOL)preferLogReport;
+ (void)setLogLevel:(IJKVodLogLevel)logLevel;

@property(nonatomic, readonly) CGFloat fpsInMeta;
@property(nonatomic, readonly) CGFloat fpsAtOutput;

@property (nonatomic,readonly) NSDictionary *mediaMetadata;

- (void)setOptionValue:(NSString *)value
                forKey:(NSString *)key
            ofCategory:(IJKFFVodOptionCategory)category;

- (void)setOptionIntValue:(NSInteger)value
                   forKey:(NSString *)key
               ofCategory:(IJKFFVodOptionCategory)category;

- (void)setFormatOptionValue:       (NSString *)value forKey:(NSString *)key;
- (void)setCodecOptionValue:        (NSString *)value forKey:(NSString *)key;
- (void)setSwsOptionValue:          (NSString *)value forKey:(NSString *)key;
- (void)setPlayerOptionValue:       (NSString *)value forKey:(NSString *)key;

- (void)setFormatOptionIntValue:    (int64_t)value forKey:(NSString *)key;
- (void)setCodecOptionIntValue:     (int64_t)value forKey:(NSString *)key;
- (void)setSwsOptionIntValue:       (int64_t)value forKey:(NSString *)key;
- (void)setPlayerOptionIntValue:    (int64_t)value forKey:(NSString *)key;

- (uint64_t)currentPlaybackPts;
- (void)setPlayerSpeedRate: (float)rate;

//设置循环播放次数，默认1，设为0表示无限循环
- (void)setLoop:(int)loop;
//播放配置调试参数,defaultWater 首次buffer时长, nextWather 非首次buffer时长,checkTick 检测间隔
- (void)setIJKPlayOption:(int)defaultWater nextWater:(int)nextWater checkTick:(int)checkTick;

- (void)setIJKPlayOption:(int)startWater nextWater:(int)nextWater maxWater:(int)maxWater checkTick:(int)checkTick;
//关闭播放器，完成时回调completion。正常关闭finished为YES，若调用时已关闭或关闭中finished为NO
- (void)shutdownWithCompletion:(void (^)(BOOL finished))completion;

#ifdef DEBUG_DUMP_FILE
- (void)setDumpFilename: (NSString *)filename;
#endif

- (float const)playerSpeedRate;
- (NSString*const)getServerIpAddr;
- (long)getStreamReceiveSize;

- (long)getAudioReceiveSize;

- (long)getVideoReceiveSize;

- (long)getAudioCacheDuration;

- (long)getVideoCacheDuration;

- (long)getAudioDecodeSize;

- (long)getVideoDecodeCount;

- (long)getAudioRenderSize;

- (long)getVideoRenderCount;
- (uint64_t)getMetaTime;
- (uint64_t)getTcpConnectTime;
typedef struct HttpDetecter HttpDetecter;
- (int)getHttpDetectInfoWithHttpDetecter: (HttpDetecter *)httpDetecter;
- (uint64_t)getStreamMetaTime;
- (uint64_t)getFirstVideoReceiveTime;
- (uint64_t)getFirstAudioReceiveTime;
- (uint64_t)getFirstVideoDecodeTime;
- (uint64_t)getFirstAudioDecodeTime;
- (uint64_t)getFirstVideoRenderTime;
- (uint64_t)getFirstAudioRenderTime;
- (uint32_t)getStreamCount;
- (NSData *)getH264SEI;
- (double)getVideoBufferDurationInSec;

//call getVideoSize after observeMoviePlayerPlaybackVideoRendering
- (CGSize)getVideoSize;

//when screen record start set flag YES, or set flag NO
- (void)recordAudio:(BOOL)enable;
//when live aid push audio, start set flag YES, or set flag NO
- (void)switchAudioCallback:(BOOL)enable;
//set audio playback volume (0 ~ 1.0)
- (void)setAudioPlaybackVolume:(float)volume;
//设置是否关闭音频处理，修改后会从头播放
- (void)setMuteAndReset:(BOOL)mute;
//多音轨切换开关，默认关闭
- (void)openAudioTracks:(BOOL)enable;
//设置播放的音轨，默认是1，只有打开多音轨才支持设置，如双音轨可设置1、2
- (void)setAudioTrackIndex:(int)index;

//获取avformat_find_stream_info 耗时
- (int64_t)getFindStreamInfoCost;

//获取第一个视频packet耗时
- (int64_t)getFirstVideoPacketCost;

//获取解码后第一视频帧耗时
- (int64_t)getFirstVideoFrameCost;

//获取第一帧渲染的耗时
- (int64_t)getFirstVideoRenderCost;

//获取第一个音频packet耗时
- (int64_t)getFirstAudioPacketCost;

//获取解码后第一音频帧的耗时
- (int64_t)getFirstAudioFrameCost;

//获取第一音频帧渲染的耗时
- (int64_t)getFirstAudioRenderCost;

//获取已接收到的字节
- (int64_t)getRecvBytes;

- (int64_t)getVideoCodecID;

- (int64_t)getAudioCodecID;

- (int64_t)getVideoCodecLevel;

- (int64_t)getVideoCodecProfile;

- (int64_t)getVideoRecvBytes;

- (int64_t)getAudioRecvBytes;

- (int64_t)getVideoDuration;

- (int64_t)getAudioDuration;

- (int64_t)getBufferingDuration;

- (int64_t)getVideoHardwareCodecCost;

- (int64_t)getVideoFFPlayCost;

- (int64_t)getTcpConnectCost;

- (int64_t)getVideoCachedBytes;

- (int64_t)getAudioCachedBytes;

@end
