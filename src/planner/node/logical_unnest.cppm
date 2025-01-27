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

export module logical_unnest;

import stl;
import logical_node_type;
import column_binding;
import logical_node;
import base_expression;
import data_type;
import internal_types;
import base_table_ref;

namespace infinity {

export class LogicalUnnest : public LogicalNode {
public:
    explicit LogicalUnnest(u64 node_id, SharedPtr<BaseTableRef> base_table_ref, Vector<SharedPtr<BaseExpression>> expressions, u64 unnest_idx)
        : LogicalNode(node_id, LogicalNodeType::kUnnest), base_table_ref_(base_table_ref), expression_list_(std::move(expressions)), unnest_idx_(unnest_idx) {}

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalUnnest"; }

public:
    // getter
    Vector<SharedPtr<BaseExpression>> &expression_list() { return expression_list_; }
    const Vector<SharedPtr<BaseExpression>> &expression_list() const { return expression_list_; }
    u64 unnest_idx() const { return unnest_idx_; }

    SharedPtr<BaseTableRef> base_table_ref_{};

private:
    Vector<SharedPtr<BaseExpression>> expression_list_{};
    u64 unnest_idx_{};
};

} // namespace infinity
