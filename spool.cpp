#include <iostream>
#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h>

char* gblScoped;
char gl[17] = "IamNotAJellyf1sh";


int main(void)
{
    // New amount

    gblScoped = new char[17];
    memset_s(gblScoped,17,0,17);

    printf("%p\n", gblScoped);
    
    for(int i = 0; i < 16; ++i)
    {
        gblScoped[i] = gl[i];
    }

    while(true)
    {

    }
}


