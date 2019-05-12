//
//  MDCacheCleanHandler.h
//  IJKMedia
//
//  Created by lm on 17/3/1.
//  Copyright © 2017年 MOMO. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void (^MDBeforeCleanTask)();

@interface MDVodCacheCleanHandler : NSObject

@property (nonatomic, assign) NSInteger minDiskMSize;
@property (nonatomic, assign) NSInteger maxCacheMSize;

- (void)cancel;

- (void)doCacheCleanWithPath:(NSString*)cachePath inBackgroundState:(BOOL)isBackground beforeBlock:(MDBeforeCleanTask)block;

- (void)removeAllCacheInPath:(NSString*)cachePath;

@end
