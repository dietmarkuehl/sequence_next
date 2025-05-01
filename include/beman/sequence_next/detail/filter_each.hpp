// include/beman/sequence_next/detail/filter_each.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_INCLUDE_BEMAN_SEQUENCE_NEXT_DETAIL_FILTER_EACH
#define INCLUDED_INCLUDE_BEMAN_SEQUENCE_NEXT_DETAIL_FILTER_EACH

#include <beman/sequence_next/detail/set_next.hpp>
#include <beman/execution/execution.hpp>

// ----------------------------------------------------------------------------

namespace beman::sequence_next::detail {

struct filter_each_t : ::beman::execution::sender_adaptor_closure<filter_each_t> {
    template <beman::execution::sender Sender, typename Predicate>
    struct sender {
        using sender_concept        = ::beman::execution::sender_t;
        using completion_signatures = ::beman::execution::completion_signatures<::beman::execution::set_value_t()>;

        struct state_base_base {
            Predicate predicate;
        };
        template <typename Receiver>
        struct state_base {
            Receiver  rcvr;
            Predicate predicate;
            template <typename R, typename P>
            state_base(R&& r, P&& p) : rcvr(std::forward<R>(r)), predicate(std::forward<P>(p)) {}
        };

        template <typename ElementSender>
        struct element_sender {
            template <::beman::execution::receiver Receiver>
            struct state {
                using operation_state_concept = ::beman::execution::operation_state_t;
                std::remove_cvref_t<Receiver> receiver;
            };
            using sender_concept = ::beman::execution::sender_t;
            std::remove_cvref_t<ElementSender> downstream;
        };

        template <::beman::execution::receiver Receiver>
        struct receiver {
            using receiver_concept = ::beman::execution::receiver_t;
            state_base<Receiver>* st;

            template <::beman::execution::sender S>
            auto set_next(S sndr) {
                auto s{this->st};
                return element_sender(::std::move(sndr), this);
            }
            auto set_value() && noexcept { ::beman::execution::set_value(std::move(this->st->rcvr)); }
        };

        template <::beman::execution::receiver Receiver>
        struct state : state_base<Receiver> {
            using operation_state_concept = ::beman::execution::operation_state_t;
            using inner_t =
                decltype(::beman::execution::connect(std::declval<Sender>(), std::declval<receiver<Receiver>>()));

            inner_t inner;

            template <typename F, ::beman::execution::sender S, ::beman::execution::receiver R>
            state(F&& f, S&& s, R&& r)
                : state_base<Receiver>(std::forward<R>(r), std::forward<F>(f)),
                  inner(::beman::execution::connect(std::forward<S>(s), receiver<Receiver>{this})) {}
            void start() & noexcept { ::beman::execution::start(this->inner); }
        };

        Sender    sender;
        Predicate predicate;

        template <::beman::execution::receiver Receiver>
        auto connect(Receiver&& rcvr) {
            return state<std::remove_cvref_t<Receiver>>(
                std::move(this->predicate), std::move(sender), std::forward<Receiver>(rcvr));
        }
    };

    template <::beman::execution::sender Sender, typename Predicate>
    sender<::std::remove_cvref_t<Sender>, ::std::remove_cvref_t<Predicate>> operator()(Sender&&    sndr,
                                                                                       Predicate&& predicate) const {
        return {::std::forward<Sender>(sndr), ::std::forward<Predicate>(predicate)};
    }
    template <typename Predicate>
    auto operator()(Predicate&& predicate) const {
        return ::beman::execution::detail::sender_adaptor{*this, ::std::forward<Predicate>(predicate)};
    }
};

} // namespace beman::sequence_next::detail

namespace beman::sequence_next {
using filter_each_t = ::beman::sequence_next::detail::filter_each_t;
inline constexpr filter_each_t filter_each{};
} // namespace beman::sequence_next

// ----------------------------------------------------------------------------

#endif
