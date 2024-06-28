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

// struct HnswBenchmarkOption {

// };

#include <iostream>

import stl;
import third_party;
import hnsw_alg;
import vec_store_type;
import compilation_config;
import local_file_system;
import file_system;
import file_system_type;
import status;
import hnsw_common;
import infinity_exception;

using namespace infinity;

int main() {
    Path data_path = "./test/data/benchmark/gist_1m/gist_base.fvecs";
    Path query_path = "./test/data/benchmark/gist_1m/gist_query.fvecs";
    Path groundtruth_path = "./test/data/benchmark/gist_1m/gist_groundtruth.ivecs";

    Path index_save_path_ = Path(tmp_data_path()) / "hnsw_index.bin";

    using LabelT = i32;

    using Hnsw = KnnHnsw<PlainL2VecStoreType<float>, LabelT>;
    using CompressHnsw = KnnHnsw<LVQL2VecStoreType<float, int8_t>, LabelT>;

    SizeT chunk_size = 8192;
    SizeT max_chunk_size = 1024;
    SizeT M = 16;
    SizeT ef_construction = 200;

    SizeT thread_n = 16;
    LocalFileSystem fs;
    if (true) {
        auto [file_handler, status] = fs.OpenFile(data_path.string(), FileFlags::READ_FLAG, FileLockType::kReadLock);
        if (!status.ok()) {
            UnrecoverableError(status.message());
        }
        i32 dim = 0;
        fs.Read(*file_handler, &dim, sizeof(dim));

        SizeT file_size = fs.GetFileSize(*file_handler);
        SizeT vec_num = (file_size - sizeof(dim)) / (dim * sizeof(float));

        std::cout << fmt::format("dim: {}, vec_num: {}", dim, vec_num) << std::endl;

        auto data = MakeUnique<float[]>(dim * vec_num);
        fs.Read(*file_handler, data.get(), dim * vec_num * sizeof(float));

        auto hnsw = Hnsw::Make(chunk_size, max_chunk_size, dim, M, ef_construction);
        DenseVectorIter<float, LabelT> iter(data.get(), dim, vec_num);

        hnsw.StoreData(iter);
        Vector<std::thread> threads;
        Atomic<i32> cur_i = 0;
        for (SizeT i = 0; i < thread_n; ++i) {
            threads.emplace_back([&] {
                SizeT i;
                while ((i = cur_i.fetch_add(1)) < vec_num) {
                    hnsw.Build(i);
                }
            });
        }
        for (auto &thread : threads) {
            thread.join();
        }

        CompressHnsw compressed_hnsw = std::move(hnsw).CompressToLVQ();

        auto [index_file, index_status] =
            fs.OpenFile(index_save_path_.string(), FileFlags::WRITE_FLAG | FileFlags::CREATE_FLAG, FileLockType::kNoLock);
        if (!index_status.ok()) {
            UnrecoverableError(index_status.message());
        }
        compressed_hnsw.Save(*index_file);
    }
    {
        auto [index_file, index_status] = fs.OpenFile(index_save_path_.string(), FileFlags::READ_FLAG, FileLockType::kNoLock);
        if (!index_status.ok()) {
            UnrecoverableError(index_status.message());
        }
        auto hnsw = CompressHnsw::Load(*index_file);

        auto [query_file, query_status] = fs.OpenFile(query_path.string(), FileFlags::READ_FLAG, FileLockType::kReadLock);
        if (!query_status.ok()) {
            UnrecoverableError(query_status.message());
        }
        i32 dim;
        fs.Read(*query_file, &dim, sizeof(dim));

        SizeT file_size = fs.GetFileSize(*query_file);
        SizeT query_num = (file_size - sizeof(dim)) / (dim * sizeof(float));
        auto query_data = MakeUnique<float[]>(dim * query_num);
        fs.Read(*query_file, query_data.get(), dim * query_num * sizeof(float));

        Vector<HashSet<LabelT>> results(query_num);
        for (SizeT i = 0; i < query_num; ++i) {
            const float *query = query_data.get() + i * dim;
            auto [result_n, dists, labels] = hnsw.Search(query, 100);
            for (SizeT j = 0; j < result_n; ++j) {
                results[i].insert(labels[j]);
            }
        }

        auto [groundtruth_file, groundtruth_status] =
            fs.OpenFile(groundtruth_path.string(), FileFlags::READ_FLAG, FileLockType::kReadLock);
        if (!groundtruth_status.ok()) {
            UnrecoverableError(groundtruth_status.message());
        }

        i32 top_k;
    }
}