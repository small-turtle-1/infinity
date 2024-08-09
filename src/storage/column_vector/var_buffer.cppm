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

export module var_buffer;

import stl;
import buffer_obj;
import buffer_handle;

namespace infinity {

class BlockColumnEntry;
class BufferManager;

export class VarBuffer {
    friend class VarFileWorker;

public:
    VarBuffer() = default;

    VarBuffer(BufferObj *buffer_obj) : buffer_size_prefix_sum_({0}), buffer_obj_(buffer_obj) {}

    // this is called by VarFileWorker
    VarBuffer(BufferObj *buffer_obj, UniquePtr<char[]> buffer, SizeT size) : buffer_size_prefix_sum_({0, size}), buffer_obj_(buffer_obj) {
        buffers_.push_back(std::move(buffer));
    }

public:
    SizeT Append(UniquePtr<char[]> buffer, SizeT size, bool *free_success = nullptr);

    SizeT Append(const char *data, SizeT size, bool *free_success = nullptr);

    const char *Get(SizeT offset, SizeT size) const;

    void Write(char *&ptr) const;

    void Write(char *&ptr, SizeT offset, SizeT size) const;

    SizeT TotalSize() const { return buffer_size_prefix_sum_.back(); }

private:
    Vector<UniquePtr<char[]>> buffers_;
    Vector<SizeT> buffer_size_prefix_sum_ = {0};

    BufferObj *buffer_obj_ = nullptr;
};

export class VarBufferManager {
public:
    VarBufferManager() : buffer_(nullptr) {}

    VarBufferManager(BlockColumnEntry *block_column_entry = nullptr, BufferManager *buffer_mgr = nullptr)
        : buffer_(None), block_column_entry_(block_column_entry), buffer_mgr_(buffer_mgr) {}

    SizeT Append(UniquePtr<char[]> buffer, SizeT size, bool *free_success = nullptr);

    SizeT Append(const char *data, SizeT size, bool *free_success = nullptr);

    const char *Get(SizeT offset, SizeT size) const { return GetInner()->Get(offset, size); }

    void Write(char *&ptr) const { GetInner()->Write(ptr); }

    void Write(char *&ptr, SizeT offset, SizeT size) const { GetInner()->Write(ptr, offset, size); }

    SizeT TotalSize() const { return GetInner()->TotalSize(); }

private:
    VarBuffer *GetInner();

    const VarBuffer *GetInner() const;

private:
    std::variant<UniquePtr<VarBuffer>, Optional<BufferHandle>> buffer_;
    BlockColumnEntry *block_column_entry_ = nullptr;
    BufferManager *buffer_mgr_ = nullptr;
};

} // namespace infinity