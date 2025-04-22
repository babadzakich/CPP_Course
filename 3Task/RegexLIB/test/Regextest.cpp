//
// Created by babadzakich on 08.04.2025.
//
#include <gtest/gtest.h>
#include "Regex.h"

TEST(RegexTest, SingleCharMatch) {
    Regex re("a");
    EXPECT_TRUE(re.match("a"));
    EXPECT_FALSE(re.match("b"));
}

TEST(RegexTest, DotMatchesAny) {
    Regex re(".");
    EXPECT_TRUE(re.match("a"));
    EXPECT_TRUE(re.match("5"));
    EXPECT_FALSE(re.match(""));
}

TEST(RegexTest, CharGroupMatch) {
    Regex re("[abc]");
    EXPECT_TRUE(re.match("a"));
    EXPECT_TRUE(re.match("b"));
    EXPECT_FALSE(re.match("d"));
}

TEST(RegexTest, NegatedCharGroup) {
    Regex re("[^abc]");
    EXPECT_TRUE(re.match("d"));
    EXPECT_FALSE(re.match("a"));
}

TEST(RegexTest, CharRangeMatch) {
    Regex re("[a-d]");
    EXPECT_TRUE(re.match("a"));
    EXPECT_TRUE(re.match("c"));
    EXPECT_FALSE(re.match("e"));
}

TEST(RegexTest, ZeroOrMoreGreedy) {
    Regex re("a*");
    EXPECT_TRUE(re.match(""));
    EXPECT_TRUE(re.match("aaaa"));

    Regex re2("a*a");
    EXPECT_FALSE(re2.match("aa"));
}

TEST(RegexTest, OneOrMoreGreedy) {
    Regex re("a+");
    EXPECT_FALSE(re.match(""));
    EXPECT_TRUE(re.match("aaaa"));

    Regex re2("a+a");
    EXPECT_FALSE(re2.match("aa"));
}

TEST(RegexTest, ZeroOrOneLazy) {
    Regex re("a?");
    EXPECT_TRUE(re.match(""));
    EXPECT_TRUE(re.match("a"));
    EXPECT_FALSE(re.match("aa"));

    Regex re2("a?a");
    EXPECT_TRUE(re2.match("a"));
}

TEST(RegexTest, SearchFunction) {
    Regex re("abc");
    EXPECT_TRUE(re.search("xyzabc"));
    EXPECT_FALSE(re.search("axbycz"));
}

TEST(RegexTest, ParseErrorUnclosedGroup) {
    EXPECT_THROW(Regex("[abc"), std::runtime_error);
}

TEST(RegexTest, ComplexPattern) {
    Regex re("[ab]+c?");
    EXPECT_TRUE(re.match("aab"));
    EXPECT_TRUE(re.match("bc"));
    EXPECT_FALSE(re.match("cba"));
}

TEST(RegexTest, GreedyVsLazy) {
    Regex reGreedy("a*a");
    EXPECT_FALSE(reGreedy.match("aa"));

    Regex reLazy("a?a");
    EXPECT_TRUE(reLazy.match("a"));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}