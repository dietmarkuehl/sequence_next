// include/beman/sequence_next/detail/conditional_element.hpp                        -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_INCLUDE_BEMAN_SEQUENCE_NEXT_DETAIL_CONDITONAL_ELEMENT
#define INCLUDED_INCLUDE_BEMAN_SEQUENCE_NEXT_DETAIL_CONDITONAL_ELEMENT

#include <beman/execution/execution.hpp>
#include <beman/sequence_next/detail/set_next.hpp>
#include <type_traits>
#include <utility>
#include <variant>

// ----------------------------------------------------------------------------

namespace beman::sequence_next::detail {
template <beman::execution::sender S, beman::execution::receiver R>
struct state_helper {
    using sender_t = S;
    using state_t  = decltype(::beman::execution::connect(::std::declval<sender_t>(), ::std::declval<R>()));
    state_t state;
    state_helper(S&& s, R&& r) : state(::beman::execution::connect(::std::forward<S>(s), ::std::forward<R>(r))) {}
};

struct conditional_element_t {
    template <::beman::execution::sender Sender_up, typename Factory, typename Predicate>
    struct sender {
        using sender_concept = ::beman::execution::sender_t;
        template <typename Env>
        auto get_completion_signatures(const Env& e) const noexcept {
            return beman::execution::get_completion_signatures(this->sender_up, e);
        }

        template <typename Receiver>
        struct just_state_helper {
            template <typename... A>
            using type =
                state_helper<decltype(std::declval<Factory>()(::beman::execution::just(std::declval<A>()...))),
                             Receiver>;
        };
        template <::beman::execution::receiver Receiver>
        struct state {
            using operation_state_concept = ::beman::execution::operation_state_t;
            using env_t                   = decltype(::beman::execution::get_env(::std::declval<Receiver>()));
            template <typename... A>
            using just_state_helper_t = typename just_state_helper<Receiver>::template type<A...>;
            template <typename... T>
            using variant_with_monostate = std::variant<std::monostate, T...>;

            using state_variant =
                ::beman::execution::value_types_of_t<Sender_up, env_t, just_state_helper_t, variant_with_monostate>;

            struct receiver_up {
                using receiver_concept = ::beman::execution::receiver_t;

                state* st;

                template <typename... A>
                auto set_value(A&&... a) && noexcept -> void {
                    if (this->st->predicate(a...)) {
                        auto sndr          = this->st->factory(::beman::execution::just(::std::forward<A>(a)...));
                        using down_state_t = decltype(state_helper(std::move(sndr), std::move(this->st->receiver)));
                        auto& s{this->st->state_down.template emplace<down_state_t>(std::move(sndr),
                                                                                    std::move(this->st->receiver))};
                        ::beman::execution::start(s.state);
                    } else {
                        ::beman::execution::set_value(std::move(this->st->receiver));
                    }
                }
                template <typename Error>
                auto set_error(Error&& error) && noexcept -> void {
                    ::beman::execution::set_error(std::move(this->st->receiver), std::forward<Error>(error));
                }
                auto set_stopped() && noexcept -> void {
                    ::beman::execution::set_stopped(std::move(this->st->receiver));
                }
            };

            using inner_state_t =
                decltype(::beman::execution::connect(std::declval<Sender_up>(), std::declval<receiver_up>()));

            Receiver      receiver;
            inner_state_t inner_state;
            Factory       factory;
            Predicate     predicate;
            state_variant state_down;

            template <typename R, typename U, typename F, typename P>
            state(R&& rcvr, U&& u, F&& f, P&& p)
                : receiver(std::forward<R>(rcvr)),
                  inner_state(::beman::execution::connect(::std::forward<U>(u), receiver_up{this})),
                  factory(std::forward<F>(f)),
                  predicate(std::forward<P>(p)) {}

            void start() & noexcept { ::beman::execution::start(this->inner_state); }
        };

        template <::beman::execution::receiver Receiver>
        auto connect(Receiver&& rcvr) && {
            return state<std::remove_cvref_t<Receiver>>(std::forward<Receiver>(rcvr),
                                                        std::move(this->sender_up),
                                                        std::move(this->factory),
                                                        std::move(this->pred));
        }

        Sender_up sender_up;
        Factory   factory;
        Predicate pred;
    };

    template <::beman::execution::sender Sender_up, typename Factory, typename Pred>
    sender<::std::remove_cvref_t<Sender_up>, ::std::remove_cvref_t<Factory>, ::std::remove_cvref_t<Pred>>
    operator()(Sender_up&& sndr_up, Factory&& factory, Pred&& pred) const {
        return {::std::forward<Sender_up>(sndr_up), ::std::forward<Factory>(factory), ::std::forward<Pred>(pred)};
    }
};
} // namespace beman::sequence_next::detail

namespace beman::sequence_next {
using conditional_element_t = ::beman::sequence_next::detail::conditional_element_t;
inline constexpr conditional_element_t conditional_element{};
} // namespace beman::sequence_next

#endif
