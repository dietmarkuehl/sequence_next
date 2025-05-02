// tests/beman/sequence_next/sequence_next.test.cpp                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/sequence_next/sequence_next.hpp>
#include <gtest/gtest.h>

namespace sn = beman::sequence_next;
namespace ex = beman::execution;

// ----------------------------------------------------------------------------

TEST(SequenceNextTest, sequence_next) {
    static_assert(std::same_as<decltype(sn::filter_each), const sn::filter_each_t>);
    static_assert(std::same_as<decltype(sn::ignore_all), const sn::ignore_all_t>);
    static_assert(std::same_as<decltype(sn::set_next), const sn::set_next_t>);
    static_assert(std::same_as<decltype(sn::then_each), const sn::then_each_t>);
}
