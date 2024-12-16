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

#include <cstdint>
#include <cstring>
#include <errno.h>
#include <exception>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

module mmap_obj;

import stl;
import infinity_exception;
import logger;
import third_party;

namespace infinity {

FDObj::FDObj(const String &filepath) {
    int fd = open(filepath.c_str(), O_RDONLY);
    if (fd < 0) {
        String err_msg = fmt::format("Fail to open file: {}, error: {}", filepath, std::strerror(errno));
        UnrecoverableError(err_msg);
    }
    fd_ = fd;
}

FDObj::~FDObj() {
    if (fd_ < 0) {
        return;
    }
    int ret = close(fd_);
    if (ret < 0) {
        String err_msg = fmt::format("Fail to close file descriptor: {}, error: {}", fd_, std::strerror(errno));
        LOG_CRITICAL(err_msg);
        std::terminate();
    }
    fd_ = -1;
}

void FDObj::reset() {
    if (fd_ < 0) {
        return;
    }
    int ret = close(fd_);
    if (ret < 0) {
        String err_msg = fmt::format("Fail to close file descriptor: {}, error: {}", fd_, std::strerror(errno));
        UnrecoverableError(err_msg);
    }
    fd_ = -1;
}

MmapObj::MmapObj(const String &filepath, SizeT offset, SizeT size) : fd_(filepath) {
    int fd = fd_.fd();
    void *ptr = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, offset);
    if (ptr == MAP_FAILED) {
        String err_msg = fmt::format("Mmap of file {}, offset {}, size: {} fail, error: {}", filepath, offset, size, std::strerror(errno));
        UnrecoverableError(err_msg);
    }
    ptr_ = ptr;
    LOG_INFO(fmt::format("Mmap file: {}, offset: {}, size: {}, ptr: {}", filepath, offset, size, reinterpret_cast<std::uintptr_t>(ptr_)));
}

MmapObj::~MmapObj() {
    if (ptr_ == nullptr) {
        return;
    }
    int ret = munmap(ptr_, size_);
    if (ret < 0) {
        String err_msg = fmt::format("Unmmap failed at {}", reinterpret_cast<std::uintptr_t>(ptr_));
        LOG_CRITICAL(err_msg);
        std::terminate();
    }
    ptr_ = nullptr;
}

void MmapObj::reset() {
    if (ptr_ == nullptr) {
        return;
    }
    int ret = munmap(ptr_, size_);
    if (ret < 0) {
        String err_msg = fmt::format("Unmmap failed at {}", reinterpret_cast<std::uintptr_t>(ptr_));
        UnrecoverableError(err_msg);
    }
    ptr_ = nullptr;
}

} // namespace infinity
