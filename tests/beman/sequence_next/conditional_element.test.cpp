// tests/beman/sequence_next/conditional_element.test.cpp             -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/sequence_next/detail/conditional_element.hpp>
#include <beman/execution/execution.hpp>
#include <gtest/gtest.h>

namespace sn = beman::sequence_next;
namespace ex = beman::execution;

// ----------------------------------------------------------------------------

namespace {
struct receiver {
    using receiver_concept = ex::receiver_t;

    auto set_value(auto&&...) && noexcept {}
    auto set_error(auto&&) && noexcept {}
    auto set_stopped() && noexcept {}
};

auto test_condition(int value, bool expected) {
    bool                     called{false};
    sn::detail::state_helper state(sn::conditional_element(
                                       ex::just(value),
                                       [&called](auto s) {
                                           return ex::then(::std::move(s), [&called]<typename T>([[maybe_unused]] T x) {
                                               static_assert(std::same_as<int, T>);
                                               assert(x == 1);
                                               called = true;
                                           });
                                       },
                                       [](auto x) { return x == 1; }),
                                   receiver{});
    assert(called == false);
    ex::start(state);
    assert(called == expected);
}
} // namespace

TEST(SequenceNextTest, conditional_element) {
    test_condition(0, false);
    test_condition(1, true);
    test_condition(2, false);
}
