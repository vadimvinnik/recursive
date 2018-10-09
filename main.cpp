#include "recursive.h"

#include <iostream>
#include <list>
#include <optional>
#include <string>
#include <utility>

std::string append_int(int x, std::string const& s)
{
    return s + std::to_string(x) + " ";
}

std::string prepend_int(int x, std::string const& s)
{
    return std::to_string(x) + " " + s;
}

struct int_ascending_splitter
{
    int_ascending_splitter(int _upper_bound)
        : upper_bound(_upper_bound)
    {}

    std::optional<std::pair<int, int>> operator()(int x) const
    {
        return x < upper_bound
            ? std::make_optional(std::make_pair(x, x + 1))
            : std::nullopt;
    }

    int const upper_bound;
};

int main()
{
    //int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::list<int> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    std::cout
        << "Fold iterator pairs"
        << std::endl
        << "foldl append_int\t"
        << recursive::foldl<std::string>(append_int, std::begin(data), std::end(data), "")
        << std::endl
        << "foldl prepend_int\t"
        << recursive::foldl<std::string>(prepend_int, std::begin(data), std::end(data), "")
        << std::endl
        << "foldr append_int\t"
        << recursive::foldr<std::string>(append_int, std::begin(data), std::end(data), "")
        << std::endl
        << "foldr prepend_int\t"
        << recursive::foldr<std::string>(prepend_int, std::begin(data), std::end(data), "")
        << std::endl
        << std::endl;

    auto const data_range = recursive::to_range(data);
    const auto& uncons_data = recursive::uncons_range<decltype(std::cbegin(data))>;

    std::cout
        << "Fold ranges"
        << std::endl
        << "foldl append_int\t"
        << recursive::foldl<std::string>(append_int, uncons_data, data_range, "")
        << std::endl
        << "foldl prepend_int\t"
        << recursive::foldl<std::string>(prepend_int, uncons_data, data_range, "")
        << std::endl
        << "foldr append_int\t"
        << recursive::foldr<std::string>(append_int, uncons_data, data_range, "")
        << std::endl
        << "foldr prepend_int\t"
        << recursive::foldr<std::string>(prepend_int, uncons_data, data_range, "")
        << std::endl
        << std::endl;

    int_ascending_splitter generator(10);

    std::cout
        << "Unfold"
        << std::endl
        << "unfoldl\t"
        << recursive::unfoldl<std::string>(generator, append_int, "", 0)
        << std::endl
        << "unfoldr\t"
        << recursive::unfoldr<std::string>(generator, append_int, "", 0)
        << std::endl
        << std::endl;

    return 0;
}