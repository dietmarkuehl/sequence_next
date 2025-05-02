// tests/beman/sequence_next/sequence_next.test.cpp                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/sequence_next/sequence_next.hpp>
#include <gtest/gtest.h>

namespace sn = beman::sequence_next;
namespace ex = beman::execution;

// ----------------------------------------------------------------------------

TEST(SequenceNextTest, FilterEach) {
    std::vector<int> results;
    ex::sync_wait(sn::iota{2, 7} | sn::then_each([](auto x) { return x * 3; }) | sn::filter_each([](auto x) {
                      static_assert(std::same_as<int, decltype(x)>);
                      return x % 2;
                  }) |
                  sn::then_each([&](auto x) { results.push_back(x); }) | sn::ignore_all);
    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0], 2);
    EXPECT_EQ(results[1], 7);
}
