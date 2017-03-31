#include "gtest\gtest.h"
#include "Args.h"
using namespace std;
TEST(Test, testCreateWithNoSchemaOrArguments)
{
    Args args("", {});
    EXPECT_EQ(0, args.cardinality());
}

TEST(Test, testWithNoSchemaButWithOneArgument)
{
    try {
        Args("", { "-x" });
        FAIL();
    }
    catch (const ArgsException& e) {
        EXPECT_EQ(ArgsException::ErrorCode::UNEXPECTED_ARGUMENT,
            e.getErrorCode());
        EXPECT_EQ('x', e.getErrorArgumentId());
    }
}

TEST(Test, testWithNoSchemaButWithMultipleArguments) {
    try {
        Args("", { "-x", "-y" });
        FAIL();
    }
    catch (const ArgsException& e) {
        EXPECT_EQ(ArgsException::ErrorCode::UNEXPECTED_ARGUMENT,
            e.getErrorCode());
        EXPECT_EQ('x', e.getErrorArgumentId());
    }
}

TEST(Test, testNonLetterSchema) {
    try {
        Args("*", {});
        FAIL() << "Args constructor should have thrown exception";
    }
    catch (const ArgsException& e) {
        EXPECT_EQ(ArgsException::ErrorCode::INVALID_FORMAT,
            e.getErrorCode());
        EXPECT_EQ('*', e.getErrorArgumentId());
    }
}

TEST(Test, testInvalidArgumentFormat) {
    try {
        Args("f~", {});
        FAIL() << "Args constructor should have throws exception";
    }
    catch (const ArgsException& e) {
        EXPECT_EQ(ArgsException::ErrorCode::INVALID_FORMAT, e.getErrorCode());
        EXPECT_EQ('f', e.getErrorArgumentId());
    }
}

TEST(Test, testSimpleBooleanPresent) {
    Args args = Args("x", { "-x" });
    EXPECT_EQ(1, args.cardinality());
    EXPECT_EQ(true, args.getBoolean('x'));
}

TEST(Test, testSimpleStringPresent) {
    Args args = Args("x*", { "-x", "param" });
    EXPECT_EQ(1, args.cardinality());
    ASSERT_TRUE(args.has('x'));
    EXPECT_EQ("param", args.getString('x'));
}

TEST(Test, testMissingStringArgument) {
    try {
        Args("x*", { "-x" });
        FAIL();
    }
    catch (const ArgsException& e) {
        EXPECT_EQ(ArgsException::ErrorCode::MISSING_STRING, e.getErrorCode());
        EXPECT_EQ('x', e.getErrorArgumentId());
    }
}

TEST(Test, testSpacesInFormat) {
    Args args = Args("x, y", { "-xy" });
    EXPECT_EQ(2, args.cardinality());
    ASSERT_TRUE(args.has('x'));
    ASSERT_TRUE(args.has('y'));
}

TEST(Test, testSimpleIntPresent) {
    Args args = Args("x#", { "-x", "42" });
    EXPECT_EQ(1, args.cardinality());
    ASSERT_TRUE(args.has('x'));
    EXPECT_EQ(42, args.getInt('x'));
}

TEST(Test, testInvalidInteger) {
    try {
        Args("x#", { "-x", "Forty two" });
        FAIL();
    }
    catch (const ArgsException& e) {
        EXPECT_EQ(ArgsException::ErrorCode::INVALID_INTEGER, e.getErrorCode());
        EXPECT_EQ('x', e.getErrorArgumentId());
        EXPECT_EQ("Forty two", e.getErrorParameter());
    }
}

TEST(Test, testMissingInteger) {
    try {
        Args("x#", { "-x" });
        FAIL();
    }
    catch (const ArgsException& e) {
        EXPECT_EQ(ArgsException::ErrorCode::MISSING_INTEGER, e.getErrorCode());
        EXPECT_EQ('x', e.getErrorArgumentId());
    }
}

TEST(Test, testSimpleDoublePresent) {
    Args args = Args("x##", { "-x", "42.3" });
    EXPECT_EQ(1, args.cardinality());
    ASSERT_TRUE(args.has('x'));
    ASSERT_NEAR(42.3, args.getDouble('x'), .001);
}

TEST(Test, testInvalidDouble) {
    try {
        Args("x##", { "-x", "Forty two" });
        FAIL();
    }
    catch (const ArgsException& e) {
        EXPECT_EQ(ArgsException::ErrorCode::INVALID_DOUBLE, e.getErrorCode());
        EXPECT_EQ('x', e.getErrorArgumentId());
        EXPECT_EQ("Forty two", e.getErrorParameter());
    }
}

TEST(Test, testMissingDouble) {
    try {
        Args("x##", { "-x" });
        FAIL();
    }
    catch (const ArgsException& e) {
        EXPECT_EQ(ArgsException::ErrorCode::MISSING_DOUBLE, e.getErrorCode());
        EXPECT_EQ('x', e.getErrorArgumentId());
    }
}

TEST(Test, testUnexpectedMessage) {
    ArgsException e = ArgsException(ArgsException::ErrorCode::UNEXPECTED_ARGUMENT,
        'x');
    EXPECT_EQ("Argument(s) : x unexpected.", e.what());
}

TEST(Test, testMissingStringMessage) {
    ArgsException e = ArgsException(ArgsException::ErrorCode::MISSING_STRING,
        'x');
    EXPECT_EQ("Could not find string parameter for : x", e.what());
}

TEST(Test, testInvalidIntegerMessage) {
    ArgsException e = ArgsException(ArgsException::ErrorCode::INVALID_INTEGER, 'x');
    e.setErrorParameter("Forty two");
    EXPECT_EQ("Argument -x expects an integer but was 'Forty two'",
        e.what());
}

TEST(Test, testMissingIntegerMessage) {
    ArgsException e = ArgsException(ArgsException::ErrorCode::MISSING_INTEGER, 'x');
    EXPECT_EQ("Could not find integer parameter for : x", e.what());
}

TEST(Test, testInvalidDoubleMessage) {
    ArgsException e = ArgsException(ArgsException::ErrorCode::INVALID_DOUBLE,
        'x');
    e.setErrorParameter("Forty two");
    EXPECT_EQ("Argument -x expects a double but was 'Forty two'",
        e.what());
}

TEST(Test, testMissingDoubleMessage) {
    ArgsException e = ArgsException(ArgsException::ErrorCode::MISSING_DOUBLE,
        'x');
    EXPECT_EQ("Could not find double parameter for : x", e.what());
}
