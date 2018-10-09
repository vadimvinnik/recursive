#pragma once

#include <type_traits>
#include <optional>

namespace
{

template <typename ForwardIt>
constexpr
ForwardIt advance(ForwardIt const it)
{
    auto result = it;
    ++result;
    return result;
}

} // namespace

namespace recursive
{

template <
    typename Result,
    typename Func,
    typename ForwardIt>
Result foldr(
    Func func,
    ForwardIt const from,
    ForwardIt const to,
    Result const seed)
{
    return from == to
        ? seed
        : func(
            *from,
            foldr(
                func,
                advance(from),
                to,
                seed));
}

template <
    typename Result,
    typename Func,
    typename ForwardIt>
Result foldl(
    Func func,
    ForwardIt const from,
    ForwardIt const to,
    Result const seed)
{
    return from == to
        ? seed
        : foldl(
            func,
            advance(from),
            to,
            func(
                *from,
                seed));
}

template <typename ForwardIt>
struct range_traits
{
    using iterator = ForwardIt;
    using range = std::pair<iterator, iterator>;
    using uncons = std::pair<iterator, range>;
    using maybe = std::optional<uncons>;
};

template <typename ForwardIt>
using range_t = typename range_traits<ForwardIt>::range;

template <typename ForwardIt>
using maybe_t = typename range_traits<ForwardIt>::maybe;

template <typename Container>
using iterator_of = decltype(std::cbegin(std::declval<Container>()));

template <typename Container>
using container_traits = range_traits<iterator_of<Container>>;

template <typename Container>
using container_range_t = typename container_traits<Container>::range;

template <typename Container>
using container_maybe_t = typename container_traits<Container>::maybe;

template <typename ForwardIt>
maybe_t<ForwardIt> uncons_range(range_t<ForwardIt> const range)
{
    return range.first == range.second
        ? std::nullopt
        : std::make_optional(
            std::make_pair(
                range.first,
                std::make_pair(
                    advance(range.first),
                    range.second)));
}

template <typename Container>
constexpr
container_range_t<Container> to_range(Container const& container)
noexcept
{
    return std::make_pair(std::cbegin(container), std::cend(container));
}

template <typename T, size_t N>
constexpr
range_t<T const*> to_range(T const (&container)[N])
noexcept
{
    return std::make_pair(std::cbegin(container), std::cend(container));
}

template <
    typename Result,
    typename Func,
    typename Unconser,
    typename Unconsable>
Result foldr(
    Func func,
    Unconser unconser,
    Unconsable unconsable,
    Result const& seed)
{
    auto const maybe_uncons = unconser(unconsable);

    return !maybe_uncons.has_value()
        ? seed
        : func(
            *maybe_uncons.value().first,
            foldr(
                func,
                unconser,
                maybe_uncons.value().second,
                seed));
}

template <
    typename Result,
    typename Func,
    typename Unconser,
    typename Unconsable>
Result foldl(
    Func func,
    Unconser unconser,
    Unconsable unconsable,
    Result const& seed)
{
    auto const maybe_uncons = unconser(unconsable);

    return !maybe_uncons.has_value()
        ? seed
        : foldl(
            func,
            unconser,
            maybe_uncons.value().second,
            func(
                *maybe_uncons.value().first,
                seed));
}

template <
    typename Result,
    typename Unfolder,
    typename Combiner,
    typename Seed>
Result unfoldr(
    Unfolder unfolder,
    Combiner combiner,
    Result const& initial,
    Seed const& seed)
{
    auto maybe_unfolded = unfolder(seed);

    return !maybe_unfolded.has_value()
        ? initial
        : combiner(
            maybe_unfolded.value().first,
            unfoldr(
                unfolder,
                combiner,
                initial,
                maybe_unfolded.value().second));
}

template <
    typename Result,
    typename Unfolder,
    typename Combiner,
    typename Seed>
Result unfoldl(
    Unfolder unfolder,
    Combiner combiner,
    Result const& initial,
    Seed const& seed)
{
    auto maybe_unfolded = unfolder(seed);

    return !maybe_unfolded.has_value()
        ? initial
        : unfoldl(
            unfolder,
            combiner,
            combiner(
                maybe_unfolded.value().first,
                initial),
            maybe_unfolded.value().second);
}

} // namespace recursive
