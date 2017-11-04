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
find_package(Threads)

# Googletest
ExternalProject_Add(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.7.0
  BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/googletest
  INSTALL_COMMAND "" # Disable install step
  #TEST_BEFORE_INSTALL 1
  UPDATE_COMMAND "" # Don't need to update since using tagged release
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
  BUILD_BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/googletest/libgtest_main.a ${CMAKE_CURRENT_BINARY_DIR}/googletest/libgtest.a
  )

add_library(gtest_main STATIC IMPORTED GLOBAL)
add_dependencies(gtest_main googletest)
add_library(gtest STATIC IMPORTED GLOBAL)
add_dependencies(gtest googletest)

ExternalProject_Get_Property(googletest source_dir binary_dir)
set_property(TARGET gtest_main PROPERTY IMPORTED_LOCATION
  ${binary_dir}/libgtest_main.a)
set_property(TARGET gtest_main PROPERTY IMPORTED_LINK_INTERFACE
  ${CMAKE_THREAD_LIBS_INIT})
set_property(TARGET gtest_main PROPERTY INTERFACE_INCLUDE_DIRECTORIES
  ${source_dir}/include)

set_property(TARGET gtest PROPERTY IMPORTED_LOCATION
  ${binary_dir}/libgtest.a)
set_property(TARGET gtest PROPERTY IMPORTED_LINK_INTERFACE
  ${CMAKE_THREAD_LIBS_INIT})
set_property(TARGET gtest PROPERTY INTERFACE_INCLUDE_DIRECTORIES
  ${source_dir}/include)

# Workaround for INTERFACE_INCLUDE_DIRECTORIES. The problem is the include
# directory needs to exist at cmake configuration time even though it won't
# exist until the external project is checked out at build time.
file(MAKE_DIRECTORY ${source_dir}/include)


# Function to collect a libraries tests together into a distinct
# googletest executable per library
function(add_unittest test_name)
  add_executable(${test_name} ${ARGN})
  target_link_libraries(${test_name}
    gtest
    gtest_main
    ${CMAKE_THREAD_LIBS_INIT}
    ${TEST_LINK_COMPONENTS}
    )

  add_test(${test_name} ${CMAKE_BINARY_DIR}/${test_name})

  if (USE_VALGRIND_MEMCHECK)
    set(memcheck_command "${CTEST_MEMORYCHECK_COMMAND} ${CTEST_MEMORYCHECK_COMMAND_OPTIONS}")
    separate_arguments(memcheck_command)
    add_test(memcheck_${test_name} ${memcheck_command} ${CMAKE_BINARY_DIR}/${test_name})
  endif()
endfunction()

# googletest triggers some warnings via macro includes, suppress them
if("${CMAKE_C_COMPILER}" MATCHES "clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors -Wno-missing-variable-declarations")
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(adt)
add_subdirectory(bfeattacks)
add_subdirectory(bloomfilter)
add_subdirectory(common)
add_subdirectory(concurrent)
add_subdirectory(graph)
add_subdirectory(hash)
add_subdirectory(util)
