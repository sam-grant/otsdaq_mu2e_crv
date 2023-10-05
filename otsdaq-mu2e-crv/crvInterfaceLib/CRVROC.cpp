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

//============================ Functions =================================================
/**
 * Select an active port connecting to a FEB
 * @param port Port number, we use 1-24, 0 is not valid.
 * @param check if true the function checks if the selected port is active and if the 
 *        activation was sucessful
 * @throw Exception if the checks fail
*/
void CRVROC::SetActivePort(uint16_t port, bool check) {
    if(check)
    {
        uint32_t active = GetActivePorts();
        if( !(active & (0x00000001<<(port-1))) ) // through exception if selected port is not activr
        {
            std::stringstream ss;
            ss << "Error selecting port " << +port << ", port is not active: 0x" << std::hex << active;
            throw std::runtime_error(ss.str());
            //__FE_SS__ << "Error selecting port " << +port << ", port is not active: 0x" << std::hex << active;
		    //__SS_THROW__;
        }
    }
    writeRegister(ROC_Register::LP, port);

    //if(check)
    //{
    //    uint16_t ap = GetActivePort();
    //    if(port != ap)
    //    {
    //        std::stringstream ss;
    //        ss << "Error selecting port. Failed to activate port " << port << ". After the activation, port " <<  ap << " is active.";
    //        throw std::runtime_error(ss.str());
    //        //__FE_SS__ << "Error selecting port. Failed to activate port " << port << ". After the activation, port " <<  ap << " is active.";
	//	    //__SS_THROW__;
    //    }
    //}
}

uint16_t CRVROC::GetActivePort() {
    return readRegister(ROC_Register::LP);
}

//============================ ROC Register Read and Writes ===============================
//=========================================================================================
void     CRVROC::SetTestCounter(uint16_t data) { writeRegister(ROC_Register::TestCounter, data); }
uint16_t CRVROC::ReadTestCounter() { return readRegister(ROC_Register::TestCounter); }
uint32_t CRVROC::GetActivePorts() { 
    return (static_cast<uint32_t>(readRegister(ROC_Register::ActvRegAddrHi)) << 16) | 
            static_cast<uint32_t>(readRegister(ROC_Register::ActvRegAddrLo));
}