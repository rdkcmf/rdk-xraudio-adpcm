/*
##########################################################################
# If not stated otherwise in this file or this component's LICENSE
# file the following copyright and licenses apply:
#
# Copyright 2019 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################
*/
/**
 * @file adpcm_decode.c
 * @author Chris Buchter
 * @brief File contains the ADPCM Decode Implementation.
 * 
 * This file contains the implementation of the ADPCM Decode API.
 */

// Includes
#include "adpcm_internal.h"

// Static Globals
uint16_t table_step_size[] = {7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
                              19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
                              50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
                              130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
                              337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
                              876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
                              2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
                              5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
                              15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767};

int8_t table_index[] = {-1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8};

// Static Function Implementations
static uint8_t command_id_next(uint8_t command_id, uint8_t command_id_min, uint8_t command_id_max) {
    uint8_t ret = command_id + 1;
    if(ret > command_id_max) {
        ret = command_id_min;
    }
    return(ret);
}

static uint8_t command_id_diff(uint8_t expected, uint8_t received, uint8_t command_id_min, uint8_t command_id_max) {
    if(received > expected) {
        return(received - expected);
    } else {
        return((command_id_max + 1 - expected) + (received - command_id_min));    
    }
}

static pcm_t adpcm_decode_single(adpcm_dec_t *decoder, adpcm_t halfbyte) {
    int32_t difference       = 0;
    pcm_t  new_sample       = decoder->predicted_sample;
    int8_t step_size_index   = decoder->step_size_index;
    uint16_t step_size       = table_step_size[step_size_index];

    if(halfbyte & 4) {
        difference += step_size;
    }
    if(halfbyte & 2) {
        difference += step_size >> 1;
    }
    if(halfbyte & 1) {
        difference += step_size >> 2;
    }
    difference += step_size >> 3;

    if(halfbyte & 8) {
        difference = -difference;
    }

    if((new_sample + difference) > INT16_MAX) {
        new_sample = INT16_MAX;
    } else if((new_sample + difference) < INT16_MIN) {
        new_sample = INT16_MIN;
    } else {
        new_sample += difference;
    }

    if((step_size_index + table_index[halfbyte]) < 0) {
        step_size_index = 0;
    } else if((step_size_index + table_index[halfbyte]) > 88) {
        step_size_index = 88;
    } else {
        step_size_index += table_index[halfbyte];
    }

    decoder->predicted_sample       = new_sample;
    decoder->step_size_index        = step_size_index;
    decoder->stats.samples_decoded += 1;

    return new_sample;
}

// Function Implementations
adpcm_dec_t *adpcm_decode_create() {
    // Allocate new decoder
    adpcm_dec_t *ret = (adpcm_dec_t *)malloc(sizeof(adpcm_dec_t));
    if(NULL == ret) {
        XLOGD_ERROR("%s: failed to allocate memory for decoder object", __FUNCTION__);
        return(ret);
    }

    // Zero out decoder attributes
    memset(ret, 0, sizeof(adpcm_dec_t));

    // Set magic number
    ret->magic_number = ADPCM_DECODE_MAGIC_NUMBER;
    
    return(ret);
}

int32_t adpcm_deframe(adpcm_dec_t *decoder, uint8_t *inbuf, uint32_t inlen, uint8_t command_id_min, uint8_t command_id_max) {
   // Check parameters
   if(NULL == decoder) {
       XLOGD_ERROR("%s: decoder is null", __FUNCTION__);
       return(-1);
   } else if(NULL == inbuf) {
       XLOGD_ERROR("%s: inbuf is null", __FUNCTION__);
       decoder->stats.failed_decodes += 1;
       return(-1);
   } else if(ADPCM_DECODE_MAGIC_NUMBER != decoder->magic_number) {
       XLOGD_ERROR("%s: decoder object validity test fail", __FUNCTION__);
       return(-1);
   } else if(false == adpcm_analyze(decoder, inbuf, inlen, command_id_min, command_id_max)) {
       XLOGD_ERROR("%s: analyzing packet failed, do not decode\n", __FUNCTION__);
       // Decode fails has already been incremented from analyze routine
       return(-1);
   }

   // Shift the data to remove the header (would be nice to optimize this copy away later)
   uint8_t *payload = &inbuf[ADPCM_HEADER_LENGTH];
   for(uint32_t i = ADPCM_HEADER_LENGTH; i < inlen; i++) {
      *inbuf = *payload;
   }

   return(inlen - ADPCM_HEADER_LENGTH);
}

