// tests/beman/sequence_next/filter_each.test.cpp                     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/sequence_next/sequence_next.hpp>
#include <gtest/gtest.h>

namespace sn = beman::sequence_next;
namespace ex = beman::execution;

// ----------------------------------------------------------------------------

TEST(SequenceNextTest, filter_each) {
    std::vector<int> results;
    // clang-format off
    ex::sync_wait(
        sn::iota{2, 7}
        | sn::then_each([](auto x) { return x * 3; })
        | sn::filter_each([](auto x) { return x % 2; })
        | sn::then_each([&](auto x) { results.push_back(x); })
        | sn::ignore_all
        );
    // clang-format on
    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0], 9);
    EXPECT_EQ(results[1], 15);
}
