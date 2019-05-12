//
//  hevc_parser.h
//  IJKMedia
//
//  Created by shuyj on 2017/6/29.
//  Copyright © 2017年 MOMO. All rights reserved.
//

#ifndef vod_hevc_parser_h
#define vod_hevc_parser_h
#include "libavcodec/avcodec.h"
#include "libavcodec/bytestream.h"
#include "libavcodec/get_bits.h"
//#include "libavcodec/hevc.h"

#define IS_IDR(nal_unit_type) (nal_unit_type == HEVC_NAL_IDR_W_RADL || nal_unit_type == HEVC_NAL_IDR_N_LP)
#define IS_BLA(nal_unit_type) (nal_unit_type == HEVC_NAL_BLA_W_RADL || nal_unit_type == HEVC_NAL_BLA_W_LP || \
nal_unit_type == HEVC_NAL_BLA_N_LP)
#define IS_IRAP(nal_unit_type) (nal_unit_type >= 16 && nal_unit_type <= 23)
#define HEVC_MAX_SUB_LAYERS 7
/**
 * Table 7-3: NAL unit type codes
 */
enum HEVCNALUnitType {
    HEVC_NAL_TRAIL_N    = 0,
    HEVC_NAL_TRAIL_R    = 1,
    HEVC_NAL_TSA_N      = 2,
    HEVC_NAL_TSA_R      = 3,
    HEVC_NAL_STSA_N     = 4,
    HEVC_NAL_STSA_R     = 5,
    HEVC_NAL_RADL_N     = 6,
    HEVC_NAL_RADL_R     = 7,
    HEVC_NAL_RASL_N     = 8,
    HEVC_NAL_RASL_R     = 9,
    HEVC_NAL_BLA_W_LP   = 16,
    HEVC_NAL_BLA_W_RADL = 17,
    HEVC_NAL_BLA_N_LP   = 18,
    HEVC_NAL_IDR_W_RADL = 19,
    HEVC_NAL_IDR_N_LP   = 20,
    HEVC_NAL_CRA_NUT    = 21,
    HEVC_NAL_VPS        = 32,
    HEVC_NAL_SPS        = 33,
    HEVC_NAL_PPS        = 34,
    HEVC_NAL_AUD        = 35,
    HEVC_NAL_EOS_NUT    = 36,
    HEVC_NAL_EOB_NUT    = 37,
    HEVC_NAL_FD_NUT     = 38,
    HEVC_NAL_SEI_PREFIX = 39,
    HEVC_NAL_SEI_SUFFIX = 40,
};

enum HEVCSliceType {
    HEVC_SLICE_B = 0,
    HEVC_SLICE_P = 1,
    HEVC_SLICE_I = 2,
};

typedef struct FFHVCCNALUnitArray {
    uint8_t  array_completeness;
    uint8_t  NAL_unit_type;
    uint16_t numNalus;
    uint16_t *nalUnitLength;
    uint8_t  **nalUnit;
} FFHVCCNALUnitArray;

typedef struct FFHEVCDecoderConfigurationRecord {
    uint8_t  configurationVersion;
    uint8_t  general_profile_space;
    uint8_t  general_tier_flag;
    uint8_t  general_profile_idc;
    uint32_t general_profile_compatibility_flags;
    uint64_t general_constraint_indicator_flags;
    uint8_t  general_level_idc;
    uint16_t min_spatial_segmentation_idc;
    uint8_t  parallelismType;
    uint8_t  chromaFormat;
    uint8_t  bitDepthLumaMinus8;
    uint8_t  bitDepthChromaMinus8;
    uint16_t avgFrameRate;
    uint8_t  constantFrameRate;
    uint8_t  numTemporalLayers;
    uint8_t  temporalIdNested;
    uint8_t  lengthSizeMinusOne;
    uint8_t  numOfArrays;
    FFHVCCNALUnitArray *array;
} FFHEVCDecoderConfigurationRecord;

typedef struct FFHVCCProfileTierLevel {
    uint8_t  profile_space;
    uint8_t  tier_flag;
    uint8_t  profile_idc;
    uint32_t profile_compatibility_flags;
    uint64_t constraint_indicator_flags;
    uint8_t  level_idc;
} FFHVCCProfileTierLevel;

