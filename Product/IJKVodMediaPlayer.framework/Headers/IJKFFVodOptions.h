//
//  IJKFFVodOptions.h
//  IJKMedia
//
//  Created by guowei on 12/04/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum IJKFFVodOptionCategory {
    kIJKFFVodOptionCategoryFormat = 1,
    kIJKFFVodOptionCategoryCodec  = 2,
    kIJKFFVodOptionCategorySws    = 3,
    kIJKFFVodOptionCategoryPlayer = 4,
} IJKFFVodOptionCategory;

// for codec option 'skip_loop_filter' and 'skip_frame'
typedef enum IJKVodAVDiscard {
    /* We leave some space between them for extensions (drop some
     * keyframes for intra-only or drop just some bidir frames). */
    IJKVod_AVDISCARD_NONE    =-16, ///< discard nothing
    IJKVod_AVDISCARD_DEFAULT =  0, ///< discard useless packets like 0 size packets in avi
    IJKVod_AVDISCARD_NONREF  =  8, ///< discard all non reference
    IJKVod_AVDISCARD_BIDIR   = 16, ///< discard all bidirectional frames
    IJKVod_AVDISCARD_NONKEY  = 32, ///< discard all frames except keyframes
    IJKVod_AVDISCARD_ALL     = 48, ///< discard all
} IJKVodAVDiscard;

typedef struct IjkVodMediaPlayer IjkVodMediaPlayer;

@interface IJKFFVodOptions : NSObject

@property (nonatomic, assign) int configs;//兼容原直播播放器配置方式
@property (nonatomic,assign) BOOL audioQueueRestartEnable;

+(IJKFFVodOptions *)optionsByDefault;
+(IJKFFVodOptions *)optionsByDefaultWithConfigs:(int)configs;
-(void)applyTo:(IjkVodMediaPlayer *)mediaPlayer;

- (void)setOptionValue:(NSString *)value
                forKey:(NSString *)key
            ofCategory:(IJKFFVodOptionCategory)category;

- (void)setOptionIntValue:(int64_t)value
                   forKey:(NSString *)key
               ofCategory:(IJKFFVodOptionCategory)category;


-(void)setFormatOptionValue:       (NSString *)value forKey:(NSString *)key;
-(void)setCodecOptionValue:        (NSString *)value forKey:(NSString *)key;
-(void)setSwsOptionValue:          (NSString *)value forKey:(NSString *)key;
-(void)setPlayerOptionValue:       (NSString *)value forKey:(NSString *)key;

-(void)setFormatOptionIntValue:    (int64_t)value forKey:(NSString *)key;
-(void)setCodecOptionIntValue:     (int64_t)value forKey:(NSString *)key;
-(void)setSwsOptionIntValue:       (int64_t)value forKey:(NSString *)key;
-(void)setPlayerOptionIntValue:    (int64_t)value forKey:(NSString *)key;

-(void)setMaxFps:(int)value;
-(void)setFrameDrop:(int)value;
-(void)setVideoPictureSize:(int)value;
-(void)setVideoToolboxEnabled:(BOOL)value;
-(void)setVideoToolboxMaxFrameWidth:(int)value;
-(void)setIgnoreDurationFirstVideo:(int)value;

-(void)setReconnect:(int)value;
-(void)setTimeout:(int64_t)value;
-(void)setUserAgent:(NSString *)value;

-(void)setSkipLoopFilter:(IJKVodAVDiscard)value;
-(void)setSkipFrame:(IJKVodAVDiscard)value;
#define VTB_CONFIG_OFFSET 1
#define LOOP_FILTER_CONFIG_OFFSET 4
#define SKIP_FRAME_CONFIG_OFFSET 4

#define VTB_ENABLE_MASK (1 << VTB_CONFIG_OFFSET)
#define LOOP_FILTER_MASK (1 << LOOP_FILTER_CONFIG_OFFSET)
#define SKIP_FRAME_MASK (1 << SKIP_FRAME_CONFIG_OFFSET)


@end

typedef NS_OPTIONS(NSUInteger, MDIJKVodPlayerConfigsMask) {
    MDIJKVodPlayerConfigsMaskVTBEnable = VTB_ENABLE_MASK,
    MDIJKVodPlayerConfigsMaskLoopFilter = LOOP_FILTER_MASK,
    MDIJKVodPlayerConfigsMaskSkipFrame = SKIP_FRAME_MASK,
    MDIJKVodPlayerConfigsMaskNoFastFindStream = 1 << 18 //是否不使用fastFindStream
};
