#ifndef CRVROC_REGISTERS_H
#define CRVROC_REGISTERS_H

//#include <bitset> // std::bitset
//#include <cstdint> // uint8_t, uint16_t

namespace CRVLib {
enum ROC_Register : uint16_t
{
	DesignVersion = 0x0000,
    ActvRegAddrHi = 0x0008,
    ActvRegAddrLo = 0x0009,
    TestCounter   = 0x0035,

    // uC functions
    LP            = 0x8000 
};
} // namespace CRVLib


#endif //CRVROC_REGISTERS_H