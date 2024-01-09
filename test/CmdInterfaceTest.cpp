#include <gtest/gtest.h>
#include "ZipFileList.h" 
#include <vector>

// 测试 str_to_vec 函数
TEST(StrToVecTest, HandlesEmptyString) {
    std::string empty_str = "";
    std::vector<char> result = str_to_vec(empty_str);
    EXPECT_TRUE(result.empty());
}

TEST(StrToVecTest, HandlesNonEmptyString) {
    std::string str = "test";
    std::vector<char> result = str_to_vec(str);
    ASSERT_EQ(result.size(), str.size());
    for (size_t i = 0; i < str.size(); ++i) {
        EXPECT_EQ(result[i], str[i]);
    }
}

// 测试 vec_to_string 函数
TEST(VecToStringTest, HandlesEmptyVector) {
    std::vector<char> empty_vec;
    std::string result = vec_to_string(empty_vec);
    EXPECT_TRUE(result.empty());
}

TEST(VecToStringTest, HandlesNonEmptyVector) {
    std::vector<char> vec = {'t', 'e', 's', 't'};
    std::string result = vec_to_string(vec);
    EXPECT_EQ(result, "test");
}

class ZipFileListTest : public ::testing::Test {
protected:
    ZipFileList zfl;

    void SetUp() override {
       zfl.zipAccessor = new ZipAccessor;
    }

    void TearDown() override {
       zfl.zipAccessor = new ZipAccessor;
    }
};

TEST_F(ZipFileListTest, SetPasswordNoOldPassword) {
    EXPECT_EQ(zfl.set_password("newpassword"), 1);
}

TEST_F(ZipFileListTest, SetPasswordWithOldPassword) {
    zfl.set_password("oldpassword");
    EXPECT_EQ(zfl.set_password("newpassword"), 0);
}

TEST_F(ZipFileListTest, ResetPasswordCorrectOldPassword) {
    zfl.set_password("oldpassword");
    EXPECT_EQ(zfl.reset_password("oldpassword"), 1);
}

TEST_F(ZipFileListTest, ResetPasswordWrongOldPassword) {
    zfl.set_password("oldpassword");
    EXPECT_EQ(zfl.reset_password("wrongpassword"), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
