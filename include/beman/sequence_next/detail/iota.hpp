// include/beman/sequence_next/detail/iota.hpp                        -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_INCLUDE_BEMAN_SEQUENCE_NEXT_DETAIL_IOTA
#define INCLUDED_INCLUDE_BEMAN_SEQUENCE_NEXT_DETAIL_IOTA

#include <beman/sequence_next/detail/set_next.hpp>
#include <beman/sequence_next/detail/state_helper.hpp>
#include <beman/execution/execution.hpp>
#include <memory>

// ----------------------------------------------------------------------------

namespace beman::sequence_next {

struct iota {
    using sender_concept        = ::beman::execution::sender_t;
    using completion_signatures = ::beman::execution::completion_signatures<::beman::execution::set_value_t()>;

    int from{};
    int to{};

    template <::beman::execution::receiver Receiver>
    struct state {
        using operation_state_concept = ::beman::execution::operation_state_t;
        Receiver rcvr;
        int      next;
        int      end;

        void start() & noexcept { this->produce_next(); }

        struct set_next_receiver {
            using receiver_concept = ::beman::execution::receiver_t;
            state* s;
            void   set_value() noexcept {
                state* st{this->s};
                st->helper.reset();
                st->produce_next();
            }
            void set_error(auto&&) noexcept { /*-dk:TODO */ }
        };

        using set_next_sender = decltype(set_next(std::declval<Receiver&>(), ::beman::execution::just(0)));
        using set_next_state =
            decltype(::beman::execution::connect(std::declval<set_next_sender>(), std::declval<set_next_receiver>()));
        using helper_t = ::beman::sequence_next::detail::state_helper<set_next_sender, set_next_receiver>;
        std::optional<helper_t> helper;
        void                    produce_next() {
            if (this->next != this->end) {
                this->helper.emplace(set_next(this->rcvr, ::beman::execution::just(this->next++)),
                                     set_next_receiver{this});
                ::beman::execution::start(*this->helper);
            } else {
                ::beman::execution::set_value(std::move(this->rcvr));
            }
        }
    };

    template <::beman::execution::receiver Receiver>
    auto connect(Receiver&& rcvr) {
        return state<std::remove_cvref_t<Receiver>>{rcvr, this->from, this->to, {}};
    }
};
} // namespace beman::sequence_next

// ----------------------------------------------------------------------------

#endif
