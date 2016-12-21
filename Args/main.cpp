#include "stdafx.h"
#include <iostream>
#include "gtest\gtest.h"
#include "Args.h"
using namespace std;


int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    //std::getchar(); // keep console window open until Return keystroke
    return 1;
}

int main1()
{
    vector<string> args;
    args.push_back("-l");
    args.push_back("-p 1234");
    args.push_back("-d abc");
    Args arg(string("l,p#,d*"), args);
    bool logging = arg.getBoolean('l');
    int port = arg.getInt('p');
    string directory = arg.getString('d');
    cout << logging << " " << port << " " << directory << std::endl;    
    return 0;

}
