#ifndef SORBET_DSL_MINITEST_H
#define SORBET_DSL_MINITEST_H
#include "ast/ast.h"

namespace sorbet::dsl {

/**
 * This class desugars things of the form
 *
 *    class MyTest
 *      describe 'foo' do
 *        it 'bar' do
 *          baz
 *        end
 *      end
 *    end
 *
 * into
 *
 *    class MyTest
 *      class <class_foo> < self
 *        sig {void}
 *        def <test_bar>
 *          baz
 *        end
 *      end
 *    end
 *
 * which is sort of a lie in that each `test_` method should actually running in
 * its own instance where `.name` retuns `test_0001_bar` but I think it is
 * close enough for our purposes.
 */
class Minitest final {
public:
    static std::vector<std::unique_ptr<ast::Expression>> replaceDSL(core::MutableContext ctx, ast::Send *send);

    Minitest() = delete;
};

} // namespace sorbet::dsl

#endif
