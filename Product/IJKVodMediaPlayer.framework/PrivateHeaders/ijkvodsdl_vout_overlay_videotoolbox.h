/*****************************************************************************
 * ijksdl_vout_overlay_videotoolbox.h
 *****************************************************************************
 *
 * copyright (c) 2014 ZhouQuan <zhouqicy@gmail.com>
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

#ifndef ijkvodsdl_vout_overlay_videotoolbox_h
#define ijkvodsdl_vout_overlay_videotoolbox_h

#include "ijkvodsdl_stdinc.h"
#include "ijkvodsdl_vout.h"
#include "ijkvodsdl_inc_ffmpeg.h"
#include "IJKVodVideoToolBox.h"

SDL_VoutOverlay *VODSDL_VoutVideoToolBox_CreateOverlay(int width, int height, Uint32 format, SDL_Vout *vout);
int              VODSDL_VoutOverlayVideoToolBox_FillFrame(SDL_VoutOverlay *overlay, VodVTBPicture* picture);
CVPixelBufferRef VODSDL_VoutOverlayVideoToolBox_GetCVPixelBufferRef(SDL_VoutOverlay *overlay);

#endif
