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
 * @file test.c
 * @author Chris Buchter
 * @brief File contains tests for the ADPCM library
 * 
 * This file contains tests for the ADPCM library.
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "adpcm.h"

typedef bool (*adpcm_decode_test)(adpcm_dec_t *);
static bool adpcm_decode_test_1(adpcm_dec_t *decoder);
static bool adpcm_decode_test_2(adpcm_dec_t *decoder);
static bool adpcm_decode_test_3(adpcm_dec_t *decoder);
static bool adpcm_decode_test_4(adpcm_dec_t *decoder);
static bool adpcm_decode_test_5(adpcm_dec_t *decoder);
static bool adpcm_decode_test_6(adpcm_dec_t *decoder);
static bool adpcm_decode_test_7(adpcm_dec_t *decoder);
static bool adpcm_decode_test_8(adpcm_dec_t *decoder);
static bool adpcm_decode_test_9(adpcm_dec_t *decoder);
static bool adpcm_decode_test_10(adpcm_dec_t *decoder);
static bool adpcm_decode_test_11(adpcm_dec_t *decoder);
static bool adpcm_decode_test_12(adpcm_dec_t *decoder);
static bool adpcm_decode_test_13(adpcm_dec_t *decoder);
static bool adpcm_decode_test_14(adpcm_dec_t *decoder);
static bool adpcm_decode_test_15(adpcm_dec_t *decoder);
static bool adpcm_decode_test_16(adpcm_dec_t *decoder);
static bool adpcm_decode_test_17(adpcm_dec_t *decoder);
static bool adpcm_decode_test_18(adpcm_dec_t *decoder);
static bool adpcm_decode_test_19(adpcm_dec_t *decoder);
static bool adpcm_decode_test_20(adpcm_dec_t *decoder);
static bool adpcm_decode_test_21(adpcm_dec_t *decoder);
static bool adpcm_decode_test_22(adpcm_dec_t *decoder);
static bool adpcm_decode_test_23(adpcm_dec_t *decoder);

static adpcm_decode_test adpcm_decode_tests[] = {
    adpcm_decode_test_1,
    adpcm_decode_test_2,
    adpcm_decode_test_3,
    adpcm_decode_test_4,
    adpcm_decode_test_5,
    adpcm_decode_test_6,
    adpcm_decode_test_7,
    adpcm_decode_test_8,
    adpcm_decode_test_9,
    adpcm_decode_test_10,
    adpcm_decode_test_11,
    adpcm_decode_test_12,
    adpcm_decode_test_13,
    adpcm_decode_test_14,
    adpcm_decode_test_15,
    adpcm_decode_test_16,
    adpcm_decode_test_17,
    adpcm_decode_test_18,
    adpcm_decode_test_19,
    adpcm_decode_test_20,
    adpcm_decode_test_21,
    adpcm_decode_test_22,
    adpcm_decode_test_23
};

int main() {
    int tests = sizeof(adpcm_decode_tests) / sizeof(adpcm_decode_tests[0]);
    int tests_failed = 0, tests_passed = 0, i = 0;
    adpcm_dec_t *decoder = NULL;

    printf("%s: Creating decoder\n", __FUNCTION__);
    decoder = adpcm_decode_create();
    if(NULL == decoder) {
        printf("%s: Failed to create decoder\n", __FUNCTION__);
        exit(1);
    }

    // Start tests
    printf("%s: ADPCM Library Tests\n", __FUNCTION__);
    for(i = 0; i < tests; i++) {
        printf("%s: Running test %d of %d\n", __FUNCTION__, i+1, tests);
        if(adpcm_decode_tests[i](decoder) == true) {
            printf("%s: Test %d Passed\n", __FUNCTION__, i+1);
            tests_passed += 1;
        } else {
            printf("%s: Test %d Failed\n", __FUNCTION__, i+1);
            tests_failed += 1;
        }
    }
    
    printf("%s: End ADPCM Library Tests\n", __FUNCTION__);
    printf("%s: Total Tests <%d>, Passed <%d>, Failed <%d>\n", __FUNCTION__, tests, tests_passed, tests_failed);

    // Destroy decoder
    if(decoder) {
        adpcm_decode_destroy(decoder);
    }
    return 0;
}

