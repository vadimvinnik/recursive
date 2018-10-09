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

int main()
{
    //int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::list<int> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    std::cout
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

    return 0;
}