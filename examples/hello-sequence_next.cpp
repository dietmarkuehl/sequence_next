// examples/hello-sequence_next.cpp                                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/sequence_next/sequence_next.hpp>
#include <iostream>

namespace ex = beman::execution;
namespace sn = beman::sequence_next;

// ----------------------------------------------------------------------------

int main() {
    ex::sync_wait(
          sn::iota{2, 5}
        //| something(resume_after(100ms))
        | sn::then_each([](auto x){
            std::cout << "element1=" << x << '\n';
            return x * 2;
            })
        | sn::then_each([](auto x){
            std::cout << "element2=" << x << '\n';
            })
        | sn::ignore_all);

    for (int i{}; i != 5; ++i) {
        ex::sync_wait(sn::conditional_element(
            ex::just(i, 2),
            ex::then([](auto x, auto y){
                std::cout << "condition hold: (" << x << ", " << y << ")\n";
            }),
            [](auto x, auto y){ return x < y; }
            )
            | ex::then([](auto&&...){ std::cout << "complete\n"; })
        );
    }
}
