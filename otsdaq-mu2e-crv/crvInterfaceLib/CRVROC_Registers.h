#ifndef CRVROC_REGISTERS_H
#define CRVROC_REGISTERS_H

//#include <bitset> // std::bitset
//#include <cstdint> // uint8_t, uint16_t

namespace CRVLib {
enum ROC_Register : uint16_t
{
    // FPGA0
	CSRRegAddr      = 0x000,
    GTPCSRAddr      = 0x001,
    GTPFIFOAddr     = 0x002,
    ActvRegAddrHi   = 0x008,
    ActvRegAddrLo   = 0x009,
    PLLPDnAddr      = 0x019,
    TestCounter     = 0x035,
    LinkCSRAddr     = 0x027,
    HrtBtOffsetAd   = 0x038,

    // FPGA 1, 2, 3
    BROADCAST       = 0x300,
    FPGA1           = 0x400,
    FPGA2           = 0x800,
    FPGA3           = 0xc00,

    LinkCtrlAd      = 0x001,
    SDRamWrtPtrHiAd = 0x002,
    SDRamWrtPtrLoAd = 0x003,
    SDRamRdPtrHiAd  = 0x004,
    SDRamRdPtrLoAd  = 0x005,


    // uC functions on ROC
    LP            = 0x8000,
    RESET         = 0x8001,
    TRIG          = 0x800b,
    PWRRST        = 0x800a
};
} // namespace CRVLib


#endif //CRVROC_REGISTERS_H