static int ff_hevc_extradata_to_annexb(uint8_t *extradata, uint32_t extradata_size, uint8_t** annexbdata, uint32_t* annexbsize, FFHEVCDecoderConfigurationRecord* hvcc);
bool vod_validate_hvcC_spc(uint8_t *extradata, uint32_t extrasize, int32_t *max_ref_frames, int *level, int *profile)
{
    // check the hvcC atom's sps for number of reference frames and validate profile level
    FFHEVCDecoderConfigurationRecord hvcc = {};
    uint8_t *new_extradata = NULL;
    uint32_t   new_extradata_size = 0;
    if( ff_hevc_extradata_to_annexb(extradata, extrasize, &new_extradata, &new_extradata_size, &hvcc) < 0 )
        return false;
    *profile = hvcc.general_profile_idc;
    *level = hvcc.general_level_idc;
    av_freep(&new_extradata);
    return true;
}
static uint8_t *ff_nal_unit_extract_rbsp(const uint8_t *src, uint32_t src_len,
                                      uint32_t *dst_len)
{
    uint8_t *dst;
    uint32_t i, len;
    
    dst = av_malloc(src_len + FF_INPUT_BUFFER_PADDING_SIZE);
    if (!dst)
        return NULL;
    
    /* NAL unit header (2 bytes) */
    i = len = 0;
    while (i < 2 && i < src_len)
        dst[len++] = src[i++];
    
    while (i + 2 < src_len)
        if (!src[i] && !src[i + 1] && src[i + 2] == 3) {
            dst[len++] = src[i++];
            dst[len++] = src[i++];
            i++; // remove emulation_prevention_three_byte
        } else
            dst[len++] = src[i++];
    
    while (i < src_len)
        dst[len++] = src[i++];
    
    *dst_len = len;
    return dst;
}
static void ff_nal_unit_parse_header(GetBitContext *gb, uint8_t *nal_type)
{
    skip_bits1(gb); // forbidden_zero_bit
    
    *nal_type = get_bits(gb, 6);
    
    /*
     * nuh_layer_id          u(6)
     * nuh_temporal_id_plus1 u(3)
     */
    skip_bits(gb, 9);
}
static void ff_hvcc_update_ptl(FFHEVCDecoderConfigurationRecord *hvcc,
                               FFHVCCProfileTierLevel *ptl)
{
    /*
     * The value of general_profile_space in all the parameter sets must be
     * identical.
     */
    hvcc->general_profile_space = ptl->profile_space;
    
    /*
     * The level indication general_level_idc must indicate a level of
     * capability equal to or greater than the highest level indicated for the
     * highest tier in all the parameter sets.
     */
    if (hvcc->general_tier_flag < ptl->tier_flag)
        hvcc->general_level_idc = ptl->level_idc;
    else
        hvcc->general_level_idc = FFMAX(hvcc->general_level_idc, ptl->level_idc);
    
    /*
     * The tier indication general_tier_flag must indicate a tier equal to or
     * greater than the highest tier indicated in all the parameter sets.
     */
    hvcc->general_tier_flag = FFMAX(hvcc->general_tier_flag, ptl->tier_flag);
    
    /*
     * The profile indication general_profile_idc must indicate a profile to
     * which the stream associated with this configuration record conforms.
     *
     * If the sequence parameter sets are marked with different profiles, then
     * the stream may need examination to determine which profile, if any, the
     * entire stream conforms to. If the entire stream is not examined, or the
     * examination reveals that there is no profile to which the entire stream
     * conforms, then the entire stream must be split into two or more
     * sub-streams with separate configuration records in which these rules can
     * be met.
     *
     * Note: set the profile to the highest value for the sake of simplicity.
     */
    hvcc->general_profile_idc = FFMAX(hvcc->general_profile_idc, ptl->profile_idc);
    
    /*
     * Each bit in general_profile_compatibility_flags may only be set if all
     * the parameter sets set that bit.
     */
    hvcc->general_profile_compatibility_flags &= ptl->profile_compatibility_flags;
    
    /*
     * Each bit in general_constraint_indicator_flags may only be set if all
     * the parameter sets set that bit.
     */
    hvcc->general_constraint_indicator_flags &= ptl->constraint_indicator_flags;
}

