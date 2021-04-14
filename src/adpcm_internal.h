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
#ifndef __ADPCM_INTERNAL_H__
#define __ADPCM_INTERNAL_H__

/**
 * @file adpcm_internal.h
 * @author Chris Buchter
 * @brief Internal header for ADPCM Encode/Decode Library
 * 
 * This file contains the internal data structures and functions 
 * for the ADPCM Encode/Decode Library.
 */

/** @defgroup defines
 * @{
 */
#define ADPCM_INDEX_COMMAND_ID           (0)       ///< Index of the command id in ADPCM packet as defined in XVP Spec.
#define ADPCM_INDEX_STEP_SIZE_INDEX      (1)       ///< Index of the step size index in ADPCM packet as defined in XVP Spec.
#define ADPCM_INDEX_PREDICTED_SAMPLE_LSB (2)       ///< Index of the LSB Predicted sample in ADPCM packet as defined in XVP Spec.
#define ADPCM_INDEX_PREDICTED_SAMPLE_MSB (3)       ///< Index of the MSB Predicted sample in ADPCM packet as defined in XVP Spec.
#define ADPCM_HEADER_LENGTH              (4)       ///< Length of header in ADPCM packet as defined in XVP Spec.
#define ADPCM_PACKET_LENGTH_MAX          (100)     ///< Max Length of ADPCM command frame supported.

#define ADPCM_ODD_SAMPLE_GET(x)          (x & 0xF) ///< Function for getting odd adpcm samples (first, third, fifth...)
#define ADPCM_EVEN_SAMPLE_GET(x)         (x >> 4)  ///< Function for getting even adpcm samples (second, fourth, sixth...)

#define ADPCM_DECODE_MAGIC_NUMBER        (46)      ///< Magic number for decoder validity check
/**
 * @}
 */

/** @defgroup includes
 * @{
 */
#include <stdio.h>
#include "adpcm.h"
#include "adpcm_log.h"
/**
 * @}
 */

/** @defgroup structs_decode
 * @{
 */

/**
 * @brief ADPCM decoder that will be passed in for each API call.
 * 
 * This structure will keep all the internal state and stats for a decoder.
 */
struct adpcm_dec_t {
    uint8_t              magic_number;        ///< Magic number to determine if object is value
    uint8_t              expected_command_id; ///< The next expected command id.
    int8_t               step_size_index;     ///< Step size index for ADPCM decoding.
    int16_t              predicted_sample;    ///< Predicted sample for ADPCM decoding.
    adpcm_decode_stats_t stats;               ///< Stats for the current decoding session.
};
/**
 * @}
 */

#endif
