/*
 * h264_sps_parser.h
 *
 * Copyright (c) 2014 Zhou Quan <zhouqicy@gmail.com>
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
#ifndef vod_h264_sps_parser_h
#define vod_h264_sps_parser_h

#define AV_RB16(x)                          \
((((const uint8_t*)(x))[0] <<  8) |        \
((const uint8_t*)(x)) [1])

#define AV_RB24(x)                          \
((((const uint8_t*)(x))[0] << 16) |        \
(((const uint8_t*)(x))[1] <<  8) |        \
((const uint8_t*)(x))[2])

#define AV_RB32(x)                          \
((((const uint8_t*)(x))[0] << 24) |        \
(((const uint8_t*)(x))[1] << 16) |        \
(((const uint8_t*)(x))[2] <<  8) |        \
((const uint8_t*)(x))[3])


/* NAL unit types */
enum {
    NAL_SLICE           = 1,
    NAL_DPA             = 2,
    NAL_DPB             = 3,
    NAL_DPC             = 4,
    NAL_IDR_SLICE       = 5,
    NAL_SEI             = 6,
    NAL_SPS             = 7,
    NAL_PPS             = 8,
    NAL_AUD             = 9,
    NAL_END_SEQUENCE    = 10,
    NAL_END_STREAM      = 11,
    NAL_FILLER_DATA     = 12,
    NAL_SPS_EXT         = 13,
    NAL_AUXILIARY_SLICE = 19,
    NAL_FF_IGNORE       = 0xff0f001,
};


typedef struct
{
    const uint8_t *data;
    const uint8_t *end;
    int head;
    uint64_t cache;
} nal_bitstream;

static void
nal_bs_init(nal_bitstream *bs, const uint8_t *data, size_t size)
{
    bs->data = data;
    bs->end  = data + size;
    bs->head = 0;
    // fill with something other than 0 to detect
    //  emulation prevention bytes
    bs->cache = 0xffffffff;
}

static uint64_t
nal_bs_read(nal_bitstream *bs, int n)
{
    uint64_t res = 0;
    int shift;

    if (n == 0)
        return res;

    // fill up the cache if we need to
    while (bs->head < n) {
        uint8_t a_byte;
        bool check_three_byte;

        check_three_byte = true;
    next_byte:
        if (bs->data >= bs->end) {
            // we're at the end, can't produce more than head number of bits
            n = bs->head;
            break;
        }
        // get the byte, this can be an emulation_prevention_three_byte that we need
        // to ignore.
        a_byte = *bs->data++;
        if (check_three_byte && a_byte == 0x03 && ((bs->cache & 0xffff) == 0)) {
            // next byte goes unconditionally to the cache, even if it's 0x03
            check_three_byte = false;
            goto next_byte;
        }
        // shift bytes in cache, moving the head bits of the cache left
        bs->cache = (bs->cache << 8) | a_byte;
        bs->head += 8;
    }

    // bring the required bits down and truncate
    if ((shift = bs->head - n) > 0)
        res = bs->cache >> shift;
    else
        res = bs->cache;

    // mask out required bits
    if (n < 32)
        res &= (1 << n) - 1;

    bs->head = shift;

    return res;
}

static bool
nal_bs_eos(nal_bitstream *bs)
{
    return (bs->data >= bs->end) && (bs->head == 0);
}

// read unsigned Exp-Golomb code
static int64_t
nal_bs_read_ue(nal_bitstream *bs)
{
    int i = 0;

    while (nal_bs_read(bs, 1) == 0 && !nal_bs_eos(bs) && i < 32)
        i++;

    return ((1 << i) - 1 + nal_bs_read(bs, i));
}

typedef struct
{
    uint64_t profile_idc;
    uint64_t level_idc;
    uint64_t sps_id;

    uint64_t chroma_format_idc;
    uint64_t separate_colour_plane_flag;
    uint64_t bit_depth_luma_minus8;
    uint64_t bit_depth_chroma_minus8;
    uint64_t qpprime_y_zero_transform_bypass_flag;
    uint64_t seq_scaling_matrix_present_flag;

    uint64_t log2_max_frame_num_minus4;
    uint64_t pic_order_cnt_type;
    uint64_t log2_max_pic_order_cnt_lsb_minus4;

    uint64_t max_num_ref_frames;
    uint64_t gaps_in_frame_num_value_allowed_flag;
    uint64_t pic_width_in_mbs_minus1;
    uint64_t pic_height_in_map_units_minus1;

    uint64_t frame_mbs_only_flag;
    uint64_t mb_adaptive_frame_field_flag;

    uint64_t direct_8x8_inference_flag;

    uint64_t frame_cropping_flag;
    uint64_t frame_crop_left_offset;
    uint64_t frame_crop_right_offset;
    uint64_t frame_crop_top_offset;
    uint64_t frame_crop_bottom_offset;
} sps_info_struct;

