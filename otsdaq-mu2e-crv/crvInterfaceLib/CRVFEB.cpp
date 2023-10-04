#include "otsdaq-mu2e-crv/crvInterfaceLib/CRVFEB.h"
#include "otsdaq-mu2e-crv/crvInterfaceLib/CRVFEB_Registers.h"

using namespace CRVLib;


//=========================================================================================
CRVFEB::CRVFEB()
{
}

//==========================================================================================
CRVFEB::~CRVFEB(void)
{
}

//=========================================================================================
void CRVFEB::init(DTCLib::DTC* thisDTC, DTCLib::DTC_Link_ID linkID, unsigned int tmo_ms)
{
    thisDTC_ = thisDTC;
    linkID_  = linkID;
    tmo_ms_  = tmo_ms;
}

//=========================================================================================
uint16_t CRVFEB::readRegister(uint16_t address)
{
    return thisDTC_->ReadROCRegister(linkID_, address|0x1000, tmo_ms_);
}

//=========================================================================================
void CRVFEB::writeRegister(uint16_t address, uint16_t data_to_write)
{
    thisDTC_->WriteROCRegister(linkID_, address|0x1000, data_to_write,
								false, 0 //requestAck, ack_tmo_ms
								);
}

//============================ ROC Register Read and Writes ===============================
//=========================================================================================
void     CRVFEB::SetTestCounter(uint16_t data) { writeRegister(FEB_Register::TestCounter, data); }
uint16_t CRVFEB::ReadTestCounter() { return readRegister(FEB_Register::TestCounter); }