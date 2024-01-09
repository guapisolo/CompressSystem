#include <gtest/gtest.h>

// int add(int a, int b) {
//     return a + b;
// }

// TEST(AddTest, PositiveNumbers) {
//     EXPECT_EQ(2, add(1, 1));
// }

// TEST(AddTest, NegativeNumbers) {
//     EXPECT_EQ(-2, add(-1, -1));
// }

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
