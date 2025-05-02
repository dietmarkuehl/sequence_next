// examples/hello-sequence_next.cpp                                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/sequence_next/sequence_next.hpp>
#include <iostream>

namespace ex = beman::execution;
namespace sn = beman::sequence_next;

// ----------------------------------------------------------------------------

int main() {
    ex::sync_wait(
          sn::iota{2, 7}
        | sn::then_each([](auto x) { return x * 3; })
        | sn::filter_each([](auto x){
            static_assert(std::same_as<int, decltype(x)>);
            return x % 2;
          })
        | sn::then_each([](auto x) {
            std::cout << "element2=" << x << '\n';
          })
        | sn::ignore_all);
}
