// tests/beman/sequence_next/state_helper.test.cpp                    -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/sequence_next/detail/state_helper.hpp>
#include <gtest/gtest.h>

namespace sn = beman::sequence_next;
namespace ex = beman::execution;

namespace {
enum class result : char { none, set_value, set_error, set_stopped };

struct receiver {
    result& res;
    using receiver_concept = ex::receiver_t;
    auto set_value(auto&&...) && noexcept { this->res = result::set_value; }
    auto set_error(auto&&) && noexcept { this->res = result::set_error; }
    auto set_stopped() && noexcept { this->res = result::set_stopped; }
};
static_assert(ex::receiver<receiver>);
} // namespace

TEST(SequenceNextTest, state_helper) {
    // Given: a sender with a known completion kind
    // When: creating a state for this sender a receiver
    //       recordng the disposition
    // Then: starting the state results in the correctly
    //       recorded disposition
    {
        auto                     res{result::none};
        sn::detail::state_helper state(ex::just(1, 2), receiver{res});
        ex::start(state);
        EXPECT_EQ(res, result::set_value);
    }
    {
        auto                     res{result::none};
        sn::detail::state_helper state(ex::just_error(1), receiver{res});
        ex::start(state);
        EXPECT_EQ(res, result::set_error);
    }
    {
        auto                     res{result::none};
        sn::detail::state_helper state(ex::just_stopped(), receiver{res});
        ex::start(state);
        EXPECT_EQ(res, result::set_stopped);
    }
}
