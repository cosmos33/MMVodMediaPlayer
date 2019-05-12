//
//  IJKVodFFPlayerDef.h
//  IJKMedia
//
//  Created by guowei on 11/04/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "ff_ffvodmsg_queue.h"

struct IJKSize {
    NSInteger width;
    NSInteger height;
};
typedef struct IJKSize IJKSize;

CG_INLINE IJKSize
IJKSizeMake(NSInteger width, NSInteger height)
{
    IJKSize size;
    size.width = width;
    size.height = height;
    return size;
}

struct IJKSampleAspectRatio {
    NSInteger numerator;
    NSInteger denominator;
};
typedef struct IJKSampleAspectRatio IJKSampleAspectRatio;

CG_INLINE IJKSampleAspectRatio
IJKSampleAspectRatioMake(NSInteger numerator, NSInteger denominator)
{
    IJKSampleAspectRatio sampleAspectRatio;
    sampleAspectRatio.numerator = numerator;
    sampleAspectRatio.denominator = denominator;
    return sampleAspectRatio;
}

@interface IJKFFVodPlayerMessage : NSObject {
@public
    AVMessage _msg;
}
@end

@interface IJKFFVodMediaPlayerMessagePool : NSObject

- (IJKFFVodMediaPlayerMessagePool *)init;
- (IJKFFVodPlayerMessage *)obtain;
- (void)recycle:(IJKFFVodPlayerMessage *)msg;

@end
