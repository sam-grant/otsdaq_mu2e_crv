#ifndef _ots_CRVROC_h_
#define _ots_CRVROC_h_

#include "dtcInterfaceLib/DTC.h"
#include "dtcInterfaceLib/DTC.h"

namespace CRVLib 
{
class CRVROC 
{
public:
    CRVROC(void);
    ~CRVROC(void);

    void init(DTCLib::DTC*, DTCLib::DTC_Link_ID, unsigned int);
    void writeRegister(uint16_t address, uint16_t data_to_write);
    uint16_t readRegister(uint16_t address);

    // roc register read and writes
    void     SetTestCounter(uint16_t);
    uint16_t ReadTestCounter();

private:    
	DTCLib::DTC* 									thisDTC_;
    DTCLib::DTC_Link_ID 							linkID_;
    unsigned int   									tmo_ms_;


};
} // namespace CRVLib
#endif