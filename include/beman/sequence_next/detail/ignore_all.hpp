// include/beman/sequence_next/detail/ignore_all.hpp                  -*-C++-*-
// ----------------------------------------------------------------------------
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// ----------------------------------------------------------------------------

#ifndef INCLUDED_INCLUDE_BEMAN_SEQUENCE_NEXT_DETAIL_IGNORE_ALL
#define INCLUDED_INCLUDE_BEMAN_SEQUENCE_NEXT_DETAIL_IGNORE_ALL

#include <beman/sequence_next/detail/set_next.hpp>
#include <beman/execution/execution.hpp>

// ----------------------------------------------------------------------------

namespace beman::sequence_next::detail {

struct ignore_all_t
    : ::beman::execution::sender_adaptor_closure<ignore_all_t>
{
    struct state_base {
        virtual ~state_base() = default;
        virtual void complete() = 0;
    };
    struct receiver {
        using receiver_concept = ::beman::execution::receiver_t;
        state_base* state;

        template <::beman::execution::sender S>
        auto set_next(S sndr) {
            return std::move(sndr) | ::beman::execution::then([](auto&&...){});
        }
        auto set_value() && noexcept {
            this->state->complete();
        }
    };
    static_assert(::beman::execution::receiver<receiver>);

    template <::beman::execution::sender Sender,
              ::beman::execution::receiver Receiver>
    struct state
        : state_base {
        using operation_state_concept = ::beman::execution::operation_state_t;
        using inner_t = decltype(::beman::execution::connect(std::declval<Sender>(), std::declval<receiver>()));

        Receiver rcvr;
        inner_t  inner;

        template <::beman::execution::sender S, ::beman::execution::receiver R>
        state(S&& s, R&& r)
            : rcvr(std::forward<R>(r))
            , inner(::beman::execution::connect(std::forward<S>(s), receiver{this}))
        {
        }
        void start() & noexcept {
            ::beman::execution::start(this->inner);
        }
        void complete() override {
            ::beman::execution::set_value(std::move(this->rcvr));
        }
    };

    template <beman::execution::sender Sender>
    struct sender
    {
        using sender_concept = ::beman::execution::sender_t;
        using completion_signatures = ::beman::execution::completion_signatures<
            ::beman::execution::set_value_t()
        >;

        Sender sender;

        template <::beman::execution::receiver Receiver>
        auto connect(Receiver&& rcvr) {
            return state<Sender, std::remove_cvref_t<Receiver>>(
                std::move(this->sender),
                std::forward<Receiver>(rcvr)
                );
        }
    };

    template <::beman::execution::sender Sender>
    sender<::std::remove_cvref_t<Sender>> operator()(Sender&& sndr) const {
        return { std::forward<Sender>(sndr) };
    }
};

}

namespace beman::sequence_next {
    using ignore_all_t = ::beman::sequence_next::detail::ignore_all_t;
    inline constexpr ignore_all_t ignore_all{};
}

// ----------------------------------------------------------------------------

#endif
