#include "stdafx.h"
#include <iostream>
#include "gtest\gtest.h"
#include "Args.h"
using namespace std;

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    std::getchar(); // keep console window open until Return keystroke
    return 1;
}