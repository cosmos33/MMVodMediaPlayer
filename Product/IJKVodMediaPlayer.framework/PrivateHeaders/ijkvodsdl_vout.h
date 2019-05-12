/*****************************************************************************
 * ijksdl_vout.h
 *****************************************************************************
 *
 * copyright (c) 2013 Zhang Rui <bbcallen@gmail.com>
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

#ifndef ijkvodsdl_vout_h
#define ijkvodsdl_vout_h

#include "ijkvodsdl_stdinc.h"
#include "ijkvodsdl_class.h"
#include "ijkvodsdl_mutex.h"
#include "ijkvodsdl_video.h"

typedef struct SDL_VoutOverlay_Opaque SDL_VoutOverlay_Opaque;
typedef struct SDL_VoutOverlay SDL_VoutOverlay;
typedef struct SDL_VoutOverlay {
    int w; /**< Read-only */
    int h; /**< Read-only */
    Uint32 format; /**< Read-only */
    int planes; /**< Read-only */
    Uint16 *pitches; /**< in bytes, Read-only */
    Uint8 **pixels; /**< Read-write */

    int is_private;

    int sar_num;
    int sar_den;
    
    SDL_Class               *opaque_class;
    SDL_VoutOverlay_Opaque  *opaque;
    void                    (*free_l)(SDL_VoutOverlay *overlay);
    int                     (*lock)(SDL_VoutOverlay *overlay);
    int                     (*unlock)(SDL_VoutOverlay *overlay);
    void                    (*unref)(SDL_VoutOverlay *overlay);
    int rotation;
} SDL_VoutOverlay;

typedef struct SDL_Vout_Opaque SDL_Vout_Opaque;
typedef struct SDL_Vout SDL_Vout;
typedef struct SDL_Vout {
    SDL_mutex *mutex;

    SDL_Class       *opaque_class;
    SDL_Vout_Opaque *opaque;
    SDL_VoutOverlay *(*create_overlay)(int width, int height, Uint32 format, SDL_Vout *vout);
    void (*free_l)(SDL_Vout *vout);
    int (*display_overlay)(SDL_Vout *vout, SDL_VoutOverlay *overlay);
} SDL_Vout;

void VODSDL_VoutFree(SDL_Vout *vout);
void VODSDL_VoutFreeP(SDL_Vout **pvout);
int VODSDL_VoutDisplayYUVOverlay(SDL_Vout *vout, SDL_VoutOverlay *overlay);
SDL_VoutOverlay *VODSDL_Vout_CreateOverlay(int width, int height, Uint32 format, SDL_Vout *vout);
int VODSDL_VoutLockYUVOverlay(SDL_VoutOverlay *overlay);
int VODSDL_VoutUnlockYUVOverlay(SDL_VoutOverlay *overlay);
void VODSDL_VoutFreeYUVOverlay(SDL_VoutOverlay *overlay);
void VODSDL_VoutUnrefYUVOverlay(SDL_VoutOverlay *overlay);

#endif