// This tests the adpcm_decode function if decoder is NULL
bool adpcm_decode_test_1(adpcm_dec_t *decoder) {
    adpcm_t inbuf[95];
    uint32_t  inlen  = 95;
    pcm_t   outbuf[1024];
    uint32_t  outlen = 1024;
    if(adpcm_decode(NULL, inbuf, inlen, outbuf, outlen, ADPCM_XVP_COMMAND_ID_MIN, ADPCM_XVP_COMMAND_ID_MAX, true) == -1) {
        return(true);
    }
    return(false);
}

// This tests the adpcm_decode function if inbuf is NULL
bool adpcm_decode_test_2(adpcm_dec_t *decoder) {
    uint32_t  inlen  = 95;
    pcm_t   outbuf[1024];
    uint32_t  outlen = 1024;
    if(adpcm_decode(decoder, NULL, inlen, outbuf, outlen, ADPCM_XVP_COMMAND_ID_MIN, ADPCM_XVP_COMMAND_ID_MAX, true) == -1) {
        return(true);
    }
    return(false);
}

// This tests the adpcm_decode function if outbuf is NULL
bool adpcm_decode_test_3(adpcm_dec_t *decoder) {
    adpcm_t inbuf[95];
    uint32_t  inlen  = 95;
    uint32_t  outlen = 1024;
    if(adpcm_decode(decoder, inbuf, inlen, NULL, outlen, ADPCM_XVP_COMMAND_ID_MIN, ADPCM_XVP_COMMAND_ID_MAX, true) == -1) {
        return(true);
    }
    return(false);
}

// This tests the adpcm_decode function if inbuf is not the minimum length
bool adpcm_decode_test_4(adpcm_dec_t *decoder) {
    adpcm_t inbuf[95];
    uint32_t  inlen  = 2; // Doesn't meet min length requirement
    pcm_t   outbuf[1024];
    uint32_t  outlen = 1024;
    if(adpcm_decode(decoder, inbuf, inlen, outbuf, outlen, ADPCM_XVP_COMMAND_ID_MIN, ADPCM_XVP_COMMAND_ID_MAX, true) == -1) {
        return(true);
    }
    return(false);
}

// This tests the adpcm_decode function if the command id in inbuf is below the minimum
bool adpcm_decode_test_5(adpcm_dec_t *decoder) {
    adpcm_t inbuf[95];
    uint32_t  inlen  = 95;
    pcm_t   outbuf[1024];
    uint32_t  outlen = 1024;

    inbuf[0] = 0x10; // Command ID is first byte
    if(adpcm_decode(decoder, inbuf, inlen, outbuf, outlen, ADPCM_XVP_COMMAND_ID_MIN, ADPCM_XVP_COMMAND_ID_MAX, true) == -1) {
        return(true);
    }
    return(false);
}

// This tests the adpcm_decode function if the command id in inbuf is above the maximum
bool adpcm_decode_test_6(adpcm_dec_t *decoder) {
    adpcm_t inbuf[95];
    uint32_t  inlen  = 95;
    pcm_t   outbuf[1024];
    uint32_t  outlen = 1024;

    inbuf[0] = 0x40; // Command ID is first byte
    if(adpcm_decode(decoder, inbuf, inlen, outbuf, outlen, ADPCM_XVP_COMMAND_ID_MIN, ADPCM_XVP_COMMAND_ID_MAX, true) == -1) {
        return(true);
    }
    return(false);
}

// This tests the adpcm_decode function if the outbuf is not long enough
bool adpcm_decode_test_7(adpcm_dec_t *decoder) {
    adpcm_t inbuf[95];
    uint32_t  inlen  = 95;
    pcm_t   outbuf[1024];
    uint32_t  outlen = 181; // One byte short

    inbuf[0] = 0x20; // Need a valid command id
    if(adpcm_decode(decoder, inbuf, inlen, outbuf, outlen, ADPCM_XVP_COMMAND_ID_MIN, ADPCM_XVP_COMMAND_ID_MAX, true) == -1) {
        return(true);
    }
    return(false);
}

// This tests the adpcm_decode_stats function if decoder is NULL
bool adpcm_decode_test_8(adpcm_dec_t *decoder) {
    adpcm_decode_stats_t stats;

    if(adpcm_decode_stats(NULL, &stats) == false) {
        return(true);
    }
    return(false);
}

// This tests the adpcm_decode_stats function if stats is NULL
bool adpcm_decode_test_9(adpcm_dec_t *decoder) {

    if(adpcm_decode_stats(decoder, NULL) == false) {
        return(true);
    }
    return(false);
}

