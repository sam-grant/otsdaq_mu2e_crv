#ifndef CRVROC_REGISTERS_H
#define CRVROC_REGISTERS_H

//#include <bitset> // std::bitset
//#include <cstdint> // uint8_t, uint16_t

namespace CRVLib {
enum ROC_Register : uint16_t
{
	DesignVersion = 0x0000,
    TestCounter   = 0x0035
};
} // namespace CRVLib


#endif //CRVROC_REGISTERS_H