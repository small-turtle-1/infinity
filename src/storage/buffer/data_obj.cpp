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
import buffer_handle;
import buffer_obj;
import mmap_obj;
import infinity_exception;
import third_party;
import persistence_manager;
import infinity_context;

module data_obj;

namespace infinity {

SizeT DataObj::GetBufferSize() const {
    if (IsBuffer()) {
        return std::get<BufferObj *>(data_)->GetBufferSize();
    } else {
        return std::get<SharedPtr<DataObj>>(data_)->GetBufferSize();
    }
}

std::variant<BufferHandle, const char *> DataObj::Load() {
    if (IsBuffer()) {
        return std::get<BufferObj *>(data_)->Load();
    }
    return std::get<SharedPtr<MmapObj>>(data_)->ptr();
}

void DataObj::ToMmap() {
    if (!IsBuffer()) {
        UnrecoverableError(fmt::format("DataObj::ToMmap: data is not BufferObj"));
    }
    auto *buffer_obj = std::move(std::get<BufferObj *>(data_));
    buffer_obj->SubObjRc();

    PersistenceManager *pm = InfinityContext::instance().persistence_manager();
    if (pm == nullptr) {
        String file_path = buffer_obj->file_worker()->GetFilePath();
        SizeT buffer_size = buffer_obj->GetBufferSize();
        data_ = MakeShared<MmapObj>(file_path, 0, buffer_size);
    } else {
        ObjAddr obj_addr = buffer_obj->file_worker()->obj_addr_;
        String obj_path = pm->GetObjPath(obj_addr.obj_key_);
        data_ = MakeShared<MmapObj>(obj_path, obj_addr.part_offset_, obj_addr.part_size_);
    }
}

}