static void parseh264_sps(uint8_t *sps, uint32_t sps_size,  int *level, int *profile, bool *interlaced, int32_t *max_ref_frames)
{
    nal_bitstream bs;
    sps_info_struct sps_info = {0};

    nal_bs_init(&bs, sps, sps_size);

    sps_info.profile_idc  = nal_bs_read(&bs, 8);
    nal_bs_read(&bs, 1);  // constraint_set0_flag
    nal_bs_read(&bs, 1);  // constraint_set1_flag
    nal_bs_read(&bs, 1);  // constraint_set2_flag
    nal_bs_read(&bs, 1);  // constraint_set3_flag
    nal_bs_read(&bs, 4);  // reserved
    sps_info.level_idc    = nal_bs_read(&bs, 8);
    sps_info.sps_id       = nal_bs_read_ue(&bs);

    if (sps_info.profile_idc == 100 ||
        sps_info.profile_idc == 110 ||
        sps_info.profile_idc == 122 ||
        sps_info.profile_idc == 244 ||
        sps_info.profile_idc == 44  ||
        sps_info.profile_idc == 83  ||
        sps_info.profile_idc == 86)
    {
            sps_info.chroma_format_idc                    = nal_bs_read_ue(&bs);
            if (sps_info.chroma_format_idc == 3)
                sps_info.separate_colour_plane_flag         = nal_bs_read(&bs, 1);
            sps_info.bit_depth_luma_minus8                = nal_bs_read_ue(&bs);
            sps_info.bit_depth_chroma_minus8              = nal_bs_read_ue(&bs);
            sps_info.qpprime_y_zero_transform_bypass_flag = nal_bs_read(&bs, 1);

            sps_info.seq_scaling_matrix_present_flag = nal_bs_read (&bs, 1);
            if (sps_info.seq_scaling_matrix_present_flag)
            {
                /* TODO: unfinished */
            }
    }
    sps_info.log2_max_frame_num_minus4 = nal_bs_read_ue(&bs);
    if (sps_info.log2_max_frame_num_minus4 > 12) {
        // must be between 0 and 12
        // don't early return here - the bits we are using (profile/level/interlaced/ref frames)
        // might still be valid - let the parser go on and pray.
        //return;
    }

    sps_info.pic_order_cnt_type = nal_bs_read_ue(&bs);
    if (sps_info.pic_order_cnt_type == 0) {
        sps_info.log2_max_pic_order_cnt_lsb_minus4 = nal_bs_read_ue(&bs);
    }
    else if (sps_info.pic_order_cnt_type == 1) { // TODO: unfinished
        /*
         delta_pic_order_always_zero_flag = gst_nal_bs_read (bs, 1);
         offset_for_non_ref_pic = gst_nal_bs_read_se (bs);
         offset_for_top_to_bottom_field = gst_nal_bs_read_se (bs);

         num_ref_frames_in_pic_order_cnt_cycle = gst_nal_bs_read_ue (bs);
         for( i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++ )
         offset_for_ref_frame[i] = gst_nal_bs_read_se (bs);
         */
    }

    sps_info.max_num_ref_frames             = nal_bs_read_ue(&bs);
    sps_info.gaps_in_frame_num_value_allowed_flag = nal_bs_read(&bs, 1);
    sps_info.pic_width_in_mbs_minus1        = nal_bs_read_ue(&bs);
    sps_info.pic_height_in_map_units_minus1 = nal_bs_read_ue(&bs);

    sps_info.frame_mbs_only_flag            = nal_bs_read(&bs, 1);
    if (!sps_info.frame_mbs_only_flag)
        sps_info.mb_adaptive_frame_field_flag = nal_bs_read(&bs, 1);

    sps_info.direct_8x8_inference_flag      = nal_bs_read(&bs, 1);

    sps_info.frame_cropping_flag            = nal_bs_read(&bs, 1);
    if (sps_info.frame_cropping_flag) {
        sps_info.frame_crop_left_offset       = nal_bs_read_ue(&bs);
        sps_info.frame_crop_right_offset      = nal_bs_read_ue(&bs);
        sps_info.frame_crop_top_offset        = nal_bs_read_ue(&bs);
        sps_info.frame_crop_bottom_offset     = nal_bs_read_ue(&bs);
    }

    *level = (int)sps_info.level_idc;
    *profile = (int)sps_info.profile_idc;
    *interlaced = (int)!sps_info.frame_mbs_only_flag;
    *max_ref_frames = (int)sps_info.max_num_ref_frames;
}

