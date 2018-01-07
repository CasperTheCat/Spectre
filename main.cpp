#include <stdint.h>
#include <string.h>
#include <iostream>
#include "util.h"
#include "spectre.h"
#include <cstdio>

char red[] = { 0x1b, '[', '1', ';', '3', '1', 'm', 0 };
char green[] = { 0x1b, '[', '1', ';', '3', '2', 'm', 0 };
char secret[14] = "IeatJeLlyf1sh";

int main(int argc, char **argv)
{
    // I am Spectre
    auto spectre = new crySpectre();

    void *target;
    int len = 0;

    // This doesn't work btw
    if(argc == 3)
    {
        sscanf(argv[1], "%p", (void**)(&target));
        sscanf(argv[2], "%d", &len);
        printf("%p\n", target);
        spectre->target(target,len);
    }
    else
        spectre->target(secret, 14);

    // Is Spectre possible?
    bool isSpectrePossible = false;
    for(int i = 0; i < 10; ++i)
    {
        spectre->launch();
        if(!strcmp(secret, spectre->getResults()))
        {
            isSpectrePossible = true;
            std::cout << red << "Spectre is possible on this machine" << std::endl;
            break;
        }
    }

    if(!isSpectrePossible)
    {
        std::cout << green << "Spectre is not possible on this machine" << std::endl;
    }

    // Cleanup
    delete spectre;

    return 0;
}
