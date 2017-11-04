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

set(TEST_LINK_COMPONENTS
  bfeattacks
  bloomfilter
  hash
  graph
  util
  )

set(bfeattacks_sources
  FilterRequireExactly.cpp
  FilterSize.cpp
  GraphFactory.cpp
  SingleRecord.cpp
  )

add_unittest(bfeattack_tests
  ${bfeattacks_sources}
  )
