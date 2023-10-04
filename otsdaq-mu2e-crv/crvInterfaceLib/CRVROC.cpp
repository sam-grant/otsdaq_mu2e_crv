#include "otsdaq-mu2e-crv/crvInterfaceLib/CRVROC.h"
#include "otsdaq-mu2e-crv/crvInterfaceLib/CRVROC_Registers.h"

using namespace CRVLib;


//=========================================================================================
CRVROC::CRVROC()
{
}

//==========================================================================================
CRVROC::~CRVROC(void)
{
}

//=========================================================================================
void CRVROC::init(DTCLib::DTC* thisDTC, DTCLib::DTC_Link_ID linkID, unsigned int tmo_ms)
{
    thisDTC_ = thisDTC;
    linkID_  = linkID;
    tmo_ms_  = tmo_ms;
}

//=========================================================================================
uint16_t CRVROC::readRegister(uint16_t address)
{
    return thisDTC_->ReadROCRegister(linkID_, address, tmo_ms_);
}

//=========================================================================================
void CRVROC::writeRegister(uint16_t address, uint16_t data_to_write)
{
    thisDTC_->WriteROCRegister(linkID_, address, data_to_write,
								false, 0 //requestAck, ack_tmo_ms
								);
}

//============================ ROC Register Read and Writes ===============================
//=========================================================================================
void     CRVROC::SetTestCounter(uint16_t data) { writeRegister(ROC_Register::TestCounter, data); }
uint16_t CRVROC::ReadTestCounter() { return readRegister(ROC_Register::TestCounter); }