// This tests the adpcm_decode_stats function to see if stats are acquired
bool adpcm_decode_test_10(adpcm_dec_t *decoder) {
    adpcm_decode_stats_t stats;

    if(adpcm_decode_stats(decoder, &stats) == true) {
        return(true);
    }
    return(false);
}

// This tests the adpcm_decode_stats function to see if failed decodes was accurate
bool adpcm_decode_test_11(adpcm_dec_t *decoder) {
    adpcm_decode_stats_t stats;

    if(adpcm_decode_stats(decoder, &stats) == false) {
        return(false);
    }
    // 7 Failed decodes happened since the decoder existed
    if(stats.failed_decodes == 6) {
        return(true);
    }
    return(false);
}

// This tests adpcm_decode when a step size mismatch on first packet
bool adpcm_decode_test_12(adpcm_dec_t *decoder) {
    adpcm_t inbuf[95];
    uint32_t  inlen  = 95;
    pcm_t   outbuf[1024];
    uint32_t  outlen = 1024;
    adpcm_decode_stats_t stats;

    inbuf[0] = 0x20; // Need a valid command id
    inbuf[1] = 0x01; // Step size SHOULD be 0 on first packet
    inbuf[2] = 0x00; // Predicted sample
    inbuf[3] = 0x00; // SHOULD be 0 on first packet
    if(adpcm_decode(decoder, inbuf, inlen, outbuf, outlen, ADPCM_XVP_COMMAND_ID_MIN, ADPCM_XVP_COMMAND_ID_MAX, true) == -1) {
        return(false);
    }
    if(adpcm_decode_stats(decoder, &stats) == false) {
        return(false);
    }
    if(stats.step_size_mismatch == 1) {
        return(true);
    }
    return(false);
}

// This tests adpcm_decode when a predicted sample mismatch on first packet
bool adpcm_decode_test_13(adpcm_dec_t *decoder) {
    adpcm_t inbuf[95];
    uint32_t  inlen  = 95;
    pcm_t   outbuf[1024];
    uint32_t  outlen = 1024;
    adpcm_decode_stats_t stats;

    inbuf[0] = 0x20; // Need a valid command id
    inbuf[1] = 0x00; // Step size SHOULD be 0 on first packet
    inbuf[2] = 0x01; // Predicted sample
    inbuf[3] = 0x01; // SHOULD be 0 on first packet
    if(adpcm_decode(decoder, inbuf, inlen, outbuf, outlen, ADPCM_XVP_COMMAND_ID_MIN, ADPCM_XVP_COMMAND_ID_MAX, true) == -1) {
        return(false);
    }
    if(adpcm_decode_stats(decoder, &stats) == false) {
        return(false);
    }
    if(stats.predicted_sample_mismatch == 1) {
        return(true);
    }
    return(false);
}

// This tests that adpcm_decode_reset when decoder is null
bool adpcm_decode_test_14(adpcm_dec_t *decoder) {
    if(adpcm_decode_reset(NULL) == false) {
        return(true);
    }
    return(false);
}

// This tests that adpcm_decode_reset resets the stats
bool adpcm_decode_test_15(adpcm_dec_t *decoder)
{
    adpcm_decode_stats_t stats;

    if (adpcm_decode_stats(decoder, &stats) == true) {
        if (stats.predicted_sample_mismatch != 0) {
            if (adpcm_decode_reset(decoder) == true) {
                if (adpcm_decode_stats(decoder, &stats) == true) {
                    if (stats.predicted_sample_mismatch == 0) {
                        return (true);
                    }
                }
            }
        }
    }
    return (false);
}

// This tests that adpcm_decode catches command id mismatches
bool adpcm_decode_test_16(adpcm_dec_t *decoder) {
    adpcm_t inbuf[95];
    uint32_t  inlen  = 95;
    pcm_t   outbuf[1024];
    uint32_t  outlen = 1024;
    adpcm_decode_stats_t stats;
    int32_t samples_decoded = 0;
    FILE   *fin = NULL;
    uint32_t  bytes_read = 0;

    if(adpcm_decode_reset(decoder) == false) {
        return(false);
    }

    if((fin = fopen("data/adpcm_decode_test_16.raw", "r")) == NULL) {
        printf("%s: failed to open data file\n", __FUNCTION__);
        return(false);
    }

    while((bytes_read = fread(inbuf, 1, inlen, fin)) == inlen) {
        if((samples_decoded = adpcm_decode(decoder, inbuf, inlen, &outbuf[samples_decoded], outlen-samples_decoded, ADPCM_XVP_COMMAND_ID_MIN, ADPCM_XVP_COMMAND_ID_MAX, true)) == -1) {
            printf("%s: failed to decode packet\n", __FUNCTION__);
            if(fin) {
                fclose(fin);
            }
            return(false);
        }
    }
    if(fin) {
        fclose(fin);
    }
    if(adpcm_decode_stats(decoder, &stats) == false) {
        return(false);
    }

    if(stats.commands_lost == 1) {
        return(true);
    }

    return(false);
}

