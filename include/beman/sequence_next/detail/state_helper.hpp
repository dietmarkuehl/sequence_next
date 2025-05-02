// include/beman/sequence_next/detail/state_helper.hpp                -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_INCLUDE_BEMAN_SEQUENCE_NEXT_DETAIL_STATE_HELPER
#define INCLUDED_INCLUDE_BEMAN_SEQUENCE_NEXT_DETAIL_STATE_HELPER

#include <beman/execution/execution.hpp>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::sequence_next::detail {
template <::beman::execution::sender S, ::beman::execution::receiver R>
struct state_helper {
    using operation_state_concept = ::beman::execution::operation_state_t;
    using state_t                 = decltype(::beman::execution::connect(::std::declval<S>(), ::std::declval<R>()));

    state_t state;

    state_helper(S&& s, R&& r) : state(::beman::execution::connect(::std::forward<S>(s), ::std::forward<R>(r))) {}
    ~state_helper() {}

    auto start() & noexcept { ::beman::execution::start(this->state); }
};

} // namespace beman::sequence_next::detail

// ----------------------------------------------------------------------------

#endif