bool adpcm_analyze(adpcm_dec_t *decoder, adpcm_t *inbuf, uint32_t inlen, uint8_t command_id_min, uint8_t command_id_max) {
    uint8_t   command_id;
    uint8_t   step_size_index;
    int16_t   predicted_sample;

    // Check parameters
    if(NULL == decoder) {
        XLOGD_ERROR("%s: decoder is null", __FUNCTION__);
        return(false);
    } else if(NULL == inbuf) {
        XLOGD_ERROR("%s: inbuf is null", __FUNCTION__);
        decoder->stats.failed_decodes += 1;
        return(false);
    } else if(ADPCM_DECODE_MAGIC_NUMBER != decoder->magic_number) {
        XLOGD_ERROR("%s: decoder object validity test fail", __FUNCTION__);
        return(false);
    }

    // Check if inbuf is min possible length
    if(inlen < (ADPCM_HEADER_LENGTH + 1)) { // 8bit command id + 8bit step size index + 16bit predicted + 8bit data -- minimum case
        XLOGD_ERROR("%s: inlen is less than minimum size (%u)", __FUNCTION__, inlen);
        decoder->stats.failed_decodes += 1;
        return(false);
    }

    if(inlen > ADPCM_PACKET_LENGTH_MAX) {
        XLOGD_WARN("%s: WARNING - inlen (%u) is greater than max supported packet length (%u)", __FUNCTION__, inlen, ADPCM_PACKET_LENGTH_MAX);
    }

    // Get command_id, step_size_index, and predicted_sample
    command_id        = inbuf[ADPCM_INDEX_COMMAND_ID];
    if(command_id < command_id_min || command_id > command_id_max) {
        XLOGD_ERROR("%s: command id is out of bounds", __FUNCTION__);
        decoder->stats.failed_decodes += 1;
        return(false);
    }
    step_size_index   = inbuf[ADPCM_INDEX_STEP_SIZE_INDEX];
    predicted_sample  = inbuf[ADPCM_INDEX_PREDICTED_SAMPLE_LSB] + (inbuf[ADPCM_INDEX_PREDICTED_SAMPLE_MSB] << 8);

    // Check if this is first packet
    if(decoder->expected_command_id == 0) { // This is first packet.
        decoder->expected_command_id = command_id_next(command_id, command_id_min, command_id_max);
        if(step_size_index != 0) {
            decoder->stats.step_size_mismatch += 1;
        }
        decoder->step_size_index = step_size_index;

        if(predicted_sample != 0) {
            decoder->stats.predicted_sample_mismatch += 1;
        }
        decoder->predicted_sample = predicted_sample;
    } else { // This is not the first packet
        // Check the command ID
        if(command_id != decoder->expected_command_id) {
            decoder->stats.commands_lost += command_id_diff(decoder->expected_command_id, command_id, command_id_min, command_id_max);
        }
        decoder->expected_command_id = command_id_next(command_id, command_id_min, command_id_max);

        // Check the step size index
        if(decoder->step_size_index != step_size_index) {
            decoder->stats.step_size_mismatch += 1;
        }
        decoder->step_size_index = step_size_index;

        // Check the predicted sample
        if(decoder->predicted_sample != predicted_sample) {
            decoder->stats.predicted_sample_mismatch += 1;
        }
        decoder->predicted_sample = predicted_sample;
    }
    decoder->stats.commands_processed += 1;
    return(true);
}

