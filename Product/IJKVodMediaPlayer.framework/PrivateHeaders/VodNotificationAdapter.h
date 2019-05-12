//
//  VodNotificationAdapter.h
//  IJKMedia
//
//  Created by guowei on 14/07/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#ifndef VodNotificationAdapter_h
#define VodNotificationAdapter_h

#ifdef __cplusplus
extern "C" {
#endif

    void VodPostNotificationForDownloadComplete(const char* fileKey, int result);

    void VodPostNotificationForNetworkStatus(int networkStatus);
    
#ifdef __cplusplus
}
#endif

#endif /* VodNotificationAdapter_h */
