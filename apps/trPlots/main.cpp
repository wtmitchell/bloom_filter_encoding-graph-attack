/*
 * Copyright 2017 Will Mitchell
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include <iostream>
using std::cout;
using std::endl;

#include <thread>

#include "concurrent/ThreadPool.h"


int main(int argc, const char *argv[]) {

  //concurrent::LockingQueue<int> test;
  concurrent::ThreadPoolSimple test;

  cout << "Will use " << std::thread::hardware_concurrency() << " threads in pool" << endl;

  return 0;
}
