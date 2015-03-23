
#include "stdafx.h"


#define FtoS(f,s)       ((s)="", AppendFloat((s),(f)))
#define StoF(s,f)       do{const char *ss=(s).c_str(); (f)=ParseFloat(ss);}while(0)






void ult_convert()
{
    string s, ns;
    DWORD i = 0;


    //DWORD vv = 0xFF800000;
    //printf("%g\n",*(float*)&vv);

    int seed = 123;
    while(1)
    {
        DWORD _i, v = -1;

        FtoS(*(float*)&i,s);
        StoF(s,*(float*)&v);
        FtoS(*(float*)&v,ns);

        if(*(float*)&i==*(float*)&i)
            ASSERT( v == i );

        _i = i;
        i += krand(seed);
        if(i<_i) break;
    }

}