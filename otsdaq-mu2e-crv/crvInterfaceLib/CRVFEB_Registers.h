#ifndef CRVFEB_REGISTERS_H
#define CRVFEB_REGISTERS_H

//#include <bitset> // std::bitset
//#include <cstdint> // uint8_t, uint16_t

namespace CRVLib {
enum FEB_Register : uint16_t
{
	DesignVersion = 0x0000,
    TestCounter   = 0x0023
};
} // namespace CRVLib


#endif //CRVFEB_REGISTERS_H