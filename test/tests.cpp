#include <gtest/gtest.h>

TEST(ASimpleTest, Simple) {
  EXPECT_STRNE("hello", "world");
  EXPECT_EQ(3, 3);
}