// This tests that adpcm_decode catches command id mismatches (wrap around)
bool adpcm_decode_test_17(adpcm_dec_t *decoder) {
    adpcm_t inbuf[95];
    uint32_t  inlen  = 95;
    pcm_t   outbuf[1024];
    uint32_t  outlen = 1024;
    adpcm_decode_stats_t stats;
    int32_t samples_decoded = 0;
    FILE   *fin = NULL;
    uint32_t  bytes_read = 0;

    if(adpcm_decode_reset(decoder) == false) {
        return(false);
    }

    if((fin = fopen("data/adpcm_decode_test_17.raw", "r")) == NULL) {
        printf("%s: failed to open data file\n", __FUNCTION__);
        return(false);
    }

    while((bytes_read = fread(inbuf, 1, inlen, fin)) == inlen) {
        if((samples_decoded = adpcm_decode(decoder, inbuf, inlen, &outbuf[samples_decoded], outlen-samples_decoded, ADPCM_XVP_COMMAND_ID_MIN, ADPCM_XVP_COMMAND_ID_MAX, true)) == -1) {
            printf("%s: failed to decode packet\n", __FUNCTION__);
            if(fin) {
                fclose(fin);
            }
            return(false);
        }
    }
    if(fin) {
        fclose(fin);
    }
    if(adpcm_decode_stats(decoder, &stats) == false) {
        return(false);
    }

    if(stats.commands_lost == 3) {
        return(true);
    }

    return(false);
}

// This tests that adpcm_decode catches a step size mismatch on second packet
bool adpcm_decode_test_18(adpcm_dec_t *decoder) {
    adpcm_t inbuf[95];
    uint32_t  inlen  = 95;
    pcm_t   outbuf[1024];
    uint32_t  outlen = 1024;
    adpcm_decode_stats_t stats;
    int32_t samples_decoded = 0;
    FILE   *fin = NULL;
    uint32_t  bytes_read = 0;

    if(adpcm_decode_reset(decoder) == false) {
        return(false);
    }

    if((fin = fopen("data/adpcm_decode_test_18.raw", "r")) == NULL) {
        printf("%s: failed to open data file\n", __FUNCTION__);
        return(false);
    }

    while((bytes_read = fread(inbuf, 1, inlen, fin)) == inlen) {
        if((samples_decoded = adpcm_decode(decoder, inbuf, inlen, &outbuf[samples_decoded], outlen-samples_decoded, ADPCM_XVP_COMMAND_ID_MIN, ADPCM_XVP_COMMAND_ID_MAX, true)) == -1) {
            printf("%s: failed to decode packet\n", __FUNCTION__);
            if(fin) {
                fclose(fin);
            }
            return(false);
        }
    }
    if(fin) {
        fclose(fin);
    }
    if(adpcm_decode_stats(decoder, &stats) == false) {
        return(false);
    }

    if(stats.step_size_mismatch == 1) {
        return(true);
    }

    return(false);
}

// This tests that adpcm_decode catches a predicted sample mismatch on second packet
bool adpcm_decode_test_19(adpcm_dec_t *decoder) {
    adpcm_t inbuf[95];
    uint32_t  inlen  = 95;
    pcm_t   outbuf[1024];
    uint32_t  outlen = 1024;
    adpcm_decode_stats_t stats;
    int32_t samples_decoded = 0;
    FILE   *fin = NULL;
    uint32_t  bytes_read = 0;

    if(adpcm_decode_reset(decoder) == false) {
        return(false);
    }

    if((fin = fopen("data/adpcm_decode_test_19.raw", "r")) == NULL) {
        printf("%s: failed to open data file\n", __FUNCTION__);
        return(false);
    }

    while((bytes_read = fread(inbuf, 1, inlen, fin)) == inlen) {
        if((samples_decoded = adpcm_decode(decoder, inbuf, inlen, &outbuf[samples_decoded], outlen-samples_decoded, ADPCM_XVP_COMMAND_ID_MIN, ADPCM_XVP_COMMAND_ID_MAX, true)) == -1) {
            printf("%s: failed to decode packet\n", __FUNCTION__);
            if(fin) {
                fclose(fin);
            }
            return(false);
        }
    }
    if(fin) {
        fclose(fin);
    }
    if(adpcm_decode_stats(decoder, &stats) == false) {
        return(false);
    }

    if(stats.predicted_sample_mismatch == 1) {
        return(true);
    }

    return(false);
}

