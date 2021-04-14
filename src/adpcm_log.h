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
#ifndef __ADPCM_LOG_H__
#define __ADPCM_LOG_H__

#ifdef USE_RDKX_LOGGER    // If RDKX logger is available
#define XLOG_OMIT_FUNCTION
#include "rdkx_logger.h"
#else                     // Use normal logging

#define LOG_PREFIX         "LIBADPCM: "         ///< Prefix for all INFO and DEBUG logs
#define LOG_PREFIX_FATAL   "LIBADPCM:FATAL: "   ///< Prefix for FATAL logs
#define LOG_PREFIX_ERROR   "LIBADPCM:ERROR: "   ///< Prefix for ERROR logs
#define LOG_PREFIX_WARN    "LIBADPCM:WARN: "    ///< Prefix for WARN logs

#ifdef RDK_LOGGER_ENABLED
   #include "rdk_debug.h"
   #include "iarmUtil.h"
   extern int b_rdk_logger_enabled;
   #define LOG(...) if(b_rdk_logger_enabled) {\
                       RDK_LOG(RDK_LOG_DEBUG, "LOG.RDK.LIBADPCM", __VA_ARGS__);\
                    } else {\
                       char log_buffer[13]; adpcm_get_log_time(log_buffer); printf("%s " FORMAT, log_buffer, ##__VA_ARGS__); \
                    }
   #define LOG_FLUSH() fflush(stdout)
#else
   #define LOG(FORMAT, ...); {char log_buffer[13]; adpcm_get_log_time(log_buffer); printf("%s " FORMAT "\n", log_buffer, ##__VA_ARGS__);}
   #define LOG_FLUSH() fflush(stdout)
#endif
#define LOG_LEVEL_NONE  (6)
#define LOG_LEVEL_FATAL (5)
#define LOG_LEVEL_ERROR (4)
#define LOG_LEVEL_WARN  (3)
#define LOG_LEVEL_INFO  (2)
#define LOG_LEVEL_DEBUG (1)
#define LOG_LEVEL_ALL   (0)
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_INFO
#endif
#if (LOG_LEVEL >= LOG_LEVEL_NONE)
   #define XLOGD_FATAL(...)
   #define XLOGD_ERROR(...)
   #define XLOGD_WARN(...)
   #define XLOGD_INFO(...)
   #define XLOGD_DEBUG(...)
#elif (LOG_LEVEL >= LOG_LEVEL_FATAL)
   #define XLOGD_FATAL(...) LOG(LOG_PREFIX_FATAL __VA_ARGS__); LOG_FLUSH()
   #define XLOGD_ERROR(...)
   #define XLOGD_WARN(...)
   #define XLOGD_INFO(...)
   #define XLOGD_DEBUG(...)
#elif (LOG_LEVEL >= LOG_LEVEL_ERROR)
   #define XLOGD_FATAL(...) LOG(LOG_PREFIX_FATAL __VA_ARGS__); LOG_FLUSH()
   #define XLOGD_ERROR(...) LOG(LOG_PREFIX_ERROR __VA_ARGS__)
   #define XLOGD_WARN(...)  LOG(LOG_PREFIX_WARN  __VA_ARGS__)
   #define XLOGD_INFO(...)
   #define XLOGD_DEBUG(...)
#elif (LOG_LEVEL >= LOG_LEVEL_WARN)
   #define XLOGD_FATAL(...) LOG(LOG_PREFIX_FATAL __VA_ARGS__); LOG_FLUSH()
   #define XLOGD_ERROR(...) LOG(LOG_PREFIX_ERROR __VA_ARGS__)
   #define XLOGD_WARN(...)  LOG(LOG_PREFIX_WARN  __VA_ARGS__)
   #define XLOGD_INFO(...)
   #define XLOGD_DEBUG(...)
#elif (LOG_LEVEL >= LOG_LEVEL_INFO)
   #define XLOGD_FATAL(...) LOG(LOG_PREFIX_FATAL __VA_ARGS__); LOG_FLUSH()
   #define XLOGD_ERROR(...) LOG(LOG_PREFIX_ERROR __VA_ARGS__)
   #define XLOGD_WARN(...)  LOG(LOG_PREFIX_WARN  __VA_ARGS__)
   #define XLOGD_INFO(...)  LOG(LOG_PREFIX       __VA_ARGS__)
   #define XLOGD_DEBUG(...)
#else
   #define XLOGD_FATAL(...) LOG(LOG_PREFIX_FATAL __VA_ARGS__); LOG_FLUSH()
   #define XLOGD_ERROR(...) LOG(LOG_PREFIX_ERROR __VA_ARGS__)
   #define XLOGD_WARN(...)  LOG(LOG_PREFIX_WARN  __VA_ARGS__)
   #define XLOGD_INFO(...)  LOG(LOG_PREFIX       __VA_ARGS__)
   #define XLOGD_DEBUG(...) LOG(LOG_PREFIX       __VA_ARGS__)
#endif
#ifdef __cplusplus
extern "C"
{
#endif
void adpcm_get_log_time(char *log_buffer);
#ifdef __cplusplus
}
#endif
#endif

#endif
