// include/beman/sequence_next/detail/conditional_element.hpp                        -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_INCLUDE_BEMAN_SEQUENCE_NEXT_DETAIL_CONDITONAL_ELEMENT
#define INCLUDED_INCLUDE_BEMAN_SEQUENCE_NEXT_DETAIL_CONDITONAL_ELEMENT

#include <beman/execution/execution.hpp>
#include <beman/sequence_next/detail/set_next.hpp>

// ----------------------------------------------------------------------------

namespace beman::sequence_next::detail {
struct conditional_element_t {


    template <::beman::execution::sender Sender_up, ::beman::execution::sender Sender_down, typename Pred>
    struct sender {
        using sender_concept = ::beman::execution::sender_t;
        using completion_signatures = ::beman::execution::completion_signatures<
            ::beman::execution::set_value_t()
        >;

        template <::beman::execution::receiver Receiver>
        struct state : state_base<Receiver> {
            using operation_state_concept = ::beman::execution::operation_state_t;
            //using inner_t = ;

            template <typename R, typename U, typename D, typename P>
            state(R&& rcvr, U&& u, D&& d, P&&p)
            {
            }

            void start() & noexcept {}
        };

        template <::beman::execution::receiver Receiver>
        auto connect(Receiver&& rcvr) && {
            return state<std::remove_cvref_t<Receiver>>(std::forward<Receiver>(this->rcvr),
                                                        std::move(this->sender_up),
                                                        std::move(this->sender_down),
                                                        std::move(this->pred));
        }

        Sender_up   sender_up;
        Sender_down sender_down;
        Pred        pred;
    };

    template <::beman::execution::sender Sender_up, ::beman::execution::sender Sender_down, typename Pred>
    sender<::std::remove_cvref_t<Sender_up>, ::std::remove_cvref_t<Sender_down>, ::std::remove_cvref_t<Pred>>
    operator()(Sender_up&& sndr_up, Sender_down&& sndr_down, Pred&& pred) const {
        return {
            ::std::forward<Sender_up>(sndr_up), ::std::forward<Sender_down>(sndr_down), ::std::forward<Pred>(pred)};
    }
};
} // namespace beman::sequence_next::detail

namespace beman::sequence_next {
using conditional_element_t = ::beman::sequence_next::detail::conditional_element_t;
inline constexpr conditional_element_t conditional_element{};
} // namespace beman::sequence_next

#endif
