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

add_library(util
  ByteVector.cpp
  Hexadecimal.cpp
  String.cpp
  Timer.cpp
  )

if (GMP_FOUND)
  include_directories(${GMP_INCLUDE_DIR})
  set_source_files_properties(ByteVector.cpp COMPILE_FLAGS "-DUSE_GMP")
  target_link_libraries(util ${GMP_LIBRARY})
endif()
