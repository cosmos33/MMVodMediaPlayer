//
//  IJKVODSDLOpenALController.h
//  Pods
//
//  Created by MOMO on 2018/7/30.
//

#ifndef IJKVODSDLOpenALController_h
#define IJKVODSDLOpenALController_h


#import <Foundation/Foundation.h>
#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <OpenAL/oalMacOSX_OALExtensions.h>
#include "ijkvodsdl/ijkvodsdl_aout.h"
@interface LXOpenAlPlay2 :NSObject
{
    ALCcontext *mContext;
    ALCdevice *mDevicde;
    ALuint outSourceId;
    NSMutableDictionary *soundDictionary;
    NSMutableArray *bufferStorageArray;
    ALuint buff;
    NSTimer *updateBufferTimer;
}
@property(nonatomic)ALCcontext *mContext;
@property(nonatomic)ALCdevice *mDevice;
@property(nonatomic,retain)NSMutableDictionary *soundDictionary;
@property(nonatomic,retain)NSMutableArray *bufferStorageArray;
-(void)initOpenAL;
-(void)openAudioFromQueue:(uint8_t *)data dataSize:(UInt32)dataSize;
- (void) openAudioFromQueue:(NSData *)tmpData;
-(BOOL)updataQueueBuffer;
-(void)playSound;
-(void)stopSound;
-(void)cleanUpOpenAL;

@end

@interface IJKVODSDLOpenALController : NSObject
{
    ALCcontext *mContext;
    ALCdevice *mDevicde;
    ALuint outSourceId;
    NSMutableDictionary *soundDictionary;
    NSMutableArray *bufferStorageArray;
    ALuint buff;
    NSTimer *updateBufferTimer;
    
}

@property(nonatomic)ALCcontext *mContext;
@property(nonatomic)ALCdevice *mDevice;
@property(nonatomic,retain)NSMutableDictionary *soundDictionary;
@property(nonatomic,retain)NSMutableArray *bufferStorageArray;

- (id)initWithAudioSpec:(const SDL_AudioSpec *)aSpec;

- (void)play;
- (void)pause;
- (void)flush;
- (void)stop;
- (void)close;
- (void)setPlaybackVolume:(float)playbackVolume;

@property (nonatomic, readonly) SDL_AudioSpec spec;

@end
#endif /* IJKVODSDLOpenALController_h */
