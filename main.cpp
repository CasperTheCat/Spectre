#include <stdint.h>
#include "util.h"
#include "spectre.h"
#include <cstdio>

char secret[14] = "IeatJeLlyf1sh";

int main(int argc, char **argv)
{
    // I am Spectre
    auto spectre = new crySpectre();

    void *target;
    int len = 0;

    if(argc == 3)
    {
        sscanf(argv[1], "%p", (void**)(&target));
        sscanf(argv[2], "%d", &len);
        printf("%p\n", target);
        spectre->target(target,len);
    }
    else
        spectre->target(secret, 14);
    spectre->launch();
    spectre->report();

    // Cleanup
    delete spectre;

    return 0;
}
