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
    thisDTC_->GetDevice()->init(thisDTC_->ReadSimMode(), //simMode
                                0, // dtc, 
                                "", //simMemoryFile, 
                                thisDTC_->GetDevice()->getDeviceUID() // uid
                                );
    return thisDTC_->ReadROCRegister(linkID_, address|0x1000, tmo_ms_);
}

//=========================================================================================
void CRVFEB::writeRegister(uint16_t address, uint16_t data_to_write)
{
    thisDTC_->WriteROCRegister(linkID_, address|0x1000, data_to_write,
								false, 0 //requestAck, ack_tmo_ms
								);
}
void CRVFEB::TakePedestrals() { writeRegister(FEB_Register::CSRBroadCastAd, 0x200); }

void CRVFEB::ResetDDR() {
    writeRegister(FEB_Register::BrdCstRdPtrHiAd, 0);
    writeRegister(FEB_Register::BrdCstRdPtrLoAd, 0);
}

//============================ ROC Register Read and Writes ===============================
//=========================================================================================
void     CRVFEB::SetTestCounter(uint16_t data) { writeRegister(FEB_Register::TestCounter, data); }
uint16_t CRVFEB::ReadTestCounter() { return readRegister(FEB_Register::TestCounter); }
void     CRVFEB::SetTRIG(uint16_t data, bool broadcast) { 
    writeRegister(FEB_Register::TRIG|(broadcast ? FEB_Register::BROADCAST : 0x0000), data); 
}

void CRVFEB::EnableSelfTrigger() { writeRegister(FEB_Register::Inttrgenaddr, 0x2); }
void CRVFEB::DisableSelfTrigger() { writeRegister(FEB_Register::Inttrgenaddr, 0x0); }

void CRVFEB::SetOnSpillGateLength(uint16_t length) {
    writeRegister(FEB_Register::BeamOnLengthAd, length); // currnelty only 12 bits are used
}
void CRVFEB::SetOffSpillGateLength(uint16_t length) {
    writeRegister(FEB_Register::BeamOffLengthAd, length); // currnelty only 12 bits are used
}
void CRVFEB::SetHitPipelineDelay(uint16_t delay) {
    writeRegister(FEB_Register::PipeLineAddr, delay); 
}
void CRVFEB::SetNumSamples(uint16_t num) {
    writeRegister(FEB_Register::ADCSmplCntrAd, num); 
}
void CRVFEB::SetPort(uint16_t port) {
    writeRegister(FEB_Register::FEBAddresRegAd, port); 
}
uint16_t CRVFEB::GetOnSPillGateLength() { return readRegister(FEB_Register::BeamOnLengthAd); }
uint16_t CRVFEB::GetOffSPillGateLength() { return readRegister(FEB_Register::BeamOffLengthAd); }
uint16_t CRVFEB::GetHitPipelineDelay() { return readRegister(FEB_Register::PipeLineAddr); }
uint16_t CRVFEB::GetNumSamples() { return readRegister(FEB_Register::ADCSmplCntrAd); }
uint16_t CRVFEB::GetPort() { return readRegister(FEB_Register::FEBAddresRegAd); }


void CRVFEB::SetBiasTrim(uint16_t ch, uint16_t fpga, uint16_t trim) {
    writeRegister((FEB_Register::BiasTrimBase + (ch&0xf))|((fpga&0x3)<<10), trim);
}
void CRVFEB::SetBiasTrim(uint16_t ch, uint16_t trim) { SetBiasTrim(ch&0xf, (ch>>4), trim); }
void CRVFEB::SetThreshold(uint16_t ch, uint16_t fpga, uint16_t th) {
    writeRegister((FEB_Register::ThresholdBase + (ch&0xf))|((fpga&0x3)<<10), th);
}
void CRVFEB::SetThreshold(uint16_t ch, uint16_t th) { SetThreshold(ch&0xf, (ch>>4), th); }
void CRVFEB::SetBias(uint16_t no, uint16_t fpga, uint16_t bias) {
    writeRegister((FEB_Register::BiasBase + (no&0x1))|((fpga&0x3)<<10), bias);
}
void CRVFEB::SetBias(uint16_t no, uint16_t bias) { SetBias(no&0x1, (no>>1), bias); }

uint16_t CRVFEB::GetBiasTrim(uint16_t ch, uint16_t fpga) {
    return readRegister((FEB_Register::BiasTrimBase + (ch&0xf))|((fpga&0x3)<<10));
}
uint16_t CRVFEB::GetBiasTrim(uint16_t ch) { return GetBiasTrim(ch&0xf, (ch>>4)); }
uint16_t CRVFEB::GetThreshold(uint16_t ch, uint16_t fpga) {
    return readRegister((FEB_Register::ThresholdBase + (ch&0xf))|((fpga&0x3)<<10));
}
uint16_t CRVFEB::GetThreshold(uint16_t ch) { return GetThreshold(ch&0xf, (ch>>4)); }
uint16_t CRVFEB::GetBias(uint16_t no, uint16_t fpga) {
    return readRegister((FEB_Register::BiasBase + (no&0x1))|((fpga&0x3)<<10));
}
uint16_t CRVFEB::GetBias(uint16_t no) { return GetBias(no&0x1, (no>>1)); }

void CRVFEB::Reset() { writeRegister(FEB_Register::RESET, 0x1); }
