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
import buffer_obj;
import buffer_handle;
import mmap_obj;

export module data_obj;

namespace infinity {

export class DataObj {
public:
    DataObj() = default;
    DataObj(BufferObj *buffer_obj) : data_(buffer_obj) {}
    DataObj(SharedPtr<MmapObj> mmap_obj) : data_(std::move(mmap_obj)) {}

    SizeT GetBufferSize() const;
    std::variant<BufferHandle, const char *> Load();

    void ToMmap();

    bool IsBuffer() const { return std::holds_alternative<BufferObj *>(data_); }
    bool IsMmap() const { return std::holds_alternative<SharedPtr<MmapObj>>(data_); }

    BufferObj *buffer_obj() const { return std::get<BufferObj *>(data_); };
    SharedPtr<MmapObj> mmap_obj() const { return std::get<SharedPtr<MmapObj>>(data_); }

private:
    std::variant<BufferObj *, SharedPtr<MmapObj>, std::monostate> data_ = std::monostate{};
};

} // namespace infinity
