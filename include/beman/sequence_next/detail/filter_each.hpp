// include/beman/sequence_next/detail/filter_each.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_INCLUDE_BEMAN_SEQUENCE_NEXT_DETAIL_FILTER_EACH
#define INCLUDED_INCLUDE_BEMAN_SEQUENCE_NEXT_DETAIL_FILTER_EACH

#include <beman/sequence_next/detail/set_next.hpp>
#include <beman/sequence_next/detail/conditional_element.hpp>
#include <beman/sequence_next/detail/state_helper.hpp>
#include <beman/execution/execution.hpp>

// ----------------------------------------------------------------------------

namespace beman::sequence_next::detail {

struct filter_each_t : ::beman::execution::sender_adaptor_closure<filter_each_t> {
    template <beman::execution::sender Sender, typename Predicate>
    struct sender {
        using sender_concept        = ::beman::execution::sender_t;
        using completion_signatures = ::beman::execution::completion_signatures<::beman::execution::set_value_t()>;

        template <typename Receiver>
        struct state {
            using operation_state_concept = ::beman::execution::operation_state_t;
            struct receiver {
                using receiver_concept = ::beman::execution::receiver_t;

                template <::beman::execution::sender S>
                auto set_next(S sndr) {
                    return ::beman::sequence_next::conditional_element(
                        ::std::move(sndr),
                        [&rcvr = this->st->rcvr]<typename A>(A&& a) {
                            return ::beman::sequence_next::set_next(
                                rcvr,
                                ::std::forward<A>(a)
                            );
                        },
                        this->st->predicate
                    );
                }
                template <typename... A>
                auto set_value(A&&... a) && noexcept ->void {
                    ::beman::execution::set_value(
                        ::std::move(this->st->rcvr),
                        ::std::forward<A>(a)...
                        );
                }
                template <typename E>
                auto set_error(E&& e) && noexcept ->void {
                    ::beman::execution::set_error(
                        ::std::move(this->st->rcvr),
                        ::std::forward<E>(e)
                        );
                }
                auto set_stopped() && noexcept ->void {
                    ::beman::execution::set_stopped(::std::move(this->st->rcvr));
                }

                state* st;
            };

            using inner_t = ::beman::sequence_next::detail::state_helper<Sender, receiver>;

            Receiver  rcvr;
            Predicate predicate;
            inner_t   inner;

            template <typename S, typename P, typename R>
            state(S&& s, P&& p, R&& r)
                : rcvr(std::forward<R>(r))
                , predicate(std::forward<P>(p))
                , inner(::std::forward<S>(s), receiver{this}) {
            }

            auto start() & noexcept {
                ::beman::execution::start(this->inner);
            }
        };

        Sender    sender;
        Predicate predicate;

        template <::beman::execution::receiver Receiver>
        auto connect(Receiver&& rcvr) {
            return state<std::remove_cvref_t<Receiver>>(
                std::move(this->sender), std::move(predicate), std::forward<Receiver>(rcvr));
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
