#include "ast/ast.h"
#include "ast/desugar/Desugar.h"
#include "common/common.h"
#include "core/ErrorQueue.h"
#include "core/Unfreeze.h"
#include "parser/parser.h"
#include "spdlog/spdlog.h"
#include "gtest/gtest.h"
#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace spd = spdlog;
auto logger = spd::stderr_color_mt("desugar_test");
auto errorQueue = std::make_shared<sorbet::core::ErrorQueue>(*logger, *logger);

TEST(DesugarTest, SimpleDesugar) { // NOLINT
    sorbet::core::GlobalState gs(errorQueue);
    gs.initEmpty();
    sorbet::core::UnfreezeNameTable nameTableAccess(gs);
    sorbet::core::UnfreezeFileTable ft(gs);

    auto ast = sorbet::parser::Parser::run(gs, "<test>", "def hello_world; p :hello; end");
    sorbet::core::MutableContext ctx(gs, sorbet::core::Symbols::root());
    auto o1 = sorbet::ast::desugar::node2Tree(ctx, move(ast));
}
