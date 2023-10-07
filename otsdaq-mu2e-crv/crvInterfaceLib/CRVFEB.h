#ifndef _ots_CRVFEB_h_
#define _ots_CRVFEB_h_

#include "dtcInterfaceLib/DTC.h"
#include "dtcInterfaceLib/DTC.h"

namespace CRVLib 
{
class CRVFEB 
{
public:
    CRVFEB(void);
    ~CRVFEB(void);

    void init(DTCLib::DTC*, DTCLib::DTC_Link_ID, unsigned int);
    void writeRegister(uint16_t address, uint16_t data_to_write);
    uint16_t readRegister(uint16_t address);

    void     SetTRIG(uint16_t data, bool broadcast = false); // uC TRIG command
    void     TakePedestrals(); 

    // feb register read and writes
    void     SetTestCounter(uint16_t);
    void     SetOnSpillGateLength(uint16_t length); // currently onl 12 bits used, at 80MHz
    void     SetOffSpillGateLength(uint16_t length); // currently onl 12 bits used, at 80MHz
    void     SetHitPipelineDelay(uint16_t delay); // efault 5, for trigger at 3d sample
    void     SetNumSamples(uint16_t num = 8); 
    void     SetPort(uint16_t port);
    void     SetBiasTrim(uint16_t ch, uint16_t fpga, uint16_t trim);
    void     SetBiasTrim(uint16_t ch, uint16_t trim);
    void     SetThreshold(uint16_t ch, uint16_t fpga, uint16_t th);
    void     SetThreshold(uint16_t ch, uint16_t th);
    void     SetBias(uint16_t no, uint16_t fpga, uint16_t bias);
    void     SetBias(uint16_t no, uint16_t bias);

    uint16_t ReadTestCounter();
    uint16_t GetOnSPillGateLength();
    uint16_t GetOffSPillGateLength();
    uint16_t GetHitPipelineDelay();
    uint16_t GetNumSamples();
    uint16_t GetPort();
    uint16_t GetBiasTrim(uint16_t ch, uint16_t fpga);
    uint16_t GetBiasTrim(uint16_t ch);
    uint16_t GetThreshold(uint16_t ch, uint16_t fpga);
    uint16_t GetThreshold(uint16_t ch);
    uint16_t GetBias(uint16_t no, uint16_t fpga);
    uint16_t GetBias(uint16_t no);

    void EnableSelfTrigger(); void DisableSelfTrigger();

    // Resets
    void Reset(); // uC
    void ResetDDR();

private:    
	DTCLib::DTC* 									thisDTC_;
    DTCLib::DTC_Link_ID 							linkID_;
    unsigned int   									tmo_ms_;


};
} // namespace CRVLib
#endif