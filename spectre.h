#include <cstdint>

class crySpectre
{
protected:
    uint32_t interimSize = 128; // Arbitrary
    void *baseAddr;
    uint64_t addrRange;
    char *interim;

public:
    char *result;

public:
    crySpectre();
    crySpectre(void *_ba, uint64_t _ar);
    ~crySpectre();

    // Assign base and range pointers
    void target(void *_ba, uint64_t _ar);

    // Attempt to pull off spectre
    bool read();
}
