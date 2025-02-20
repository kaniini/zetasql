#
# Copyright 2019 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

load(
    "//zetasql/compliance:builddefs.bzl",
    "zetasql_compliance_test",
)

package(default_visibility = ["//zetasql/base:zetasql_implementation"])

exports_files(["reference_impl_known_errors.textproto"])

cc_library(
    name = "type_helpers",
    srcs = ["type_helpers.cc"],
    hdrs = ["type_helpers.h"],
    deps = [
        "//zetasql/base:ret_check",
        "//zetasql/base:status",
        "//zetasql/public:strings",
        "//zetasql/public:type",
        "//zetasql/resolved_ast",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
    name = "type_helpers_test",
    size = "small",
    srcs = ["type_helpers_test.cc"],
    deps = [
        ":type_helpers",
        "//zetasql/base/testing:status_matchers",
        "//zetasql/base/testing:zetasql_gtest_main",
        "//zetasql/testing:test_value",
    ],
)

# Tested by value_expr_test.
cc_library(
    name = "proto_util",
    srcs = ["proto_util.cc"],
    hdrs = ["proto_util.h"],
    deps = [
        "//zetasql/base",
        "//zetasql/base:ret_check",
        "//zetasql/base:source_location",
        "//zetasql/base:status",
        "//zetasql/common:errors",
        "//zetasql/common:internal_value",
        "//zetasql/public:type",
        "//zetasql/public:type_cc_proto",
        "//zetasql/public:value",
        "//zetasql/public/functions:arithmetics",
        "//zetasql/public/functions:date_time_util",
        "//zetasql/public/proto:type_annotation_cc_proto",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:cord",
        "@com_google_protobuf//:protobuf",
        "@com_google_protobuf//:protobuf_lite",
    ],
)

cc_library(
    name = "statement_evaluator",
    srcs = ["statement_evaluator.cc"],
    hdrs = ["statement_evaluator.h"],
    deps = [
        ":evaluator_table_iterator_cc_proto",
        ":type_helpers",
        ":type_parameter_constraints",
        "//zetasql/analyzer:resolver",
        "//zetasql/base:ret_check",
        "//zetasql/base:status",
        "//zetasql/common:status_payload_utils",
        "//zetasql/parser",
        "//zetasql/public:analyzer",
        "//zetasql/public:coercer",
        "//zetasql/public:error_helpers",
        "//zetasql/public:evaluator",
        "//zetasql/public:evaluator_table_iterator",
        "//zetasql/public:language_options",
        "//zetasql/public:multi_catalog",
        "//zetasql/public:options_cc_proto",
        "//zetasql/public:parse_location",
        "//zetasql/public:parse_resume_location",
        "//zetasql/public:simple_catalog",
        "//zetasql/public:type",
        "//zetasql/resolved_ast",
        "//zetasql/resolved_ast:resolved_node_kind_cc_proto",
        "//zetasql/scripting:error_helpers",
        "//zetasql/scripting:parsed_script",
        "//zetasql/scripting:script_exception_cc_proto",
        "//zetasql/scripting:script_executor",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/types:optional",
    ],
)

cc_library(
    name = "common",
    srcs = [
        "common.cc",
        "variable_id.cc",
    ],
    hdrs = [
        "common.h",
        "variable_id.h",
    ],
    deps = [
        "//zetasql/base",
        "//zetasql/base:ret_check",
        "//zetasql/base:status",
        "//zetasql/public:collator_lite",
        "//zetasql/public:type",
        "//zetasql/public:type_cc_proto",
        "//zetasql/public:value",
        "//zetasql/resolved_ast",
        "@com_google_absl//absl/hash",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "evaluation",
    srcs = [
        "aggregate_op.cc",
        "analytic_op.cc",
        "evaluation.cc",
        "function.cc",
        "operator.cc",
        "relational_op.cc",
        "tuple.cc",
        "tuple_comparator.cc",
        "value_expr.cc",
    ],
    hdrs = [
        "evaluation.h",
        "function.h",
        "operator.h",
        "tuple.h",
        "tuple_comparator.h",
    ],
    deps = [
        ":common",
        ":proto_util",
        ":type_parameter_constraints",
        ":variable_generator",
        "//zetasql/base",
        "@com_google_protobuf//:protobuf",
        "@com_google_protobuf//:cc_wkt_protos",
        "@com_google_googleapis//google/type:date_cc_proto",
        "@com_google_googleapis//google/type:timeofday_cc_proto",
        # buildcleaner: keep
        "//zetasql/common:thread_stack",
        "//zetasql/public/functions:differential_privacy_cc_proto",
        "//zetasql/public:anonymization_utils",
        "//zetasql/common:errors",
        "//zetasql/common:initialize_required_fields",
        "//zetasql/common:internal_value",
        "//zetasql/proto:anon_output_with_report_cc_proto",
        "//zetasql/public/types",
        "//zetasql/public:catalog",
        "//zetasql/public:civil_time",
        "//zetasql/public:coercer",
        "//zetasql/public:collator_lite",
        "//zetasql/public:evaluator_table_iterator",
        "//zetasql/public:function",
        "//zetasql/public:json_value",
        "//zetasql/public:language_options",
        "//zetasql/public:numeric_value",
        "//zetasql/public:options_cc_proto",
        "//zetasql/public:proto_value_conversion",
        "//zetasql/public:type",
        "//zetasql/public:type_cc_proto",
        "//zetasql/public:value",
        "//zetasql/public/functions:arithmetics",
        "//zetasql/public/functions:bitcast",
        "//zetasql/public/functions:bitwise",
        "//zetasql/public/functions:common_proto",
        "//zetasql/public/functions:numeric",
        "//zetasql/public/functions:comparison",
        "//zetasql/public/functions:date_time_util",
        "//zetasql/public/functions:datetime_cc_proto",
        "//zetasql/public/functions:string_format",
        "//zetasql/public/functions:generate_array",
        "//zetasql/public/functions:json",
        "//zetasql/public/functions:like",
        "//zetasql/public/functions:math",
        "//zetasql/public/functions:net",
        "//zetasql/public/functions:normalize_mode_cc_proto",
        "//zetasql/public/functions:parse_date_time",
        "//zetasql/public/functions:percentile",
        "//zetasql/public/functions:regexp",
        "//zetasql/public/functions:string",
        "//zetasql/public/proto:type_annotation_cc_proto",
        "//zetasql/resolved_ast",
        "//zetasql/resolved_ast:resolved_ast_enums_cc_proto",
        "//zetasql/resolved_ast:resolved_node_kind_cc_proto",
        "@com_google_absl//absl/algorithm:container",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/cleanup",
        "@com_google_absl//absl/container:btree",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/container:flat_hash_set",
        "@com_google_absl//absl/container:node_hash_map",
        "@com_google_absl//absl/container:node_hash_set",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/hash",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/random",
        "@com_google_absl//absl/random:distributions",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:cord",
        "@com_google_absl//absl/synchronization",
        "@com_google_absl//absl/time",
        "@com_google_absl//absl/types:optional",
        "//zetasql/base:source_location",
        "@com_google_absl//absl/types:span",
        "@com_google_absl//absl/types:variant",
        "@com_google_differential_privacy//proto:confidence_interval_cc_proto",
        "@com_google_differential_privacy//proto:data_cc_proto",
        "@com_google_cc_differential_privacy//algorithms:algorithm",
        "@com_google_cc_differential_privacy//algorithms:bounded-sum",
        "@com_google_cc_differential_privacy//algorithms:bounded-mean",
        "@com_google_cc_differential_privacy//algorithms:bounded-standard-deviation",
        "@com_google_cc_differential_privacy//algorithms:bounded-variance",
        "@com_google_cc_differential_privacy//algorithms:quantiles",
        "//zetasql/base:flat_set",
        "//zetasql/base:map_util",
        "//zetasql/base:stl_util",
        "//zetasql/base:exactfloat",
        "@com_googlesource_code_re2//:re2",
        "//zetasql/base:status",
        "//zetasql/base:ret_check",
        "//zetasql/base:clock",
    ],
)

cc_test(
    name = "function_evaluation_test",
    srcs = ["function_test.cc"],
    deps = [
        ":evaluation",
        "//zetasql/base/testing:status_matchers",
        "//zetasql/base/testing:zetasql_gtest_main",
        "//zetasql/common:evaluator_registration_utils",
        "//zetasql/public:interval_value",
        "//zetasql/public/types",
    ],
)

cc_library(
    name = "tuple_test_util",
    testonly = 1,
    hdrs = ["tuple_test_util.h"],
    deps = [
        ":evaluation",
        "//zetasql/base:ret_check",
        "//zetasql/base:status",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_googletest//:gtest",
    ],
)

cc_library(
    name = "algebrizer",
    srcs = [
        "algebrizer.cc",
    ],
    hdrs = [
        "algebrizer.h",
        "function.h",
        "operator.h",
        "tuple.h",
    ],
    deps = [
        ":common",
        ":evaluation",
        ":parameters",
        ":proto_util",
        ":type_helpers",
        ":variable_generator",
        "//zetasql/analyzer:resolver",
        "//zetasql/base",
        "//zetasql/base:flat_set",
        "//zetasql/base:map_util",
        "//zetasql/base:ret_check",
        "//zetasql/base:source_location",
        "//zetasql/base:status",
        "//zetasql/base:stl_util",
        "//zetasql/common:aggregate_null_handling",
        "//zetasql/common:internal_value",
        "//zetasql/common:thread_stack",
        "//zetasql/public:anonymization_utils",
        "//zetasql/public:builtin_function_cc_proto",
        "//zetasql/public:catalog",
        "//zetasql/public:coercer",
        "//zetasql/public:evaluator_table_iterator",
        "//zetasql/public:function",
        "//zetasql/public:id_string",
        "//zetasql/public:language_options",
        "//zetasql/public:simple_catalog",
        "//zetasql/public:type",
        "//zetasql/public:value",
        "//zetasql/public/functions:date_time_util",
        "//zetasql/public/functions:json",
        "//zetasql/public/proto:type_annotation_cc_proto",
        "//zetasql/public/types",
        "//zetasql/resolved_ast",
        "//zetasql/resolved_ast:resolved_ast_enums_cc_proto",
        "//zetasql/resolved_ast:resolved_node_kind_cc_proto",
        "//zetasql/resolved_ast:serialization_cc_proto",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/cleanup",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/container:flat_hash_set",
        "@com_google_absl//absl/container:node_hash_map",
        "@com_google_absl//absl/functional:bind_front",
        "@com_google_absl//absl/hash",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:str_format",
        "@com_google_absl//absl/synchronization",
        "@com_google_absl//absl/types:optional",
        "@com_google_absl//absl/types:span",
        "@com_google_absl//absl/types:variant",
        "@com_google_googletest//:gtest",
        "@com_google_protobuf//:protobuf",
        "@com_googlesource_code_re2//:re2",
    ],
)

cc_library(
    name = "parameters",
    hdrs = ["parameters.h"],
    deps = [
        ":common",
        "@com_google_absl//absl/types:variant",
    ],
)

cc_library(
    name = "type_parameter_constraints",
    srcs = ["type_parameter_constraints.cc"],
    hdrs = ["type_parameter_constraints.h"],
    deps = [
        "//zetasql/public:value",
        "//zetasql/public/functions:string",
        "//zetasql/public/types",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "variable_generator",
    srcs = ["variable_generator.cc"],
    hdrs = ["variable_generator.h"],
    deps = [
        ":common",
        ":parameters",
        "//zetasql/base:source_location",
        "//zetasql/base:status",
        "//zetasql/public:analyzer",
        "//zetasql/resolved_ast",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:variant",
    ],
)

cc_library(
    name = "reference_driver",
    testonly = 1,
    srcs = ["reference_driver.cc"],
    hdrs = ["reference_driver.h"],
    deps = [
        ":algebrizer",
        ":common",
        ":evaluation",
        ":parameters",
        ":rewrite_flags",
        ":statement_evaluator",
        ":type_helpers",
        "//zetasql/analyzer:resolver",
        "//zetasql/base",
        "//zetasql/base:check",
        "//zetasql/base:ret_check",
        "//zetasql/base:status",
        "//zetasql/common:evaluator_registration_utils",
        "//zetasql/common:status_payload_utils",
        "//zetasql/compliance:test_database_catalog",
        "//zetasql/compliance:test_driver",
        "//zetasql/public:analyzer",
        "//zetasql/public:analyzer_options",
        "//zetasql/public:analyzer_output",
        "//zetasql/public:language_options",
        "//zetasql/public:multi_catalog",
        "//zetasql/public:options_cc_proto",
        "//zetasql/public:parse_location",
        "//zetasql/public:simple_catalog",
        "//zetasql/public:simple_catalog_util",
        "//zetasql/public:sql_function",
        "//zetasql/public:type",
        "//zetasql/public:value",
        "//zetasql/public/annotation:collation",
        "//zetasql/public/functions:date_time_util",
        "//zetasql/public/types",
        "//zetasql/resolved_ast",
        "//zetasql/resolved_ast:resolved_ast_enums_cc_proto",
        "//zetasql/resolved_ast:resolved_node_kind_cc_proto",
        "//zetasql/scripting:error_helpers",
        "//zetasql/scripting:script_executor",
        "//zetasql/scripting:type_aliases",
        "//zetasql/testing:test_value",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
        "@com_google_absl//absl/types:span",
        "@com_google_protobuf//:protobuf",
    ],
)

# Link with this target for compliance tests against the reference
# implementation.  This returns a ReferenceDriver with no specific
# LanguageOptions set (because each test will specify the options to use
# for that test).
cc_library(
    name = "use_reference_driver_for_compliance_test",
    testonly = 1,
    srcs = ["use_reference_driver_for_compliance_test.cc"],
    deps = [
        ":reference_driver",
        "//zetasql/compliance:test_driver",
    ],
)

zetasql_compliance_test(
    name = "reference_compliance_test",
    size = "large",
    args = [
        "--zetasql_compliance_print_array_orderedness",
    ],
    known_error_files = [
        ":reference_impl_known_errors.textproto",
        "//zetasql/compliance/known_errors:reference_impl_known_errors.textproto",
        "//zetasql/compliance/known_errors:zetasql_known_errors.textproto",
    ],
    shard_count = 30,
    deps = [
        ":reference_driver",
        # buildcleaner: keep
        ":use_reference_driver_for_compliance_test",
    ],
)

zetasql_compliance_test(
    name = "reference_compliance_test_minimal_rewrites",
    size = "large",
    args = [
        "--rewrites=minimal",
        "--zetasql_compliance_print_array_orderedness",
    ],
    known_error_files = [
        ":reference_impl_known_errors.textproto",
        "//zetasql/compliance/known_errors:reference_impl_known_errors.textproto",
        "//zetasql/compliance/known_errors:zetasql_known_errors.textproto",
    ],
    shard_count = 30,
    deps = [
        ":reference_driver",
        # buildcleaner: keep
        ":use_reference_driver_for_compliance_test",
    ],
)

zetasql_compliance_test(
    name = "reference_compliance_test_all_rewrites",
    size = "large",
    args = [
        "--rewrites=all",
        "--zetasql_compliance_print_array_orderedness",
        # Four files contain tests that do not work pass all rewrites enabled.
        # Do not add to this list. Write compliance tests that work with all
        # rewriters enabled.
        # TODO: Remove these files
        "--query_name_pattern='^(?!(pivot_test|unpivot_test|anonymization_test|anonymization_errors_test)).*$$'",
    ],
    known_error_files = [
        ":reference_impl_known_errors.textproto",
        "//zetasql/compliance/known_errors:reference_impl_known_errors.textproto",
        "//zetasql/compliance/known_errors:zetasql_known_errors.textproto",
    ],
    shard_count = 30,
    deps = [
        ":reference_driver",
        # buildcleaner: keep
        ":use_reference_driver_for_compliance_test",
    ],
)

# TODO: These should be nested in (not-yet) stripping above.

cc_test(
    name = "tuple_test",
    size = "small",
    srcs = ["tuple_test.cc"],
    deps = [
        ":evaluation",
        ":tuple_test_util",
        "//zetasql/base/testing:status_matchers",
        "//zetasql/base/testing:zetasql_gtest_main",
        "//zetasql/public:value",
        "//zetasql/public/types",
        "//zetasql/testdata:test_schema_cc_proto",
        "//zetasql/testing:test_value",
        "@com_google_absl//absl/status",
        "@com_google_protobuf//:protobuf",
    ],
)

cc_library(
    name = "test_relational_op",
    testonly = 1,
    hdrs = ["test_relational_op.h"],
    deps = [
        ":evaluation",
        ":tuple_test_util",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
    ],
)

cc_test(
    name = "aggregate_op_test",
    size = "small",
    srcs = ["aggregate_op_test.cc"],
    deps = [
        ":common",
        ":evaluation",
        ":test_relational_op",
        ":tuple_test_util",
        "//zetasql/base",
        "//zetasql/base:status",
        "//zetasql/base/testing:status_matchers",
        "//zetasql/base/testing:zetasql_gtest_main",
        "//zetasql/public:numeric_value",
        "//zetasql/public:type",
        "//zetasql/public:value",
        "//zetasql/testdata:test_schema_cc_proto",
        "//zetasql/testing:test_value",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
        "@com_google_absl//absl/types:span",
        "@com_google_protobuf//:protobuf_lite",
    ],
)

cc_test(
    name = "analytic_op_test",
    size = "small",
    srcs = ["analytic_op_test.cc"],
    shard_count = 5,
    deps = [
        ":common",
        ":evaluation",
        ":test_relational_op",
        ":tuple_test_util",
        "//zetasql/base",
        "//zetasql/base:source_location",
        "//zetasql/base:status",
        "//zetasql/base:stl_util",
        "//zetasql/base/testing:status_matchers",
        "//zetasql/base/testing:zetasql_gtest_main",
        "//zetasql/public:type",
        "//zetasql/public:type_cc_proto",
        "//zetasql/public:value",
        "//zetasql/resolved_ast",
        "//zetasql/testing:test_value",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/container:btree",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
        "@com_google_absl//absl/types:span",
        "@com_google_protobuf//:protobuf_lite",
    ],
)

cc_test(
    name = "relational_op_test",
    size = "large",
    srcs = ["relational_op_test.cc"],
    deps = [
        ":common",
        ":evaluation",
        ":test_relational_op",
        ":tuple_test_util",
        "//zetasql/base",
        "//zetasql/base:clock",
        "//zetasql/base:source_location",
        "//zetasql/base:status",
        "//zetasql/base/testing:status_matchers",
        "//zetasql/base/testing:zetasql_gtest_main",
        "//zetasql/common:evaluator_test_table",
        "//zetasql/common:thread_stack",
        "//zetasql/common/testing:testing_proto_util",
        "//zetasql/public:evaluator_table_iterator",
        "//zetasql/public:language_options",
        "//zetasql/public:simple_catalog",
        "//zetasql/public:type",
        "//zetasql/public:value",
        "//zetasql/resolved_ast",
        "//zetasql/testdata:test_schema_cc_proto",
        "//zetasql/testing:test_value",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:cord",
        "@com_google_absl//absl/time",
        "@com_google_absl//absl/types:optional",
        "@com_google_protobuf//:protobuf_lite",
    ],
)

cc_test(
    name = "value_expr_test",
    size = "small",
    srcs = ["value_expr_test.cc"],
    shard_count = 5,
    deps = [
        ":common",
        ":evaluation",
        ":test_relational_op",
        ":tuple_test_util",
        ":type_helpers",
        ":variable_generator",
        "//zetasql/base",
        "//zetasql/base:ret_check",
        "//zetasql/base:status",
        "//zetasql/base/testing:status_matchers",
        "//zetasql/base/testing:zetasql_gtest_main",
        "//zetasql/common:internal_value",
        "//zetasql/common:status_payload_utils",
        "//zetasql/common/testing:testing_proto_util",
        "//zetasql/compliance:functions_testlib",
        "//zetasql/public:builtin_function",
        "//zetasql/public:function",
        "//zetasql/public:language_options",
        "//zetasql/public:options_cc_proto",
        "//zetasql/public:simple_catalog",
        "//zetasql/public:type",
        "//zetasql/public:type_cc_proto",
        "//zetasql/public:value",
        "//zetasql/resolved_ast",
        "//zetasql/resolved_ast:resolved_node_kind_cc_proto",
        "//zetasql/testdata:test_schema_cc_proto",
        "//zetasql/testing:test_function",
        "//zetasql/testing:test_value",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:cord",
        "@com_google_absl//absl/types:optional",
        "@com_google_absl//absl/types:span",
        "@com_google_protobuf//:protobuf",
        "@com_google_protobuf//:protobuf_lite",
    ],
)

cc_test(
    name = "algebrizer_test",
    size = "small",
    srcs = ["algebrizer_test.cc"],
    deps = [
        ":algebrizer",
        "//zetasql/base",
        "//zetasql/base:map_util",
        "//zetasql/base:status",
        "//zetasql/base/testing:status_matchers",
        "//zetasql/base/testing:zetasql_gtest_main",
        "//zetasql/common:evaluator_test_table",
        "//zetasql/public:analyzer",
        "//zetasql/public:builtin_function",
        "//zetasql/public:builtin_function_cc_proto",
        "//zetasql/public:civil_time",
        "//zetasql/public:function",
        "//zetasql/public:function_cc_proto",
        "//zetasql/public:options_cc_proto",
        "//zetasql/public:simple_catalog",
        "//zetasql/public:type",
        "//zetasql/public:value",
        "//zetasql/resolved_ast",
        "//zetasql/resolved_ast:make_node_vector",
        "//zetasql/resolved_ast:resolved_node_kind_cc_proto",
        "//zetasql/testdata:sample_catalog",
        "//zetasql/testing:test_value",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
    name = "parameters_test",
    size = "small",
    srcs = ["parameters_test.cc"],
    deps = [
        ":parameters",
        "//zetasql/base/testing:zetasql_gtest_main",
        "@com_google_absl//absl/memory",
    ],
)

cc_test(
    name = "type_parameter_constraints_test",
    size = "small",
    srcs = ["type_parameter_constraints_test.cc"],
    deps = [
        ":type_parameter_constraints",
        "//zetasql/base/testing:status_matchers",
        "//zetasql/base/testing:zetasql_gtest_main",
    ],
)

cc_test(
    name = "variable_generator_test",
    size = "small",
    srcs = ["variable_generator_test.cc"],
    deps = [
        ":parameters",
        ":variable_generator",
        "//zetasql/base/testing:status_matchers",
        "//zetasql/base/testing:zetasql_gtest_main",
        "//zetasql/public:type",
        "//zetasql/testing:test_value",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "rewrite_flags",
    srcs = ["rewrite_flags.cc"],
    hdrs = ["rewrite_flags.h"],
    deps = [
        "//zetasql/common:options_utils",
        "//zetasql/public:analyzer_options",
        "//zetasql/public:options_cc_proto",
        "@com_google_absl//absl/container:btree",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/strings",
    ],
)

proto_library(
    name = "evaluator_table_iterator_proto",
    srcs = ["evaluator_table_iterator.proto"],
)

cc_proto_library(
    name = "evaluator_table_iterator_cc_proto",
    deps = [":evaluator_table_iterator_proto"],
)
