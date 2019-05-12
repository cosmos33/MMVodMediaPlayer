/*
 * IJKMediaPlayback.h
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

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <MediaPlayer/MediaPlayer.h>

@protocol IJKVodMediaPlayback;


#pragma mark IJKMediaPlayback

@protocol IJKVodMediaPlayback <NSObject>

- (void)prepareToPlay;
- (void)play;
- (void)pause;
- (void)stop;
- (BOOL)isPlaying;
- (void)shutdown;
- (void)setPauseInBackground:(BOOL)pause;
- (void)setMute:(BOOL)mute;
- (NSString *const)getServerIpAddr;
- (void)setPlayerSpeedRate: (float)rate;
- (float const)playerSpeedRate;
- (uint64_t)currentPlaybackPts;
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
- (uint64_t)getStreamMetaTime;
- (uint64_t)getFirstVideoReceiveTime;
- (uint64_t)getFirstAudioReceiveTime;
- (uint64_t)getFirstVideoDecodeTime;
- (uint64_t)getFirstAudioDecodeTime;
- (uint64_t)getFirstVideoRenderTime;
- (uint64_t)getFirstAudioRenderTime;
- (uint32_t)getStreamCount;


@property(nonatomic, readonly)  UIView *view;
@property(nonatomic)            NSTimeInterval currentPlaybackTime;
@property(nonatomic, readonly)  NSTimeInterval duration;
@property(nonatomic, readonly)  NSTimeInterval playableDuration;
@property(nonatomic, readonly)  NSInteger bufferingProgress;

@property(nonatomic, readonly)  BOOL isPreparedToPlay;
@property(nonatomic, readonly)  MPMoviePlaybackState playbackState;
@property(nonatomic, readonly)  MPMovieLoadState loadState;

@property(nonatomic, readonly) int64_t numberOfBytesTransferred;

@property(nonatomic) MPMovieControlStyle controlStyle;
@property(nonatomic) MPMovieScalingMode scalingMode;
@property(nonatomic) BOOL shouldAutoplay;

@property (nonatomic) BOOL allowsMediaAirPlay;
@property (nonatomic) BOOL isDanmakuMediaAirPlay;
@property (nonatomic, readonly) BOOL airPlayMediaActive;

- (UIImage *)thumbnailImageAtCurrentTime;

#pragma mark Notifications

#ifdef __cplusplus
#define IJK_EXTERN extern "C" __attribute__((visibility ("default")))
#else
#define IJK_EXTERN extern __attribute__((visibility ("default")))
#endif

IJK_EXTERN NSString *const IJKVodMediaPlaybackIsPreparedToPlayDidChangeNotification;

IJK_EXTERN NSString *const IJKVodMoviePlayerLoadStateDidChangeNotification;
IJK_EXTERN NSString *const IJKVodMoviePlayerPlaybackDidFinishNotification;
IJK_EXTERN NSString *const IJKVodMoviePlayerPlaybackStateDidChangeNotification;

IJK_EXTERN NSString *const IJKVodMoviePlayerIsAirPlayVideoActiveDidChangeNotification;
IJK_EXTERN NSString *const IJKVodMoviePlayerVideoDecoderOpenNotification;
IJK_EXTERN NSString *const IJKVodMoviePlayerPlaybackFastSpeedStartedNotification;
IJK_EXTERN NSString *const IJKVodMoviePlayerPlaybackFastSpeedStoppedNotification;
IJK_EXTERN NSString *const IJKVodMoviePlayerPlaybackVideoRenderingNotification;

IJK_EXTERN NSString *const IJKVodMoviePlayerPlaybackUpdateH264SEI DEPRECATED_ATTRIBUTE; //兼容swift应使用Notification后缀
IJK_EXTERN NSString *const IJKVodMoviePlayerPlaybackUpdateH264SEINotification;
IJK_EXTERN NSString *const IJKVodMoviePlayerPlaybackPostAudioPacket DEPRECATED_ATTRIBUTE;
IJK_EXTERN NSString *const IJKVodMoviePlayerPlaybackPostAudioPacketNotification;
IJK_EXTERN NSString *const IJKVodMoviePlayerPlaybackPostAudioCallback DEPRECATED_ATTRIBUTE;
IJK_EXTERN NSString *const IJKVodMoviePlayerPlaybackPostAudioCallbackNotification;
IJK_EXTERN NSString *const IJKVodMoviePlayerPlaybackVideoSizeChanged DEPRECATED_ATTRIBUTE;
IJK_EXTERN NSString *const IJKVodMoviePlayerPlaybackVideoSizeChangedNotification;
IJK_EXTERN NSString *const IJKVodMoviePlayerPlaybackWindowSizeChangedNotification;
IJK_EXTERN NSString *const IJKVodMoviePlayerPlaybackVideoResolutionChangedNotification;
IJK_EXTERN NSString *const IJKVodMoviePlayerPlaybackLoopStart DEPRECATED_ATTRIBUTE;
IJK_EXTERN NSString *const IJKVodMoviePlayerPlaybackLoopStartNotification;

IJK_EXTERN NSString *const IJKVodMoviePlayerRenderFrameInfoNotification;
@end

#pragma mark IJKMediaResource

@protocol IJKVodMediaSegmentResolver <NSObject>

- (NSString *)urlOfSegment:(int)segmentPosition;

@end