// This tests that adpcm_decode can decode a full utterance
bool adpcm_decode_test_20(adpcm_dec_t *decoder) {
    adpcm_t inbuf[95];
    uint32_t  inlen  = 95;
    pcm_t   outbuf[1024];
    uint32_t  outlen = 1024;
    adpcm_decode_stats_t stats;
    int32_t samples_decoded = 0;
    FILE   *fin = NULL;
    FILE   *fout = NULL;
    uint32_t  bytes_read = 0;
    uint32_t  bytes_write = 0;

    if(adpcm_decode_reset(decoder) == false) {
        return(false);
    }

    if((fin = fopen("data/adpcm_decode_test_20.raw", "r")) == NULL) {
        printf("%s: failed to open data file\n", __FUNCTION__);
        return(false);
    }

    if((fout = fopen("data/adpcm_decode_test_20.pcm", "w")) == NULL) {
        printf("%s: failed to open output file\n", __FUNCTION__);
        return(false);
    }

    while((bytes_read = fread(inbuf, 1, inlen, fin)) == inlen) {
        if((samples_decoded = adpcm_decode(decoder, inbuf, inlen, outbuf, outlen, ADPCM_XVP_COMMAND_ID_MIN, ADPCM_XVP_COMMAND_ID_MAX, true)) == -1) {
            printf("%s: failed to decode packet\n", __FUNCTION__);
            if(fin) {
                fclose(fin);
            }
            if(fout) {
                fclose(fout);
            }
            return(false);
        }
        if((bytes_write = fwrite(outbuf, sizeof(pcm_t), samples_decoded, fout)) != samples_decoded) {
            printf("%s: failed to write pcm to file\n", __FUNCTION__);
            if(fin) {
                fclose(fin);
            }
            if(fout) {
                fclose(fout);
            }
            return(false);
        }
    }

    if(fin) {
        fclose(fin);
    }
    if(fout) {
        fclose(fout);
    }

    if(adpcm_decode_stats(decoder, &stats) == false) {
        return(false);
    }

    if(stats.samples_decoded           == 29484 &&
       stats.commands_lost             == 3     &&
       stats.failed_decodes            == 0     &&
       stats.step_size_mismatch        == 3     &&
       stats.predicted_sample_mismatch == 5) {
        return(true);
    }

    return(false);
}

// This tests the adpcm_decode function if decoder magic_number is wrong
bool adpcm_decode_test_21(adpcm_dec_t *decoder) {
    adpcm_t inbuf[95];
    uint32_t  inlen  = 95;
    pcm_t   outbuf[1024];
    uint32_t  outlen = 1024;
    uint8_t  *p_decoder = (uint8_t *)decoder;

    p_decoder[0] = 0x01; // Change magic number

    if(adpcm_decode(decoder, inbuf, inlen, outbuf, outlen, ADPCM_XVP_COMMAND_ID_MIN, ADPCM_XVP_COMMAND_ID_MAX, true) == -1) {
        return(true);
    }
    return(false);
}

// This tests the adpcm_decode_stats function if decoder magic_number is wrong
bool adpcm_decode_test_22(adpcm_dec_t *decoder) {
    adpcm_decode_stats_t stats;
    uint8_t  *p_decoder = (uint8_t *)decoder;

    p_decoder[0] = 0x01; // Change magic number

    if(adpcm_decode_stats(decoder, &stats) == false) {
        return(true);
    }
    return(false);
}

// This tests the adpcm_decode_reset function if decoder magic_number is wrong
bool adpcm_decode_test_23(adpcm_dec_t *decoder) {
    uint8_t  *p_decoder = (uint8_t *)decoder;

    p_decoder[0] = 0x01; // Change magic number

    if(adpcm_decode_reset(decoder) == false) {
        return(true);
    }
    return(false);
}

