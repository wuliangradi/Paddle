//   Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "paddle/fluid/recordio/scanner.h"
#include "paddle/fluid/platform/enforce.h"

namespace paddle {
namespace recordio {
Scanner::Scanner(std::unique_ptr<std::istream> &&stream)
    : stream_(std::move(stream)) {
  Reset();
}

Scanner::Scanner(const std::string &filename) {
  stream_.reset(new std::ifstream(filename));
  Reset();
}

void Scanner::Reset() {
  stream_->seekg(0, std::ios::beg);
  ParseNextChunk();
}

const std::string &Scanner::Next() {
  PADDLE_ENFORCE(!eof_, "StopIteration");
  auto &rec = cur_chunk_.Record(offset_++);
  if (offset_ == cur_chunk_.NumRecords()) {
    ParseNextChunk();
  }
  return rec;
}

void Scanner::ParseNextChunk() {
  eof_ = !cur_chunk_.Parse(*stream_);
  offset_ = 0;
}

bool Scanner::HasNext() const { return !eof_; }
}  // namespace recordio
}  // namespace paddle
