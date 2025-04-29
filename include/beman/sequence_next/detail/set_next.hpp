// include/beman/sequence_next/detail/set_next.hpp                    -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_INCLUDE_BEMAN_SEQUENCE_NEXT_DETAIL_SET_NEXT
#define INCLUDED_INCLUDE_BEMAN_SEQUENCE_NEXT_DETAIL_SET_NEXT

#include <beman/execution/execution.hpp>

// ----------------------------------------------------------------------------

namespace beman::sequence_next {

inline constexpr struct set_next_t {
    template <::beman::execution::receiver Receiver, ::beman::execution::sender Sender>
    auto operator()(Receiver& rcvr, Sender sndr) const {
        return rcvr.set_next(sndr);
    }
} set_next;

}

// ----------------------------------------------------------------------------

#endif
