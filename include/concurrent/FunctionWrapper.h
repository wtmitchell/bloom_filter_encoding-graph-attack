//===-- concurrent/FunctionWrapper.h - A Function Wrapper ----*- C++ -*----===//
//
// Copyright 2017 Will Mitchell
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This file contains a function wrapper which packages a function into
/// a callable object. This is like std::function<void()>, but not movable
///
//===----------------------------------------------------------------------===//
#ifndef CONCURRENT_FUNCTIONWRAPPER_H_INCLUDED
#define CONCURRENT_FUNCTIONWRAPPER_H_INCLUDED

namespace concurrent {
class FunctionWrapper {
public:
  FunctionWrapper() = default;
  ~FunctionWrapper() = default;
  FunctionWrapper(const FunctionWrapper &) = delete;
  FunctionWrapper(FunctionWrapper &&other) : pimpl(std::move(other.pimpl)) {}
  FunctionWrapper &operator=(const FunctionWrapper &) = delete;
  FunctionWrapper &operator=(FunctionWrapper &&other) {
    pimpl = std::move(other.pimpl);
    return *this;
  }

  template <typename F>
  FunctionWrapper(F &&f)
      : pimpl(new PimplType<F>(std::move(f))) {}

  void operator()() { pimpl->call(); }

private:
  struct PimplBase {
    virtual void call() = 0;
    virtual ~PimplBase() {}
  };
  std::unique_ptr<PimplBase> pimpl;
  template <typename F> struct PimplType : PimplBase {
    F f;
    PimplType(F &&f_) : f(std::move(f_)) {}
    void call() { f(); }
  };
};
}

#endif
