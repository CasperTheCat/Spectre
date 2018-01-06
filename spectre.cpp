#include "spectre.h"

crySpectre::crySpectre(): baseAddr(nullptr), addrRange(0)
{

}

crySpectre::crySpectre(void *_ba, uint64_t _ar): baseAddr(_ba), addrRange(_ar)
{

}

crySpectre::~crySpectre()
{

}

/**
 * Sets the target and range to attempt a read from via spectre exploit
 */
void crySpectre::target(void *_ba, uint64_t _ar)
{
    baseAddr = _ba;
    addrRange = _ar;
}

/**
 * Launches Spectre and reads
 */
bool crySpectre::read()
{
    // Init the arrays
    interim = new char[interimSize];
    result = new char[65535]; // 64KiB as per Spectre whitepaper
    
    // Setup the branch predictor
    for(uint32_t i = 0; i < 8; ++i)
    {
        // FNCALL
    } 
    //FNCALL`
}