int32_t adpcm_decode(adpcm_dec_t *decoder, adpcm_t *inbuf, uint32_t inlen, pcm_t *outbuf, uint32_t outlen, uint8_t command_id_min, uint8_t command_id_max, bool is_big_endian) {
    uint32_t  i      = 0;
    int32_t   samples_decoded = 0;

    // Check parameters
    if(NULL == decoder) {
        XLOGD_ERROR("%s: decoder is null", __FUNCTION__);
        return(-1);
    } else if(NULL == inbuf) {
        XLOGD_ERROR("%s: inbuf is null", __FUNCTION__);
        decoder->stats.failed_decodes += 1;
        return(-1);
    } else if(NULL == outbuf) {
        XLOGD_ERROR("%s: outbuf is null", __FUNCTION__);
        decoder->stats.failed_decodes += 1;
        return(-1);
    } else if(ADPCM_DECODE_MAGIC_NUMBER != decoder->magic_number) {
        XLOGD_ERROR("%s: decoder object validity test fail", __FUNCTION__);
        return(-1);
    } else if(false == adpcm_analyze(decoder, inbuf, inlen, command_id_min, command_id_max)) {
        XLOGD_ERROR("%s: analyzing packet failed, do not decode\n", __FUNCTION__);
        // Decode fails has already been incremented from analyze routine
        return(-1);
    }

    inbuf            += ADPCM_HEADER_LENGTH;
    inlen            -= ADPCM_HEADER_LENGTH;

    // Check to make sure output buffer is long enough
    if(outlen < (inlen * 2)) { // Each byte in inbuf contains 2 ADPCM samples.
        XLOGD_ERROR("%s: not enough memory in outbuf", __FUNCTION__);
        decoder->stats.failed_decodes += 1;
        return(-1);
    }

    // Clear the output buffer
    memset(outbuf, 0, sizeof(pcm_t) * outlen);

    // Do the actual decoding here.
    for(i = 0; i < inlen; i++) {
        if (is_big_endian) {
            outbuf[2*i]     = adpcm_decode_single(decoder, ADPCM_ODD_SAMPLE_GET(inbuf[i]));
            outbuf[(2*i)+1] = adpcm_decode_single(decoder, ADPCM_EVEN_SAMPLE_GET(inbuf[i]));
        } else {
            outbuf[2*i]     = adpcm_decode_single(decoder, ADPCM_EVEN_SAMPLE_GET(inbuf[i]));
            outbuf[(2*i)+1] = adpcm_decode_single(decoder, ADPCM_ODD_SAMPLE_GET(inbuf[i]));
        }
        samples_decoded += 2;
    }

    return(samples_decoded);

}

bool adpcm_decode_stats(adpcm_dec_t *decoder, adpcm_decode_stats_t *stats) {
    if(NULL == decoder) {
        XLOGD_ERROR("%s: decoder is null", __FUNCTION__);
        return(false);
    } else if(NULL == stats) {
        XLOGD_ERROR("%s: stats are null", __FUNCTION__);
        return(false);
    } else if(ADPCM_DECODE_MAGIC_NUMBER != decoder->magic_number) {
        XLOGD_ERROR("%s: decoder object validity test fail", __FUNCTION__);
        return(false);
    }
    memcpy(stats, &decoder->stats, sizeof(adpcm_decode_stats_t));
    return(true);
}

bool adpcm_decode_reset(adpcm_dec_t *decoder) {
    if(NULL == decoder) {
        XLOGD_ERROR("%s: decoder is null", __FUNCTION__);
        return(false);
    } else if(ADPCM_DECODE_MAGIC_NUMBER != decoder->magic_number) {
        XLOGD_ERROR("%s: decoder object validity test fail", __FUNCTION__);
        return(false);
    }

    // Zero out decoder attributes
    memset(decoder, 0, sizeof(adpcm_dec_t));
    decoder->magic_number = ADPCM_DECODE_MAGIC_NUMBER;

    return(true);
}

void adpcm_decode_destroy(adpcm_dec_t *decoder) {
    if(decoder) {
        free(decoder);
    }
}
