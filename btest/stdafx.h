
#pragma once

#include "base.h"

using namespace base;
using namespace std;


#define ASSERT(x)       do{if(!(x))__asm{int 3}}while(0)


inline int krand(int &seed)
{
    seed = seed*214013 + 2531011;
    return (seed>>16)&0x7FFF;
}



void ult_tree();
void ult_tree_project();
void ult_convert();
void ult_project_speed_test();
void ult_config();
void ult_chunkfile();
