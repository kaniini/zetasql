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

#include "zetasql/analyzer/analyzer_impl.h"

#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include <thread>

#include "zetasql/base/logging.h"
#include "zetasql/analyzer/analyzer_output_mutator.h"
#include "zetasql/analyzer/resolver.h"
#include "zetasql/analyzer/rewrite_resolved_ast.h"
#include "zetasql/common/errors.h"
#include "zetasql/common/internal_analyzer_options.h"
#include "zetasql/common/timer_util.h"
#include "zetasql/parser/parse_tree.h"
#include "zetasql/parser/parser.h"
#include "zetasql/public/analyzer_options.h"
#include "zetasql/public/analyzer_output.h"
#include "zetasql/public/catalog.h"
#include "zetasql/public/language_options.h"
#include "zetasql/public/parse_location.h"
#include "zetasql/public/types/type.h"
#include "zetasql/public/types/type_factory.h"
#include "zetasql/resolved_ast/resolved_ast.h"
#include "zetasql/resolved_ast/validator.h"
#include "absl/container/flat_hash_map.h"
#include "absl/flags/flag.h"
#include "absl/memory/memory.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "absl/types/span.h"
#include "zetasql/base/status_macros.h"

// This provides a way to extract and look at the zetasql resolved AST
// from within some other test or tool.  It prints to cout rather than logging
// because the output is often too big to log without truncating.
ABSL_FLAG(bool, zetasql_print_resolved_ast, false,
          "Print resolved AST to stdout after resolving (for debugging)");

namespace zetasql {

namespace {

absl::Status AnalyzeExpressionImpl(absl::string_view sql,
                                   const AnalyzerOptions& options_in,
                                   Catalog* catalog, TypeFactory* type_factory,
                                   const Type* target_type,
                                   std::unique_ptr<AnalyzerOutput>* output) {
  output->reset();
  internal::TimedValue overall_timed_value;
  {
    auto scoped_timer = internal::MakeScopedTimerStarted(&overall_timed_value);

    ZETASQL_VLOG(1) << "Parsing expression:\n" << sql;
    std::unique_ptr<AnalyzerOptions> copy;
    const AnalyzerOptions& options = GetOptionsWithArenas(&options_in, &copy);
    ZETASQL_RETURN_IF_ERROR(ValidateAnalyzerOptions(options));

    std::unique_ptr<ParserOutput> parser_output;
    ParserOptions parser_options = options.GetParserOptions();
    ZETASQL_RETURN_IF_ERROR(ParseExpression(sql, parser_options, &parser_output));
    const ASTExpression* expression = parser_output->expression();
    ZETASQL_VLOG(5) << "Parsed AST:\n" << expression->DebugString();

    ZETASQL_RETURN_IF_ERROR(InternalAnalyzeExpressionFromParserAST(
        *expression, std::move(parser_output), sql, options, catalog,
        type_factory, target_type, output));
  }
  AnalyzerOutputMutator(*output).overall_timed_value().Accumulate(
      overall_timed_value);
  return absl::OkStatus();
}

}  // namespace

absl::Status InternalAnalyzeExpression(
    absl::string_view sql, const AnalyzerOptions& options, Catalog* catalog,
    TypeFactory* type_factory, const Type* target_type,
    std::unique_ptr<AnalyzerOutput>* output) {
  return ConvertInternalErrorLocationAndAdjustErrorString(
      options.error_message_mode(), options.attach_error_location_payload(),
      sql,
      AnalyzeExpressionImpl(sql, options, catalog, type_factory, target_type,
                            output));
}

absl::Status ConvertExprToTargetType(
    const ASTExpression& ast_expression, absl::string_view sql,
    const AnalyzerOptions& analyzer_options, Catalog* catalog,
    TypeFactory* type_factory, const Type* target_type,
    std::unique_ptr<const ResolvedExpr>* resolved_expr) {
  Resolver resolver(catalog, type_factory, &analyzer_options);
  return ConvertInternalErrorLocationToExternal(
      resolver.CoerceExprToType(&ast_expression, target_type,
                                Resolver::kImplicitAssignment, resolved_expr),
      sql);
}

absl::Status InternalAnalyzeExpressionFromParserAST(
    const ASTExpression& ast_expression,
    std::unique_ptr<ParserOutput> parser_output, absl::string_view sql,
    const AnalyzerOptions& options, Catalog* catalog, TypeFactory* type_factory,
    const Type* target_type, std::unique_ptr<AnalyzerOutput>* output) {
  AnalyzerRuntimeInfo analyzer_runtime_info;
  if (parser_output != nullptr) {
    // Add in the parser output, we _assume_ this is semantically part of this
    // analyzer run, but this assumption may be incorrect,
    // see AnalyzerRuntimeInfo for more docs.
    analyzer_runtime_info.parser_runtime_info() = parser_output->runtime_info();
  }
  {
    auto overall_timer = internal::MakeScopedTimerStarted(
        &analyzer_runtime_info.overall_timed_value());

    std::unique_ptr<const ResolvedExpr> resolved_expr;
    Resolver resolver(catalog, type_factory, &options);
    {
      auto resolver_timer = internal::MakeScopedTimerStarted(
          &analyzer_runtime_info.resolver_timed_value());
      ZETASQL_RETURN_IF_ERROR(
          resolver.ResolveStandaloneExpr(sql, &ast_expression, &resolved_expr));
      ZETASQL_VLOG(3) << "Resolved AST:\n" << resolved_expr->DebugString();

      if (target_type != nullptr) {
        ZETASQL_RETURN_IF_ERROR(ConvertExprToTargetType(ast_expression, sql, options,
                                                catalog, type_factory,
                                                target_type, &resolved_expr));
      }
    }

    if (InternalAnalyzerOptions::GetValidateResolvedAST(options)) {
      internal::ScopedTimer scoped_validator_timer = MakeScopedTimerStarted(
          &analyzer_runtime_info.validator_timed_value());
      Validator validator(options.language());
      ZETASQL_RETURN_IF_ERROR(
          validator.ValidateStandaloneResolvedExpr(resolved_expr.get()));
    }

    if (absl::GetFlag(FLAGS_zetasql_print_resolved_ast)) {
      std::cout << "Resolved AST from thread "
                << std::this_thread::get_id()
                << ":" << std::endl
                << resolved_expr->DebugString() << std::endl;
    }

    if (options.language().error_on_deprecated_syntax() &&
        !resolver.deprecation_warnings().empty()) {
      return resolver.deprecation_warnings().front();
    }

    // Make sure we're starting from a clean state for CheckFieldsAccessed.
    resolved_expr->ClearFieldsAccessed();

    ZETASQL_ASSIGN_OR_RETURN(const QueryParametersMap& type_assignments,
                     resolver.AssignTypesToUndeclaredParameters());

    *output = std::make_unique<AnalyzerOutput>(
        options.id_string_pool(), options.arena(), std::move(resolved_expr),
        resolver.analyzer_output_properties(), std::move(parser_output),
        ConvertInternalErrorLocationsAndAdjustErrorStrings(
            options.error_message_mode(),
            options.attach_error_location_payload(), sql,
            resolver.deprecation_warnings()),
        type_assignments, resolver.undeclared_positional_parameters(),
        resolver.max_column_id());
    ZETASQL_RETURN_IF_ERROR(InternalRewriteResolvedAst(options, sql, catalog,
                                               type_factory, **output));
  }

  AnalyzerOutputMutator(*output).mutable_runtime_info().AccumulateAll(
      analyzer_runtime_info);
  return absl::OkStatus();
}
}  // namespace zetasql
