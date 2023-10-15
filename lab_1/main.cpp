#include <iostream>
#include "MinDiffSiblings.hpp"

//----------------------------------------------------------------------------//
int main()
{
    //std::vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::vector<int> vec = { 0, 10, 20, 30, 31, 50, 60, 70, 71 };
    const auto res = min_diff_siblings(vec, 2);
    std::cout << "Min difference siblings index: " << res << std::endl;
    return 0;
}

