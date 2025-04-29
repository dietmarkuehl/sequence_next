// examples/hello-sequence_next.cpp                                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/sequence_next/sequence_next.hpp>
#include <iostream>

namespace ex = beman::execution;
namespace sn = beman::sequence_next;

// ----------------------------------------------------------------------------

int main() {
    std::cout << "hello, world\n";
    ex::sync_wait(sn::ignore_all(sn::iota{2, 5}));
    ex::sync_wait(sn::ignore_all(
        sn::then_each(
            sn::iota{2, 5},
            [](auto x){ std::cout << "sequence element=" << x << '\n'; }
        )));
}
