# Copyright(C) 2024 InfiniFlow, Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import warnings
from infinity_embedded.common import InfinityException
from infinity_embedded.errors import ErrorCode


def binary_exp_to_paser_exp(binary_expr_key) -> str:
    if binary_expr_key == "eq":
        return "="
    elif binary_expr_key == "gt":
        return ">"
    elif binary_expr_key == "lt":
        return "<"
    elif binary_expr_key == "gte":
        return ">="
    elif binary_expr_key == "lte":
        return "<="
    elif binary_expr_key == "neq":
        return "!="
    elif binary_expr_key == "and":
        return "and"
    elif binary_expr_key == "or":
        return "or"
    elif binary_expr_key == "add":
        return "+"
    elif binary_expr_key == "sub":
        return "-"
    elif binary_expr_key == "mul":
        return "*"
    elif binary_expr_key == "div":
        return "/"
    elif binary_expr_key == "mod":
        return "%"
    elif binary_expr_key == "trunc":
        return binary_expr_key
    else:
        raise InfinityException(ErrorCode.INVALID_EXPRESSION, f"unknown binary expression: {binary_expr_key}")


def deprecated_api(message):
    warnings.warn(message, DeprecationWarning, stacklevel=2)
