// Copyright(C) 2023 InfiniFlow, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

module;

import stl;

export module mmap_obj;

namespace infinity {

class FDObj {
public:
    explicit FDObj(const String &filepath);

    FDObj(const FDObj &) = delete;
    FDObj(FDObj &&other) noexcept : fd_(std::exchange(other.fd_, -1)) {}

    ~FDObj();

    void reset();

    int fd() const { return fd_; }

private:
    int fd_ = -1;
};

export class MmapObj {
public:
    MmapObj(const String &filepath, SizeT offset, SizeT size);

    MmapObj(const MmapObj &) = delete;
    MmapObj(MmapObj &&other) noexcept : fd_(std::move(other.fd_)), ptr_(std::exchange(other.ptr_, nullptr)), size_(other.size_) {}

    ~MmapObj();

    void reset();

    const char *ptr() const { return reinterpret_cast<const char *>(ptr_); }
    SizeT size() const { return size_; }

private:
    FDObj fd_;
    void *ptr_ = nullptr;
    SizeT size_;
};

} // namespace infinity