static bool validate_avcC_spc(uint8_t *extradata, uint32_t extrasize, int32_t *max_ref_frames, int *level, int *profile)
{
    // check the avcC atom's sps for number of reference frames and
    // bail if interlaced, VDA does not handle interlaced h264.
    bool interlaced = true;
    uint8_t *spc = extradata + 6;
    uint32_t sps_size = AV_RB16(spc);
    if (sps_size)
        parseh264_sps(spc+3, sps_size-1, level, profile, &interlaced, max_ref_frames);
    if (interlaced)
        return false;
    return true;
}

static inline int ff_get_nal_units_type(const uint8_t * const data) {
    return   data[4] & 0x1f;
}

static uint32_t bytesToInt(const uint8_t* src) {
    uint32_t value;
    value = (uint32_t)((src[0] & 0xFF)<<24|(src[1]&0xFF)<<16|(src[2]&0xFF)<<8|(src[3]&0xFF));
    return value;
}

static void printMem(const uint8_t *mem, size_t size, const char *format, int num_in_line){
    if (format == NULL) {
        format = "%02x ";
    }
    
    if (num_in_line == 0) {
        num_in_line = 16;
    }
    
    for (int i=0; i<size; i++) {
        printf(format, mem[i]);
        if ((i+1)%num_in_line == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

static void printMemHex(const uint8_t *mem, size_t size){
    printMem(mem, size, "%02x ", 16);
}

static const uint8_t start_code_4bytes[4] = {0x00, 0x00, 0x00, 0x01};
static const uint8_t start_code_3bytes[3] = {0x00, 0x00, 0x01};

static inline int ff_get_nal_units_type_with_start_code_size(const uint8_t * const data, size_t start_code_size) {
    return   data[start_code_size] & 0x1f;
}

/*
 * return: if begin with start code, return the size of start code,
 *          else 0
 */
static size_t get_startcode_size(const uint8_t *data) {
    size_t start_code_size = 0;
    const uint32_t start_4bytes = htonl(0x00000001);
//    const uint32_t start_3bytes = htonl(0x00000100);
    
    if (  (*(uint32_t *)&data[0]) == start_4bytes ) {
        start_code_size = sizeof(start_4bytes);
    }
    
    //3bytes_startcode is confict with payload_size
//    else if ( ((*(uint32_t *)&data[0])&0xFFFFFF00) == start_3bytes){
//        start_code_size = sizeof(start_3bytes);
//    }

    return start_code_size;
}

static const uint8_t *find_next_startcode(const uint8_t *data, size_t data_size, size_t *out_start_code_size) {
    *out_start_code_size = 0;
    const uint8_t* next_nal_start = NULL;

    for (int i = 0; i<data_size-4; i++) {
        if (memcmp((void*)(data+i), start_code_4bytes, 4) == 0) {
            *out_start_code_size = sizeof(start_code_4bytes);
            next_nal_start = data+i;
            break;
        } else if (memcmp((void*)(data+i), start_code_3bytes, 3) == 0) {
            *out_start_code_size = sizeof(start_code_3bytes);
            next_nal_start = data+i;
            break;
        }
    }
    
    return next_nal_start;
}

static bool ff_avpacket_with_startcode_is_sei(const AVPacket* pkt, const uint8_t **sei_ptr, int *sei_size) {
    int state = -1;
    *sei_size = 0;
    *sei_ptr = NULL;

//    printf("ff_avpacket_with_startcode_is_sei pkt->size=%d\n", pkt->size);
//    printMemHex(pkt->data, 128);

    if (pkt->data && pkt->size >= 5) {
        int offset = 0;
        while (offset >= 0 && offset + 5 <= pkt->size) {
            const uint8_t* nal_start = pkt->data+offset;
//            printf("nal offset=%d\n", offset);
//            printMemHex(nal_start, 16);

            size_t next_start_code_size = 0;
            const uint8_t* next_nal_start = find_next_startcode(nal_start, pkt->size - offset,  &next_start_code_size);
            if (next_nal_start) {
                size_t nal_size = next_nal_start - nal_start;
                offset += nal_size + next_start_code_size;
//                printf("next nal offset=%d, nal_size=%zu, next_start_code_size=%zu\n", offset, nal_size, next_start_code_size);
//                printMemHex(nal_start, 16);
            } else {
                return false;
            }

            state = ff_get_nal_units_type_with_start_code_size(next_nal_start, next_start_code_size);
            if (state == NAL_SEI) {
                nal_start = next_nal_start + next_start_code_size;//skip the start code
                next_nal_start = find_next_startcode(nal_start, pkt->size - offset, &next_start_code_size);
                size_t nal_size = 0;
                if (next_nal_start) {
                    nal_size = next_nal_start - nal_start;
                } else {
                    nal_size = pkt->size - (nal_start - pkt->data);//until the end of the data
                }

                if (nal_size < pkt->size) {
                    *sei_size = (int)nal_size;
                    *sei_ptr = nal_start;
                    printf("with_startcode got sei, sei_size=%d\n", *sei_size);
//                    printMemHex(nal_start, *sei_size);
                    return true;
                } else {
                    printf("Error: sei_size=%zu, detect sei_size too suspicious\n", nal_size);
                    printMemHex(nal_start, 16);
                    return false;
                }
            }
        }
    }

    return false;
}

static bool ff_avpacket_with_size_is_sei(const AVPacket* pkt, const uint8_t **sei_ptr, int *sei_size) {
    int state = -1;
    *sei_size = 0;
    *sei_ptr = NULL;

    if (pkt->data && pkt->size >= 5) {
        int offset = 0;
        while (offset >= 0 && offset + 5 <= pkt->size) {
            void* nal_start = pkt->data+offset;
//            printf("sei offset=%d, next_nal_size= %d \n", offset, bytesToInt(nal_start));
//            printMemHex(nal_start, 4);
            
            state = ff_get_nal_units_type(nal_start);
            if (state == NAL_SEI) {
                *sei_size = bytesToInt(nal_start);
                *sei_ptr = nal_start+4;
                
//                printf("with_size got sei size=%d\n", *sei_size);
                return true;
            }
            
            offset+=(bytesToInt(nal_start) + 4);
        }
    }
    
    return false;
}

static bool ff_avpacket_is_sei(const AVPacket* pkt, const uint8_t **sei_ptr, int *sei_size) {
    *sei_size = 0;
    *sei_ptr = NULL;

    if (pkt->data && pkt->size >= 5) {
        size_t start_code_size = get_startcode_size(pkt->data);
        assert(start_code_size == 0 || start_code_size == 3 || start_code_size == 4);
        if (start_code_size) {
            return ff_avpacket_with_startcode_is_sei(pkt, sei_ptr, sei_size);
        } else {
            return ff_avpacket_with_size_is_sei(pkt, sei_ptr, sei_size);
        }
    }
    
    return false;
}

static bool ff_avpacket_is_idr(const AVPacket* pkt) {
    int state = -1;
    
    if (pkt->data && pkt->size >= 5) {
        //In weila conference mode, all packets are start with start_code
//        size_t startcode_size = get_startcode_size(pkt->data);
//        if (startcode_size) {
//            printf("ff_avpacket_is_idr: start code to find IDR frame, data=0x%x %x %x %x\n",
//                   pkt->data[0], pkt->data[1], pkt->data[2], pkt->data[3]);
//            return false;
//        }
        
        int offset = 0;
        while (offset >= 0 && offset + 5 <= pkt->size) {
            void* nal_start = pkt->data+offset;
            state = ff_get_nal_units_type(nal_start);
            if (state == NAL_IDR_SLICE) {
                return true;
            }
            offset+=(bytesToInt(nal_start) + 4);
        }
    }
    return false;
}

static bool ff_avpacket_is_key(const AVPacket* pkt) {
    if (pkt->flags & AV_PKT_FLAG_KEY) {
        return true;
    } else {
        return false;
    }
}

static bool ff_avpacket_i_or_idr(const AVPacket* pkt,bool isIdr) {
    if (isIdr == true) {
        return true;
    } else {
        return ff_avpacket_is_key(pkt);
    }
}



#endif
