#include <gtest/gtest.h>
#include "CmdInterface.h" 
#include <vector>

struct SplitStringTestParam {
    std::string input;
    std::string expected_part1;
    std::string expected_part2;
};

class SplitStringTest : public ::testing::TestWithParam<SplitStringTestParam> {

};

TEST_P(SplitStringTest, SplitsCorrectly) {
    SplitStringTestParam const& param = GetParam();
    std::string part1, part2;
    CmdInterface::splitString(param.input, part1, part2);
    EXPECT_EQ(part1, param.expected_part1);
    EXPECT_EQ(part2, param.expected_part2);
}

INSTANTIATE_TEST_SUITE_P(
    Splits,
    SplitStringTest,
    ::testing::Values(
        SplitStringTestParam{"test-string", "test", "string"},
        SplitStringTestParam{"no-dash", "no", "dash"},
        SplitStringTestParam{"multiple-dashes-string", "multiple", "dashes-string"},
        SplitStringTestParam{"-", "", ""},
        SplitStringTestParam{"", "", ""}
        // 更多测试用例
    )
);

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
