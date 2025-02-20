//
// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "zetasql/analyzer/all_rewriters.h"

#include <vector>

#include "zetasql/analyzer/anonymization_rewriter.h"
#include "zetasql/analyzer/rewriters/array_functions_rewriter.h"
#include "zetasql/analyzer/rewriters/builtin_function_inliner.h"
#include "zetasql/analyzer/rewriters/flatten_rewriter.h"
#include "zetasql/analyzer/rewriters/set_operation_corresponding_rewriter.h"
#include "zetasql/analyzer/rewriters/grouping_set_rewriter.h"
#include "zetasql/analyzer/rewriters/like_any_all_rewriter.h"
#include "zetasql/analyzer/rewriters/map_function_rewriter.h"
#include "zetasql/analyzer/rewriters/nulliferror_function_rewriter.h"
#include "zetasql/analyzer/rewriters/pivot_rewriter.h"
#include "zetasql/analyzer/rewriters/registration.h"
#include "zetasql/analyzer/rewriters/sql_function_inliner.h"
#include "zetasql/analyzer/rewriters/sql_view_inliner.h"
#include "zetasql/analyzer/rewriters/typeof_function_rewriter.h"
#include "zetasql/analyzer/rewriters/unpivot_rewriter.h"
#include "zetasql/analyzer/rewriters/with_expr_rewriter.h"
#include "zetasql/public/options.pb.h"
#include "absl/base/call_once.h"

namespace zetasql {

void RegisterBuiltinRewriters() {
  static absl::once_flag once_flag;
  absl::call_once(once_flag, [] {
    RewriteRegistry& r = RewriteRegistry::global_instance();

    // Rewriters are run in the order that they are registered. After all
    // rewriters have run, the resulting ResolvedAST is checked to see if more
    // rewrites are applicable. If there are, the rewriters are run again in the
    // order they are registered. The rewriters are applied until there is no
    // applicable rewrite.

    // Functioning inlining runs first so that other rewriters can apply to
    // the function bodies that are inserted by this rule.
    r.Register(ResolvedASTRewrite::REWRITE_INLINE_SQL_FUNCTIONS,
               GetSqlFunctionInliner());
    r.Register(ResolvedASTRewrite::REWRITE_INLINE_SQL_UDAS,
               GetSqlAggregateInliner());
    r.Register(ResolvedASTRewrite::REWRITE_INLINE_SQL_TVFS, GetSqlTvfInliner());
    r.Register(ResolvedASTRewrite::REWRITE_INLINE_SQL_VIEWS,
               GetSqlViewInliner());

    r.Register(ResolvedASTRewrite::REWRITE_FLATTEN, GetFlattenRewriter());
    r.Register(ResolvedASTRewrite::REWRITE_ANONYMIZATION,
               GetAnonymizationRewriter());
    r.Register(ResolvedASTRewrite::REWRITE_PROTO_MAP_FNS,
               GetMapFunctionRewriter());
    r.Register(ResolvedASTRewrite::REWRITE_ARRAY_FILTER_TRANSFORM,
               GetArrayFilterTransformRewriter());
    r.Register(ResolvedASTRewrite::REWRITE_UNPIVOT, GetUnpivotRewriter());
    r.Register(ResolvedASTRewrite::REWRITE_PIVOT, GetPivotRewriter());
    r.Register(ResolvedASTRewrite::REWRITE_ARRAY_INCLUDES,
               GetArrayIncludesRewriter());
    r.Register(ResolvedASTRewrite::REWRITE_TYPEOF_FUNCTION,
               GetTypeofFunctionRewriter());
    r.Register(ResolvedASTRewrite::REWRITE_NULLIFERROR_FUNCTION,
               GetNullIfErrorFunctionRewriter());
    r.Register(ResolvedASTRewrite::REWRITE_BUILTIN_FUNCTION_INLINER,
               GetBuiltinFunctionInliner());
    r.Register(ResolvedASTRewrite::REWRITE_LIKE_ANY_ALL,
               GetLikeAnyAllRewriter());

    r.Register(ResolvedASTRewrite::REWRITE_SET_OPERATION_CORRESPONDING,
               GetSetOperationCorrespondingRewriter());

    r.Register(ResolvedASTRewrite::REWRITE_GROUPING_SET,
               GetGroupingSetRewriter());

    // This rewriter should typically be the last in the rewrite sequence
    // because it cleans up after several other rewriters add ResolvedWithExprs.
    // Thus new rewriters should usually be added above this one.
    r.Register(ResolvedASTRewrite::REWRITE_WITH_EXPR, GetWithExprRewriter());
  });
}

}  // namespace zetasql
