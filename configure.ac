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
AC_INIT([adpcm], [1.0], [Chris_Buchter@cable.comcast.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_PROG_AR
LT_INIT

AC_PROG_CC
AC_PROG_CXX

AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 test/Makefile
])

AC_ARG_ENABLE([rdkxlogger],
[  --enable-rdkxlogger    Turn on RDKX logger support],
[case "${enableval}" in
  yes) rdkxlogger=true ;;
  no)  rdkxlogger=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-rdkxlogger]) ;;
esac],[rdkxlogger=false])
AM_CONDITIONAL([RDKX_LOGGER_ENABLED], [test x$rdkxlogger = xtrue])

AC_ARG_VAR(GIT_BRANCH, git branch name)

AC_OUTPUT
