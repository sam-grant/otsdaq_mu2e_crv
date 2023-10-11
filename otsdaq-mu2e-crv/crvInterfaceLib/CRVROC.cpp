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
    // temporary fix for the current firmware version
    const unsigned int maxAttempts = 10;
    unsigned int wait = 100;
    
    for (unsigned int  attempt = 1; attempt <= maxAttempts; ++attempt) {
        try {
            //thisDTC_->GetDevice()->init(thisDTC_->ReadSimMode(), //simMode
            //                        0, // dtc, 
            //                        "", //simMemoryFile, 
            //                        thisDTC_->GetDevice()->getDeviceUID() // uid
            //                        );
                           
            return thisDTC_->ReadROCRegister(linkID_, address, tmo_ms_); // 
            break;
        } catch(const std::exception& e) {
            std::cerr << "Attempt " << attempt << ": " << e.what();;
            if (attempt < maxAttempts) {
                std::cerr << " Retrying after " << wait << " us" << std::endl;
                usleep(wait);
                wait *=2;
            } else {    
                std::cerr << " Maximum attempts reached. Exiting." << std::endl;
                throw e;
                break;
            }
        }
    }
    return -1;
    // alternative fix, the one above seems sufficient 
    //auto tempDTC = new DTCLib::DTC(thisDTC_->ReadSimMode(),  
    //                       0,//thisDTC_- dtc, 
    //                       0x1 //link_mask
    //                       );
    //return tempDTC->ReadROCRegister(linkID_, address, tmo_ms_);
    //delete tempDTC;
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

bool CRVROC::CheckLockFibers() {
    uint16_t data = readRegister(ROC_Register::GTPCSRAddr);
    return (data&0xff) == 0xcc;
}

/**
  * bit 0: Rx FIFO reset
  * bit 1: Link Tx Reset
  * bit 2: Phy Power Down
  * bit 3: FM Rx Enable
  * bit 4: MIG Reset
  * bit 5: DDR Write Seq. Enabled
  * bit 6: Phy Data Source
  * bit 7: DDR read seq.
  * bit 8: DDR write seq.
  * rest: not used
  * default data is 0xA8
  * fpga selects fpga 1, 2, or 3. If -1 (0xff) (default), broadcast to all
  */
void CRVROC::SetCSR(uint16_t data, uint8_t fpga) {
    if(fpga==0xff) writeRegister(ROC_Register::BROADCAST&ROC_Register::CSRRegAddr, data);
    else           writeRegister((static_cast<uint16_t>(fpga) << 10)&CSRRegAddr, data);
}

void CRVROC::ResetDDR(uint8_t fpga) {
    uint16_t addres = (fpga == 0xff) ? ROC_Register::BROADCAST : (static_cast<uint16_t>(fpga) << 10);
    writeRegister(addres|ROC_Register::SDRamWrtPtrHiAd, 0);
    writeRegister(addres|ROC_Register::SDRamWrtPtrLoAd, 0);
    writeRegister(addres|ROC_Register::SDRamRdPtrHiAd,  0);
    writeRegister(addres|ROC_Register::SDRamRdPtrLoAd,  0);
}


//============================ ROC Register Read and Writes ===============================
//=========================================================================================
void     CRVROC::SetTestCounter(uint16_t data) { writeRegister(ROC_Register::TestCounter, data); }
void     CRVROC::SetTRIG(uint16_t data) { writeRegister(ROC_Register::TRIG, data); }
uint16_t CRVROC::ReadTestCounter() { return readRegister(ROC_Register::TestCounter); }
uint32_t CRVROC::GetActivePorts() { 
    return (static_cast<uint32_t>(readRegister(ROC_Register::ActvRegAddrHi)) << 16) | 
            static_cast<uint32_t>(readRegister(ROC_Register::ActvRegAddrLo));
}
void     CRVROC::SetUBOffset(uint16_t offset) { writeRegister(ROC_Register::HrtBtOffsetAd, offset); }
uint16_t CRVROC::GetUBOffset() { return readRegister(ROC_Register::HrtBtOffsetAd); }
void CRVROC::EnableTDAQMarkerSync() {
    uint16_t data = readRegister(ROC_Register::CSRRegAddr);
    writeRegister(ROC_Register::CSRRegAddr, data|(1<<5)); // enable 
}
void CRVROC::DisableTDAQMarkerSync() {
    uint16_t data = readRegister(ROC_Register::CSRRegAddr);
    writeRegister(ROC_Register::CSRRegAddr, data|(~(1<<5))); // enable 
}
void CRVROC::EnablePLL() { writeRegister(ROC_Register::PLLPDnAddr, 0); } // 0x0001 power down
void CRVROC::DisablePLL() { writeRegister(ROC_Register::PLLPDnAddr, 1); } // 0x0001 power down

void CRVROC::EnableAvtiveFebTransmit(uint8_t fpga) {
    if (fpga == 0xff) writeRegister(ROC_Register::BROADCAST&ROC_Register::LinkCtrlAd, 1);
    else writeRegister((static_cast<uint16_t>(fpga) << 10)&ROC_Register::LinkCtrlAd, 1);
} 
void CRVROC::DisableAvtiveFebTransmit(uint8_t fpga) {
    if (fpga == 0xff) writeRegister(ROC_Register::BROADCAST&ROC_Register::LinkCtrlAd, 0);
    else writeRegister((static_cast<uint16_t>(fpga) << 10)&ROC_Register::LinkCtrlAd, 0);
} 

void CRVROC::Reset() { writeRegister(ROC_Register::RESET, 1); } // uC
void CRVROC::ResetGTPFIFO() { writeRegister(ROC_Register::GTPFIFOAddr, 1); }
void CRVROC::ResetLinkRxFIFO() { writeRegister(ROC_Register::LinkCSRAddr, 1); }

/**
 * power cycle the power to febs
 * fpga 1-24 individual feb
 * all febs for port 25, or port -1
 * this will take a while, a second maybe?
 */
void CRVROC::PowerResetPort(uint16_t port) {
    if (port == 0xffff) writeRegister(ROC_Register::BROADCAST&ROC_Register::PWRRST, 25);
    else writeRegister(ROC_Register::PWRRST, port);
}