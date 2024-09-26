// Copyright(C) 2024 InfiniFlow, Inc. All rights reserved.
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

module object_file;

import status;
import virtual_storage_system;

namespace infinity {

ObjectFile::ObjectFile(VirtualStorageSystem *storage_system, StorageType type)
    : AbstractFileHandle(storage_system, type) {}

ObjectFile::~ObjectFile() = default;

Status ObjectFile::Open(const String &path, FileAccessMode access_mode) { return Status::OK(); }

Status ObjectFile::Close() { return Status::OK(); }

Status ObjectFile::Append(const char *buffer) { return Status::OK(); }

Status ObjectFile::Append(const String &buffer) { return Status::OK(); }

Tuple<SizeT, Status> ObjectFile::Read(char *buffer) { return {0, Status::OK()}; }

Tuple<SizeT, Status> ObjectFile::Read(String &buffer) { return {0, Status::OK()}; }

SizeT ObjectFile::FileSize() { return 0; }

Tuple<char *, SizeT, Status> ObjectFile::MmapRead(const String &name) { return {nullptr, 0, Status::OK()}; }

Status ObjectFile::Unmmap(const String &name) { return Status::OK(); }

} // namespace infinity