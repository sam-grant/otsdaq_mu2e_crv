#ifndef CRVFEB_REGISTERS_H
#define CRVFEB_REGISTERS_H

//#include <bitset> // std::bitset
//#include <cstdint> // uint8_t, uint16_t

namespace CRVLib {
enum FEB_Register : uint16_t
{
	DesignVersion   = 0x000,
    TestCounter     = 0x023,

    BiasTrimBase    = 0x030, 
    BiasBase        = 0x044, 
    ThresholdBase   = 0x090,

    PipeLineAddr    = 0x304, // broadcast, 
    BeamOnLengthAd  = 0x305, // broadcast, on spill gate length at 80 MHz
    BeamOffLengthAd = 0x306, // broadcast, off spill gate length at 80 MHz
    ADCSmplCntrAd   = 0x30c, // broadcast
    Inttrgenaddr    = 0x30e, // broadcast, Self Trigger Control Register 
    BrdCstRdPtrHiAd = 0x310,
    BrdCstRdPtrLoAd = 0x311,
    FEBAddresRegAd  = 0x314, // broadcast, port the feb is connected to
    CSRBroadCastAd  = 0x316, // broadcast, CSRc

    TRIG            = 0x900b, // TRIG
    RESET           = 0x9001,
    BROADCAST       = 0x2000
};
} // namespace CRVLib


#endif //CRVFEB_REGISTERS_H