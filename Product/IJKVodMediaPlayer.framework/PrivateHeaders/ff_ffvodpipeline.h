/*
 * ff_ffvodpipeline.h
 *
 * Copyright (c) 2014 Zhang Rui <bbcallen@gmail.com>
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

#ifndef ff_ffvodpipeline_h
#define ff_ffvodpipeline_h

#include "ijkvodsdl/ijkvodsdl_class.h"
#include "ijkvodsdl/ijkvodsdl_mutex.h"
#include "ijkvodsdl/ijkvodsdl_aout.h"
#include "ff_ffvodpipenode.h"

typedef struct IJKFF_Pipeline_Opaque IJKFF_Pipeline_Opaque;
typedef struct IJKFFVod_Pipeline IJKFFVod_Pipeline;
typedef struct IJKFFVod_Pipeline {
    SDL_Class             *opaque_class;
    IJKFF_Pipeline_Opaque *opaque;

    void            (*func_destroy)             (IJKFFVod_Pipeline *pipeline);
    IJKFF_Pipenode *(*func_open_video_decoder)  (IJKFFVod_Pipeline *pipeline, FFPlayer *ffp);
    IJKFF_Pipenode *(*func_open_video_output)   (IJKFFVod_Pipeline *pipeline, FFPlayer *ffp);
    SDL_Aout       *(*func_open_audio_output)   (IJKFFVod_Pipeline *pipeline, FFPlayer *ffp);
} IJKFFVod_Pipeline;

IJKFFVod_Pipeline *ffvodpipeline_alloc(SDL_Class *opaque_class, size_t opaque_size);
void ffvodpipeline_free(IJKFFVod_Pipeline *pipeline);
void ffvodpipeline_free_p(IJKFFVod_Pipeline **pipeline);

IJKFF_Pipenode *ffvodpipeline_open_video_decoder(IJKFFVod_Pipeline *pipeline, FFPlayer *ffp);
IJKFF_Pipenode *ffvodpipeline_open_video_output(IJKFFVod_Pipeline *pipeline, FFPlayer *ffp);

SDL_Aout       *ffvodpipeline_open_audio_output(IJKFFVod_Pipeline *pipeline, FFPlayer *ffp);

#endif
