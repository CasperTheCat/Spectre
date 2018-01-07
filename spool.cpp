#include <iostream>

char* gblScoped;
char gl[50] = "IamNotAJellyf1sh";


int main(void)
{
    // New amount

    gblScoped = new char[50];
    printf("%p\n", gblScoped);

    while(true)
    {
    }
}
