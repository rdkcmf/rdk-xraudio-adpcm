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
#include <sys/time.h>
#include <time.h>
#include "adpcm_internal.h"
#include "adpcm_version.h"

void adpcm_version(const char **name, const char **version, const char **branch, const char **commit_id) {
   if(name != NULL) {
      *name = "xraudio-adpcm";
   }
   if(version != NULL) {
      *version = ADPCM_VERSION;
   }
   if(branch != NULL) {
      *branch = ADPCM_BRANCH;
   }
   if(commit_id != NULL) {
      *commit_id = ADPCM_COMMIT_ID;
   }
}

#ifndef USE_RDKX_LOGGER
void adpcm_get_log_time(char *log_buffer) {
   struct tm *local;
   struct timeval tv;
   uint16_t msecs;
   gettimeofday(&tv, NULL);
   local = localtime(&tv.tv_sec);
   msecs = (uint16_t)(tv.tv_usec/1000);
   strftime(log_buffer, 9, "%T", local);
   //printing milliseconds as ":XXX "
   log_buffer[12] = '\0';                             //Null terminate milliseconds
   log_buffer[11] = (msecs % 10) + '0'; msecs  /= 10; //get the 1's digit
   log_buffer[10] = (msecs % 10) + '0'; msecs  /= 10; //get the 10's digit
   log_buffer[9]  = (msecs % 10) + '0';               //get the 100's digit
   log_buffer[8]  = ':';
}
#endif
