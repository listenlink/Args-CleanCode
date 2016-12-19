#include "stdafx.h"
#include "gtest\gtest.h"
#include "Args.h"

TEST(test_boolean_args, with_one_flag)
{
    string schema("l");
    vector<string> args{ "-l" };
    Args arg(schema, args);
    EXPECT_EQ(true, arg.getBoolean('l'));
    EXPECT_EQ(false, arg.getBoolean('m'));
}

TEST(test_boolean_args, schema_with_leading_tailing_space)
{
    string schema("  l   ");
    vector<string> args{ "-l" };
    Args arg(schema, args);
    EXPECT_EQ(true, arg.getBoolean('l'));
}

TEST(test_boolean_args, with_multiple_flags_and_spaces)
{
    string schema("  l   ,m , n, o ");
    vector<string> args{ "-l -n" };
    Args arg(schema, args);
    EXPECT_EQ(true, arg.getBoolean('l'));
    EXPECT_EQ(false, arg.getBoolean('m'));
    EXPECT_EQ(true, arg.getBoolean('n'));
    EXPECT_EQ(false, arg.getBoolean('o'));
}