static void ff_hvcc_parse_ptl(GetBitContext *gb,
                              FFHEVCDecoderConfigurationRecord *hvcc,
                              unsigned int max_sub_layers_minus1)
{
    unsigned int i;
    FFHVCCProfileTierLevel general_ptl;
    uint8_t sub_layer_profile_present_flag[HEVC_MAX_SUB_LAYERS];
    uint8_t sub_layer_level_present_flag[HEVC_MAX_SUB_LAYERS];
    
    general_ptl.profile_space               = get_bits(gb, 2);
    general_ptl.tier_flag                   = get_bits1(gb);
    general_ptl.profile_idc                 = get_bits(gb, 5);
    general_ptl.profile_compatibility_flags = get_bits_long(gb, 32);
    general_ptl.constraint_indicator_flags  = get_bits64(gb, 48);
    general_ptl.level_idc                   = get_bits(gb, 8);
    ff_hvcc_update_ptl(hvcc, &general_ptl);
    
    for (i = 0; i < max_sub_layers_minus1; i++) {
        sub_layer_profile_present_flag[i] = get_bits1(gb);
        sub_layer_level_present_flag[i]   = get_bits1(gb);
    }
    
    if (max_sub_layers_minus1 > 0)
        for (i = max_sub_layers_minus1; i < 8; i++)
            skip_bits(gb, 2); // reserved_zero_2bits[i]
    
    for (i = 0; i < max_sub_layers_minus1; i++) {
        if (sub_layer_profile_present_flag[i]) {
            /*
             * sub_layer_profile_space[i]                     u(2)
             * sub_layer_tier_flag[i]                         u(1)
             * sub_layer_profile_idc[i]                       u(5)
             * sub_layer_profile_compatibility_flag[i][0..31] u(32)
             * sub_layer_progressive_source_flag[i]           u(1)
             * sub_layer_interlaced_source_flag[i]            u(1)
             * sub_layer_non_packed_constraint_flag[i]        u(1)
             * sub_layer_frame_only_constraint_flag[i]        u(1)
             * sub_layer_reserved_zero_44bits[i]              u(44)
             */
            skip_bits_long(gb, 32);
            skip_bits_long(gb, 32);
            skip_bits     (gb, 24);
        }
        
        if (sub_layer_level_present_flag[i])
            skip_bits(gb, 8);
    }
}
static int ff_hvcc_parse_vps(GetBitContext *gb, FFHEVCDecoderConfigurationRecord *hvcc)
{
    unsigned int vps_max_sub_layers_minus1;
    
    /*
     * vps_video_parameter_set_id u(4)
     * vps_reserved_three_2bits   u(2)
     * vps_max_layers_minus1      u(6)
     */
    skip_bits(gb, 12);
    
    vps_max_sub_layers_minus1 = get_bits(gb, 3);
    
    /*
     * numTemporalLayers greater than 1 indicates that the stream to which this
     * configuration record applies is temporally scalable and the contained
     * number of temporal layers (also referred to as temporal sub-layer or
     * sub-layer in ISO/IEC 23008-2) is equal to numTemporalLayers. Value 1
     * indicates that the stream is not temporally scalable. Value 0 indicates
     * that it is unknown whether the stream is temporally scalable.
     */
    hvcc->numTemporalLayers = FFMAX(hvcc->numTemporalLayers,
                                    vps_max_sub_layers_minus1 + 1);
    
    /*
     * vps_temporal_id_nesting_flag u(1)
     * vps_reserved_0xffff_16bits   u(16)
     */
    skip_bits(gb, 17);
    
    ff_hvcc_parse_ptl(gb, hvcc, vps_max_sub_layers_minus1);
    
    /* nothing useful for hvcC past this point */
    return 0;
}
static int ff_hvcc_parse_nalu(uint8_t *nal_buf, uint32_t nal_size, FFHEVCDecoderConfigurationRecord* hvcc)
{
    int ret = 0;
    GetBitContext gbc;
    uint8_t nal_type;
    uint8_t *rbsp_buf;
    uint32_t rbsp_size;
    
    rbsp_buf = ff_nal_unit_extract_rbsp(nal_buf, nal_size, &rbsp_size);
    if (!rbsp_buf) {
        ret = AVERROR(ENOMEM);
        goto end;
    }
    
    ret = init_get_bits8(&gbc, rbsp_buf, rbsp_size);
    if (ret < 0)
        goto end;
    
    ff_nal_unit_parse_header(&gbc, &nal_type);
    
    /*
     * Note: only 'declarative' SEI messages are allowed in
     * hvcC. Perhaps the SEI playload type should be checked
     * and non-declarative SEI messages discarded?
     */
    switch (nal_type) {
        case HEVC_NAL_VPS:
        case HEVC_NAL_SPS:
        case HEVC_NAL_PPS:
        case HEVC_NAL_SEI_PREFIX:
        case HEVC_NAL_SEI_SUFFIX:
            
            if (nal_type == HEVC_NAL_VPS)
                ret = ff_hvcc_parse_vps(&gbc, hvcc);
            else if (nal_type == HEVC_NAL_SPS)
                ;//ret = hvcc_parse_sps(&gbc, hvcc);
            else if (nal_type == HEVC_NAL_PPS)
                ;//ret = hvcc_parse_pps(&gbc, hvcc);
            if (ret < 0)
                goto end;
            break;
        default:
            ret = AVERROR_INVALIDDATA;
            goto end;
    }
    
end:
    av_free(rbsp_buf);
    return ret;
}

