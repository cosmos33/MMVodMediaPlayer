//
//  MDVodPlayerCacheManager.h
//  IJKMedia
//
//  Created by guowei on 19/06/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#ifndef MDVodPlayerCacheManager_h
#define MDVodPlayerCacheManager_h

typedef NSDictionary* (^MDUrlPostParamsBlock)(NSString **urlSting);

extern NSString *kVodPreloadFinishNotification;

extern NSString *kVodPreloadFileKey;

extern NSString *kVodPreloadResultKey;

extern NSString *kVodPlayerNetworkStatusNotification;

extern NSString *kVodPlayerNetworkStatus;

extern NSString * const kVODIJKVideoDownloadingNotification;
extern NSString * const kVODIJKDownloadingSize;
extern NSString * const kVODIJKDownloadingInfo;

@interface MDVodPlayerCacheManager : NSObject

//设置播放器和预加载配置
- (void)setCacheOptions:(NSDictionary*)options;

//根据urlString获取请求参数
- (void)setPostParamsBlock:(MDUrlPostParamsBlock)paramBlock;

//设置预加载最大缓存帧数
//- (void)setCacheMaxFrameCount:(NSInteger)maxFrameCount;

//添加预加载缓存任务
- (void)addPreLoadTaskWithUrl:(NSString*)urlStr;

//添加预加载缓存任务列表
- (void)addPreDownLoadTaskWithUrlStrList:(NSArray*)list;

//删除url对应缓存
- (void)removeCacheWithUrl:(NSString*)urlStr;

//播放下载停止
- (void)stopPlayingDownLoad;

//取消待下载任务, 正在下载任务不能被取消
- (void)cancelAllPreTasks;

//暂停正在下载任务
- (void)suspendAllDownLoadTask;

//恢复正在下载任务
- (void)resumeAllDownLoadTask;

//删除所有缓存
- (void)removeAllCache;

//根据清理规则，清理磁盘缓存
- (void)cleanCache;

@end
#endif /* MDVodPlayerCacheManager_h */
