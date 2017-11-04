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
include(ExternalProject)

# CryptoPP Setup
set(cryptocpp_cmake_args
  -DCMAKE_BUILD_TYPE=Release
  -DBUILD_STATIC=True
  -DBUILD_SHARED=False
  -DBUILD_TESTING=False
  -DCMAKE_INSTALL_PREFIX=cryptopp
  )
ExternalProject_Add(cryptopp_project
  GIT_REPOSITORY https://github.com/weidai11/cryptopp.git
  GIT_TAG CRYPTOPP_5_6_4
  BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/cryptopp
  INSTALL_DIR cryptopp
  #TEST_BEFORE_INSTALL 1
  UPDATE_COMMAND "" # Don't need to update since using tagged release
  CMAKE_ARGS ${cryptocpp_cmake_args}
  BUILD_BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/cryptopp/libcryptopp.a
  )

add_library(cryptopp STATIC IMPORTED GLOBAL)
add_dependencies(cryptopp cryptopp_project)
ExternalProject_Get_Property(cryptopp_project source_dir binary_dir)
set_property(TARGET cryptopp PROPERTY IMPORTED_LOCATION ${binary_dir}/libcryptopp.a)

# Workaround for INTERFACE_INCLUDE_DIRECTORIES. The problem is the include
# directory needs to exist at cmake configuration time even though it won't
# exist until the external project is checked out at build time.
file(MAKE_DIRECTORY ${binary_dir}/cryptopp/include)

set_property(TARGET cryptopp PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${binary_dir}/cryptopp/include)


# Botan Setup
if (CMAKE_COMPILER_IS_GNUCXX)
  set(botan_cc
    --cc=gcc
    --cc-bin=${CMAKE_CXX_COMPILER}
    )
else()
  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(botan_cc
      --cc=clang
      --cc-bin=${CMAKE_CXX_COMPILER}
      )
  endif()
endif()

set(botan_args
  --minimized-build
  --enable-modules=blake2
  --with-boost
  --disable-shared
  --makefile-style=gmake
  --prefix=${CMAKE_CURRENT_BINARY_DIR}/botan
  )

ExternalProject_Add(botan_project
  GIT_REPOSITORY https://github.com/randombit/botan.git
  GIT_TAG 1.11.33
  BUILD_IN_SOURCE 1
  UPDATE_COMMAND "" # Don't need to update since using tagged release
  CONFIGURE_COMMAND python configure.py ${botan_cc} ${botan_args}
  BUILD_COMMAND make
  BUILD_BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/botan/lib/libbotan-1.11.a
  )

add_library(botan STATIC IMPORTED GLOBAL)
add_dependencies(botan botan_project)
set_property(TARGET botan PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/botan/lib/libbotan-1.11.a)

# Workaround for INTERFACE_INCLUDE_DIRECTORIES. The problem is the include
# directory needs to exist at cmake configuration time even though it won't
# exist until the external project is checked out at build time.
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/botan/include/botan-1.11)

set_property(TARGET botan PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR}/botan/include/botan-1.11)
