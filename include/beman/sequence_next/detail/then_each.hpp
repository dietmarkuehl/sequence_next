// include/beman/sequence_next/detail/then_each.hpp                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_INCLUDE_BEMAN_SEQUENCE_NEXT_DETAIL_THEN_EACH
#define INCLUDED_INCLUDE_BEMAN_SEQUENCE_NEXT_DETAIL_THEN_EACH

#include <beman/execution/execution.hpp>
#include <beman/execution/detail/suppress_push.hpp>

// ----------------------------------------------------------------------------

namespace beman::sequence_next::detail {

struct then_each_t
    : ::beman::execution::sender_adaptor_closure<then_each_t>
{
    template <beman::execution::sender Sender, typename Fun>
    struct sender {
        using sender_concept = ::beman::execution::sender_t;
        using completion_signatures = ::beman::execution::completion_signatures<
            ::beman::execution::set_value_t()
        >;

        template <typename Receiver>
        struct state_base {
            Receiver rcvr;
            Fun fun;
            template <typename R, typename F>
            state_base(R&& r, F&& f)
                : rcvr(std::forward<R>(r))
                , fun(std::forward<F>(f)) {}
        };

        template <::beman::execution::receiver Receiver>
        struct receiver {
            using receiver_concept = ::beman::execution::receiver_t;
            state_base<Receiver>* st;

            template <::beman::execution::sender S>
            auto set_next(S sndr) {
                auto s{this->st};
                return ::beman::sequence_next::set_next(s->rcvr, std::move(sndr) | ::beman::execution::then(s->fun));
            }
            auto set_value() && noexcept {
                ::beman::execution::set_value(std::move(this->st->rcvr));
            }
        };

        template <::beman::execution::receiver Receiver>
        struct state
            : state_base<Receiver> {
            using operation_state_concept = ::beman::execution::operation_state_t;
            using inner_t = decltype(::beman::execution::connect(std::declval<Sender>(), std::declval<receiver<Receiver>>()));

            inner_t  inner;

            template <typename F, ::beman::execution::sender S, ::beman::execution::receiver R>
            state(F&& f, S&& s, R&& r)
                : state_base<Receiver>(std::forward<R>(r), std::forward<F>(f))
                , inner(::beman::execution::connect(std::forward<S>(s), receiver<Receiver>{this}))
            {
            }
            void start() & noexcept {
                ::beman::execution::start(this->inner);
            }
        };

        Sender sender;
        Fun    fun;

        template <::beman::execution::receiver Receiver>
        auto connect(Receiver&& rcvr) {
            return state<std::remove_cvref_t<Receiver>>(std::move(this->fun), std::move(sender), std::forward<Receiver>(rcvr));
        }
    };

    template <::beman::execution::sender Sender, typename Fun>
    sender<::std::remove_cvref_t<Sender>, ::std::remove_cvref_t<Fun>>
    operator()(Sender&& sndr, Fun&& fun) const {
        return { ::std::forward<Sender>(sndr), ::std::forward<Fun>(fun) };
    }
    template <typename Fun>
    auto operator()(Fun&& fun) const {
        return ::beman::execution::detail::sender_adaptor{*this, ::std::forward<Fun>(fun)};
    }
};

}

namespace beman::sequence_next {
    using then_each_t = ::beman::sequence_next::detail::then_each_t;
    inline constexpr then_each_t then_each{};
}

// ----------------------------------------------------------------------------

#include <beman/execution/detail/suppress_pop.hpp>

#endif
