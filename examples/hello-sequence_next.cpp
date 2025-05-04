// examples/hello-sequence_next.cpp                                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/sequence_next/sequence_next.hpp>
#include <iostream>

namespace ex = beman::execution;
namespace sn = beman::sequence_next;

// ----------------------------------------------------------------------------

int main() {
    std::cout << std::unitbuf;
    // clang-format off
    ex::sync_wait(sn::iota{1, 7}
    | sn::filter_each([](auto x) { return x % 2; })
    | sn::then_each([](auto&&...){ std::cout << "next value\n"; })
    | sn::ignore_all
    );
    // clang-format on
}