static int ff_hevc_extradata_to_annexb(uint8_t *extradata, uint32_t extradata_size, uint8_t** annexbdata, uint32_t* annexbsize, FFHEVCDecoderConfigurationRecord* hvcc)
{
    GetByteContext gb;
    int length_size, num_arrays, i, j;
    int cntlbyte = 0;
    int ret = 0;
    
    uint8_t *new_extradata = NULL;
    size_t   new_extradata_size = 0;
    
    bytestream2_init(&gb, extradata, extradata_size);
    
    bytestream2_skip(&gb, 21);
    cntlbyte = bytestream2_get_byte(&gb);
    length_size = ( cntlbyte & 3) + 1;
    hvcc->numTemporalLayers = ( cntlbyte >> 3 ) & 7;
    num_arrays  = bytestream2_get_byte(&gb);
    
    for (i = 0; i < num_arrays; i++) {
        int type = bytestream2_get_byte(&gb) & 0x3f;
        int cnt  = bytestream2_get_be16(&gb);
        
        if (!(type == HEVC_NAL_VPS || type == HEVC_NAL_SPS || type == HEVC_NAL_PPS ||
              type == HEVC_NAL_SEI_PREFIX || type == HEVC_NAL_SEI_SUFFIX)) {
            printf("Invalid NAL unit type in extradata: %d\n",type);
            ret = AVERROR_INVALIDDATA;
            goto fail;
        }
        
        for (j = 0; j < cnt; j++) {
            int nalu_len = bytestream2_get_be16(&gb);
            
            if (4 + FF_INPUT_BUFFER_PADDING_SIZE + nalu_len > SIZE_MAX - new_extradata_size) {
                ret = AVERROR_INVALIDDATA;
                goto fail;
            }
            ret = av_reallocp(&new_extradata, new_extradata_size + nalu_len + 4 + FF_INPUT_BUFFER_PADDING_SIZE);
            if (ret < 0)
                goto fail;
            AV_WB32(new_extradata + new_extradata_size, 1); // add the startcode
            bytestream2_get_buffer(&gb, new_extradata + new_extradata_size + 4, nalu_len);
            
            switch( type ){
                case HEVC_NAL_VPS:
                    ff_hvcc_parse_nalu(new_extradata + new_extradata_size + 4, nalu_len, hvcc);
                    break;
                default:
                    ;
            }
            
            new_extradata_size += 4 + nalu_len;
            memset(new_extradata + new_extradata_size, 0, FF_INPUT_BUFFER_PADDING_SIZE);
        }
    }
    
    *annexbdata      = new_extradata;
    *annexbsize      = new_extradata_size;
    
    if (!new_extradata_size)
        printf("No parameter sets in the extradata\n");
    
    return length_size;
fail:
    av_freep(&new_extradata);
    return ret;
}

#endif /* vod_hevc_parser_h */
