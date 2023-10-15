#include <gtest/gtest.h>
#include <random>
#include <chrono>
#include <climits>

#include "MinDiffSiblings.hpp"

// ----------------------------------------------------------------------------- //
TEST(MinDiffSiblingsTest, TestLargeRandomThreads)
{
    std::vector<int> vec(256 * 1024, 0);
    std::random_device r;

    auto startRnd = std::chrono::high_resolution_clock::now();
    std::generate(vec.begin(), vec.end(), [&r]{return r();});
    auto stopRnd = std::chrono::high_resolution_clock::now();
    auto durationRnd = std::chrono::duration_cast<std::chrono::milliseconds>(stopRnd - startRnd);
    std::cout << "Random generation time " << durationRnd.count() << " mls" << std::endl;

    std::vector<int> nThreads = { 1, 4, 16 };
    std::vector<int> results;
    results.reserve(nThreads.size());

    for (const auto& th : nThreads)
    {
        auto start = std::chrono::high_resolution_clock::now();
        results.emplace_back(min_diff_siblings(vec, th));
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << "Time with " << th << "thread(s) " << duration.count() << " microseconds" << std::endl;
    }

    for (int i = 1; i < nThreads.size(); ++i)
    {
        EXPECT_EQ(results[i], results[i - 1]);
    }

    const auto res = results[0];

    std::cout << "Result: "  << std::endl;
    std::cout << "Index: "   << res << std::endl;
    std::cout << "Diff: "    << vec[res] - vec[res - 1] << std::endl;
    std::cout << "Left: "    << vec[res - 1] << std::endl;
    std::cout << "Right: "   << vec[res] << std::endl;
    std::cout << "INT_MIN: " << INT_MIN << std::endl;
    std::cout << "INT_MAX: " << INT_MAX << std::endl;
}

// ----------------------------------------------------------------------------- //
TEST(MinDiffSiblingsTest, TestLargeZeros)
{
    std::vector<int> vec(256 * 1024, 0);
    const auto res = min_diff_siblings(vec, 2);
    EXPECT_EQ(res, 0); // zero index means no answer
}

// ----------------------------------------------------------------------------- //
TEST(MinDiffSiblingsTest, TestSimpleSmallEven)
{
    std::vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8 };
    const auto res = min_diff_siblings(vec, 2);
    EXPECT_EQ(res, 1);
}

// ----------------------------------------------------------------------------- //
TEST(MinDiffSiblingsTest, TestSimpleSmallNotEven)
{
    std::vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    const auto res = min_diff_siblings(vec, 2);
    EXPECT_EQ(res, 1);
}

// ----------------------------------------------------------------------------- //
TEST(MinDiffSiblingsTest, TestSmallExtremeCase1)
{
    std::vector<int> vec = { 0, 10, 20, 30, 40, 50, 60, 70, 71 };
    const auto res = min_diff_siblings(vec, 2);
    EXPECT_EQ(res, 8);
}

// ----------------------------------------------------------------------------- //
TEST(MinDiffSiblingsTest, TestSmallExtremeCase2)
{
    std::vector<int> vec = { 0, 10, 20, 30, 31, 50, 60, 70, 71 };
    const auto res = min_diff_siblings(vec, 2);
    EXPECT_EQ(res, 4);
}

// ----------------------------------------------------------------------------- //
TEST(MinDiffSiblingsTest, TestSmallExtremeCase3)
{
    std::vector<int> vec = { 0, 10, 20, 30, 31, 50, 60, 71, 70 };
    const auto res = min_diff_siblings(vec, 2);
    EXPECT_EQ(res, 4);
}

// ----------------------------------------------------------------------------- //
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
