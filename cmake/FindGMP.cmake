# -*- cmake -*-
# Copyright 2017 Will Mitchell
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

FIND_PATH(GMP_INCLUDE_DIR gmp.h)

FIND_LIBRARY(GMP_LIBRARY NAMES gmp)

IF (GMP_INCLUDE_DIR AND GMP_LIBRARY)
  SET(GMP_FOUND TRUE)
ENDIF (GMP_INCLUDE_DIR AND GMP_LIBRARY)


IF (GMP_FOUND)
  IF (NOT GMP_FIND_QUIETLY)
    MESSAGE(STATUS "Found GMP: ${GMP_LIBRARY}")
  ENDIF (NOT GMP_FIND_QUIETLY)
ELSE (GMP_FOUND)
  IF (GMP_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find GMP")
  ENDIF (GMP_FIND_REQUIRED)
ENDIF (GMP_FOUND)

