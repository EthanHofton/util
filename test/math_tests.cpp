#include <gtest/gtest.h>
#include <util/math.hpp>

TEST(Math, PI) {
    EXPECT_EQ(util::PI, 3.1415926535897932);
}

TEST(Math, E) {
    EXPECT_EQ(util::E, 2.7182818284590452);
}

TEST(Math, EPSILON) {
    EXPECT_EQ(util::EPSILON, 0.00001);
}

TEST(Math, feq) {
    EXPECT_TRUE(util::feq(1.0f, 1.0f));
    EXPECT_FALSE(util::feq(1.0f, 1.0001f));
    EXPECT_FALSE(util::feq(1.0f, 1.00001f));
    EXPECT_TRUE(util::feq(1.0f, 1.00001f, 0.0001));
    EXPECT_FALSE(util::feq(1.0f, 1.00001f, 0.000001));
    EXPECT_FALSE(util::feq(1.0f, 1.00001f, 0.00000001));
}

TEST(Math, deq) {
    EXPECT_TRUE(util::deq(1.0, 1.0));
    EXPECT_FALSE(util::deq(1.0, 1.00001));
    EXPECT_TRUE(util::deq(1.0, 1.00001, 0.0001));
    EXPECT_FALSE(util::deq(1.0, 1.00001, 0.000001));
    EXPECT_FALSE(util::deq(1.0, 1.00001, 0.00000001));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}