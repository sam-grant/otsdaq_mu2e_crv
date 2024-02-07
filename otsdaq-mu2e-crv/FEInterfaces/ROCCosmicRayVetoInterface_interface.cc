#include "otsdaq-mu2e-crv/FEInterfaces/ROCCosmicRayVetoInterface.h"
#include "otsdaq/Macros/InterfacePluginMacros.h"

using namespace ots;

#undef __MF_SUBJECT__
#define __MF_SUBJECT__ "FE-ROCCosmicRayVetoInterface"

//=========================================================================================
ROCCosmicRayVetoInterface::ROCCosmicRayVetoInterface(
    const std::string&       rocUID,
    const ConfigurationTree& theXDAQContextConfigTree,
    const std::string&       theConfigurationPath)
    : ROCCoreVInterface(rocUID, theXDAQContextConfigTree, theConfigurationPath)
{
	INIT_MF("." /*directory used is USER_DATA/LOG/.*/);

	__MCOUT_INFO__("ROCCosmicRayVetoInterface instantiated with link: "
	               << (int)linkID_ << " and EventWindowDelayOffset = " << delay_ << __E__);

    initRoc();
	initFeb();

	registerFEMacroFunction(
		"Do the CRV Dance",
			static_cast<FEVInterface::frontEndMacroFunction_t>(
					&ROCCosmicRayVetoInterface::DoTheCRV_Dance),
					std::vector<std::string>{"Which Step"},
					std::vector<std::string>{						
						"Random Result"},
					1);  // requiredUserPermissions
	registerFEMacroFunction(
		"Read Test Counter",
			static_cast<FEVInterface::frontEndMacroFunction_t>(
					&ROCCosmicRayVetoInterface::ReadTestCouter),
					std::vector<std::string>{"Test?"},
					std::vector<std::string>{						
						"Test Counter"},
					1);  // requiredUserPermissions
	registerFEMacroFunction(
		"Set Active Port",
			static_cast<FEVInterface::frontEndMacroFunction_t>(
					&ROCCosmicRayVetoInterface::SetActivePort),
					std::vector<std::string>{"port"},
					std::vector<std::string>{},
					1);  // requiredUserPermissions
	registerFEMacroFunction(
		"Get Active Port",
			static_cast<FEVInterface::frontEndMacroFunction_t>(
					&ROCCosmicRayVetoInterface::GetActivePort),
					std::vector<std::string>{},
					std::vector<std::string>{"Active Port"},
					255);  // requiredUserPermissions
	registerFEMacroFunction(
		"Get Active Ports",
			static_cast<FEVInterface::frontEndMacroFunction_t>(
					&ROCCosmicRayVetoInterface::GetActivePorts),
					std::vector<std::string>{},
					std::vector<std::string>{"Active Ports"},
					255);  // requiredUserPermissions
	registerFEMacroFunction(
		"Configure Hardware",
			static_cast<FEVInterface::frontEndMacroFunction_t>(
					&ROCCosmicRayVetoInterface::ConfigHardwareMacro),
					std::vector<std::string>{"Dry Run"},
					std::vector<std::string>{},
					255);  // requiredUserPermissions
		registerFEMacroFunction(
		"Power Reset FEB",
			static_cast<FEVInterface::frontEndMacroFunction_t>(
					&ROCCosmicRayVetoInterface::ConfigHardwareMacro),
					std::vector<std::string>{"port"},
					std::vector<std::string>{},
					255);  // requiredUserPermissions
	registerFEMacroFunction(
		"Set Bias",
			static_cast<FEVInterface::frontEndMacroFunction_t>(
					&ROCCosmicRayVetoInterface::SetBias),
					std::vector<std::string>{"port", "no", "bias"},
					std::vector<std::string>{},
					255);  // requiredUserPermissions
	registerFEMacroFunction(
		"Bias Off",
			static_cast<FEVInterface::frontEndMacroFunction_t>(
					&ROCCosmicRayVetoInterface::SetBiasOff),
					std::vector<std::string>{},
					std::vector<std::string>{},
					255);  // requiredUserPermissions

	registerFEMacroFunction(
		"Reset",
			static_cast<FEVInterface::frontEndMacroFunction_t>(
					&ROCCosmicRayVetoInterface::ResetUc),
					std::vector<std::string>{},
					std::vector<std::string>{},
					255);  // requiredUserPermissions
}

//==========================================================================================
ROCCosmicRayVetoInterface::~ROCCosmicRayVetoInterface(void)
{
	// NOTE:: be careful not to call __FE_COUT__ decoration because it uses the
	// tree and it may already be destructed partially
	__COUT__ << FEVInterface::interfaceUID_ << " Destructor" << __E__;
}

// //==================================================================================================
// void ROCCosmicRayVetoInterface::writeROCRegister(uint16_t address, uint16_t data_to_write)
// {
// 	__FE_COUT__ << "Calling write CRV-ROC register: link number " << std::dec << (int)linkID_
// 	            << ", address = " << address << ", write data = " << data_to_write
// 	            << __E__;

//     roc_.writeRegister(address, data_to_write);
// 	//thisDTC_->WriteROCRegister(linkID_, address, data_to_write,
// 	//							false, 0 //requestAck, ack_tmo_ms
// 	//							);

// 	return;
// }

// //==================================================================================================
// void ROCCosmicRayVetoInterface::universalRead(char* address, char* readValue) 
// {
// 	__FE_COUT__ << "CRV ROC universalRead: ";
// 	uint16_t data = roc_.readRegister(*address);
// 	__FE_COUT__ << data << __E__;
// 	readValue[0] = 0x12;
// 	readValue[1] = 0x34;
// }

// //==================================================================================================
// uint16_t ROCCosmicRayVetoInterface::readROCRegister(uint16_t address)
// {
// 	__FE_COUT__ << "Calling read CRV ROC register: link number " << std::dec << linkID_
// 	            << ", address = " << address << std::hex << " 0x" << address << __E__;

// 	return roc_.readRegister(address);
// 	//return thisDTC_->ReadROCRegister(linkID_, address, tmo_ms_);
// } 

//============================================================================================
void ROCCosmicRayVetoInterface::writeEmulatorRegister(uint16_t address,
                                                      uint16_t data_to_write)
{
	__FE_COUT__ << "Calling write CRV ROC Emulator register: link number " << std::dec
	            << (int)linkID_ << ", address = " << address
	            << ", write data = " << data_to_write << __E__;

	return;
}

//==================================================================================================
uint16_t ROCCosmicRayVetoInterface::readEmulatorRegister(uint16_t address)
{
	__FE_COUT__ << "Calling read CRV ROC Emulator register: link number " << std::dec
	            << (int)linkID_ << ", address = " << address << __E__;

	return -1;
}

//==================================================================================================
int ROCCosmicRayVetoInterface::readTimestamp() { return this->readRegister(12); }

//==================================================================================================
void ROCCosmicRayVetoInterface::writeDelay(uint16_t delay)
{
	this->writeRegister(21, delay);
	return;
}

//==================================================================================================
int ROCCosmicRayVetoInterface::readDelay() { return this->readRegister(7); }

//==================================================================================================
int ROCCosmicRayVetoInterface::readDTCLinkLossCounter() { return this->readRegister(8); }

//==================================================================================================
void ROCCosmicRayVetoInterface::resetDTCLinkLossCounter()
{
	this->writeRegister(24, 0x1);
	return;
}

//==================================================================================================
void ROCCosmicRayVetoInterface::initRoc() {
	roc_.init(thisDTC_, linkID_, tmo_ms_);
}

//==================================================================================================
void ROCCosmicRayVetoInterface::initFeb() {
	feb_.init(thisDTC_, linkID_, tmo_ms_);
}

//==================================================================================================
void ROCCosmicRayVetoInterface::configureHardware(bool dryRun) {
    bool noHardware = dryRun; // for debugging, supress writing to hardware

    // Test Fiber Connection
	if(!noHardware)
	{
		if(!roc_.CheckLockFibers()) {
			__FE_SS__ << "Error: fibers from the DTC are not locked." << __E__; // readout wouldn't work
			__FE_SS_THROW__;
		}
	}
 
	// Synchronize heart bears with timing on punched clocks 
	if(!noHardware) roc_.EnableTDAQMarkerSync();
    if(!noHardware) roc_.EnablePLL();
    if(!noHardware) roc_.SetTRIG(1);            // uC TRIG, Enable package forwarding to the FEB
	if(!noHardware) roc_.SetCSR();              // enable: FM Rx enable, DDR write seq. DDR read seq. 
	if(!noHardware) roc_.SetUBOffset(0xa);      // TODO, needs to be removed, set to one
	if(!noHardware) roc_.ResetGTPFIFO();        // Reset GTP FIFO"
    // ResetGTPFIFO()      // 
    
    // run start/end?
	if(!noHardware) roc_.ResetDDR();            // rocDdrReset
	if(!noHardware) roc_.ResetLinkRxFIFO();     // Reset link reciver FIFO
	if(!noHardware) roc_.EnableAvtiveFebTransmit(); // switch off for running? 

	bool doConfigureFEBs = true;
	try
	{
	 	doConfigureFEBs = Configurable::getSelfNode()
	 		                .getNode("EnableFEBConfigureStep")
	 		                .getValue<bool>();
	}
	catch(...)
	{
		__FE_COUT__ << "'EnableFEBConfigureStep' not found " << __E__;
	}  // ignore missing field
	__FE_COUT__ << "DEBUG Node Name" << getSelfNode().getUIDAsString() << __E__;
	if(doConfigureFEBs) 
	{
		// values to broadcast to all FEBs
		if(!noHardware) feb_.SetTRIG(0x0, true); // select source
		if(!noHardware) feb_.EnableSelfTrigger();
        if(!noHardware) feb_.SetNumSamples(8); // 8 samples is hard coded in the Offline code

		try {
			auto rocConfigs = getSelfNode().getNode("ROCTypeLinkTable") // should this be renamed to LinkToROCTypeTable?
				.getNode("LinkToSubsystemGroupedParametersTable").getChildren();
			{
				for(const auto& rocConfig : rocConfigs)
				{
					if(rocConfig.second.getNode("Name").getValueAsString() == "OnSpillGateLength") {
						uint16_t onSpillGateLength = rocConfig.second.getNode("Value").getValue<uint16_t>();
						if(!noHardware) feb_.SetOnSpillGateLength(onSpillGateLength);
						__FE_COUT__ << "Broadcast 'onSpillGateLength' 0x" << std::hex << onSpillGateLength << " to all FEBs " << __E__;
						continue;	
					}
					if(rocConfig.second.getNode("Name").getValueAsString() == "OffSpillGateLength") {
						uint16_t offSpillGateLength = rocConfig.second.getNode("Value").getValue<uint16_t>();
						if(!noHardware) feb_.SetOffSpillGateLength(offSpillGateLength);
						__FE_COUT__ << "Broadcast 'offSpillGateLength' 0x" << std::hex << offSpillGateLength << " to all FEBs " << __E__;
						continue;	
					}
					if(rocConfig.second.getNode("Name").getValueAsString() == "HitPipelineDelay") {
						uint16_t hitPipelineDelay = rocConfig.second.getNode("Value").getValue<uint16_t>();
						if(!noHardware) feb_.SetHitPipelineDelay(hitPipelineDelay);
						__FE_COUT__ << "Broadcast 'hitPipelineDelay' 0x" << std::hex << hitPipelineDelay << " to all FEBs " << __E__;
						continue;	
					}
				}
			}
		} catch(...) {	
			TLOG(TLVL_WARNING) << "Missing 'ROCTypeLinkTable/LinkToSubsystemGroupedParametersTable', skipping broadcasted FEB config." << __E__;
		}

		// loop through all FEBs for individual configuration
		auto febs = getSelfNode().getNode("LinkToFEBInterfaceTable").getChildren();
		for(const auto& feb : febs) {
			bool active = feb.second.getNode("Status").getValue<bool>();
			if(active) 
			{
				uint16_t port = feb.second.getNode("Port").getValue<uint16_t>();
				__FE_COUT__ << "Configure FEB " << feb.first << " on port " << std::to_string(port) << __E__;
				usleep(1000); // TODO, check if for write we might also need a sleep in between?
				if(!noHardware) roc_.SetActivePort(port);
				if(!noHardware) feb_.SetPort(port);
				if(!noHardware) feb_.TakePedestrals(); // better at start of run?
				//feb_.ResetDDR() // probabl not needed?
                
				// loop through channels
				if(!feb.second.getNode("LinkToCRVChannelTable").isDisconnected())
				{
					for(const auto& ch : feb.second.getNode("LinkToCRVChannelTable").getChildren()) 
					{
						uint16_t channel   = ch.second.getNode("Channel").getValue<uint16_t>();
						uint16_t biasTrim  = ch.second.getNode("BiasTrim").getValue<uint16_t>();
						uint16_t threshold = ch.second.getNode("Threshold").getValue<uint16_t>();
						__FE_COUT__ << "Configure channel " << ch.first 
						            << " ch: " << channel
									<< ", trim: 0x" << std::hex << biasTrim
									<< ", threshold: 0x" << std::hex << threshold
									<< __E__;
						if(!noHardware) feb_.SetBiasTrim(channel, biasTrim);
						if(!noHardware) feb_.SetThreshold(channel, threshold);
					}
				} else {
					TLOG(TLVL_WARNING) << "Missing 'LinkToCRVChannelTable' link, skipping channel config." << __E__;
				}

				// loop through channel groups
				if(!feb.second.getNode("LinkToCRVChannelGroupTable").isDisconnected())
				{
					for(const auto& chg : feb.second.getNode("LinkToCRVChannelGroupTable").getChildren())
					{
						uint16_t number   = chg.second.getNode("Number").getValue<uint16_t>();
						uint16_t bias  = chg.second.getNode("Bias").getValue<uint16_t>();
						__FE_COUT__ << "Configure channel group " << chg.first 
						            << " number: " << number
									<< ", bias: 0x" << std::hex << bias
									<< __E__;
						if(!noHardware) feb_.SetBias(number, bias);
					}
				} else {
					TLOG(TLVL_WARNING) << "Missing 'LinkToCRVChannelGroupTable' link, skipping channel group config." << __E__;
				}
			} else 
			{
				__FE_COUT__ << "Skip configuration of " << feb.first << __E__;
			}
		}
	} else {
		__FE_COUT_INFO__ << "Skip FEB(s) configuration." << __E__;
	}

}

//==================================================================================================
void ROCCosmicRayVetoInterface::configure(void) try
{
	//__MCOUT_INFO__(".... do nothing for CRV ROC... " << __E__);
	__FE_COUT__ << ".... do almost nothing for CRV ROC... " << __E__;
	configureROCDCS(); // used in config of the base class
	initRoc(); // load new settings to ROC
	initFeb();

	configureHardware(true); // for the moment, don't send commands out
}
catch(const std::runtime_error& e)
{
	__FE_MOUT__ << "Error caught: " << e.what() << __E__;
	throw;
}
catch(...)
{
	__FE_SS__ << "Unknown error caught. Check printouts!" << __E__;
	__FE_MOUT__ << ss.str();
	__FE_SS_THROW__;
}

//==============================================================================
void ROCCosmicRayVetoInterface::halt(void) {}

//==============================================================================
void ROCCosmicRayVetoInterface::pause(void) {}

//==============================================================================
void ROCCosmicRayVetoInterface::resume(void) {}

//==============================================================================
void ROCCosmicRayVetoInterface::start(std::string)  // runNumber)
{
	__FE_COUT__ << "DEBUG ROC Run Start" << __E__;
	configureHardware(false); // to test ROC & FEB hardware config
	// roc_.DisableAvtiveFebTransmit();
}

//==============================================================================
void ROCCosmicRayVetoInterface::stop(void) {
	roc_.EnableAvtiveFebTransmit();
}

//==============================================================================
bool ROCCosmicRayVetoInterface::running(void) { return false; }



//========================================================================
void ROCCosmicRayVetoInterface::DoTheCRV_Dance(__ARGS__)
{	
//	uint32_t address = __GET_ARG_IN__("Which Step", uint32_t);
	__FE_COUT__ << "Hello" << __E__;
	__SET_ARG_OUT__("Random Result",0xA4);
	
} //end DoTheCRV_Dance()

void ROCCosmicRayVetoInterface::ReadTestCouter(__ARGS__)
{
	__FE_COUT__ << "CRV ROC ReadTestCouter" << __E__;
	__SET_ARG_OUT__("Test Counter", roc_.ReadTestCounter());
}

void ROCCosmicRayVetoInterface::SetActivePort(__ARGS__)
{
	uint16_t port  = __GET_ARG_IN__("port", uint16_t);
	__FE_COUT__ << "CRV ROC SetActivePort: " << port << __E__;
	roc_.SetActivePort(port, true);
}

void ROCCosmicRayVetoInterface::GetActivePort(__ARGS__)
{
	__FE_COUT__ << "CRV ROC GetActivePort" << __E__;
	__SET_ARG_OUT__("Active Port", roc_.GetActivePort());
}

void ROCCosmicRayVetoInterface::GetActivePorts(__ARGS__)
{
	__FE_COUT__ << "CRV ROC GetActivePorts" << __E__;
	__SET_ARG_OUT__("Active Ports", roc_.GetActivePorts());
}

void ROCCosmicRayVetoInterface::ConfigHardwareMacro(__ARGS__)
{
	bool dryrun  = __GET_ARG_IN__("Dry Run", bool);
	__FE_COUT__ << "CRV ROC GetActivePorts" << __E__;
	configureHardware(dryrun);
}

void ROCCosmicRayVetoInterface::SetBias(__ARGS__)
{
	uint16_t port  = __GET_ARG_IN__("port", uint16_t);
	uint16_t no    = __GET_ARG_IN__("no", uint16_t);
	uint16_t bias  = __GET_ARG_IN__("bias", uint16_t);
	__FE_COUT__ << "CRV ROC SetBias for port " << port << 
	               ", number " << port <<
				   ", bias 0x" << std::hex << bias << __E__;
	roc_.SetActivePort(port);
	feb_.SetBias(no, bias);
}

void ROCCosmicRayVetoInterface::SetBiasOff(__ARGS__)
{
	__FE_COUT__ << "CRV ROC SetBiasOff" << __E__;
    uint32_t active = roc_.GetActivePorts();
	for(uint32_t port=1; port <=24; ++port) {
		if((1<<(port-1))&active) {
			roc_.SetActivePort(port);
	
			for(uint8_t no = 0; no < (2*4); ++no) {
				feb_.SetBias(no, 0x0);
			}
		}
	}
}

void ROCCosmicRayVetoInterface::PowerResetPort(__ARGS__)
{
	uint16_t port  = __GET_ARG_IN__("port", uint16_t);	
	__FE_COUT__ << "CRV ROC PowerResetPort" << __E__;
	roc_.PowerResetPort(port);
}

void ROCCosmicRayVetoInterface::ResetUc(__ARGS__)
{	
	__FE_COUT__ << "CRV ROC Reset" << __E__;
	roc_.Reset();
}

DEFINE_OTS_INTERFACE(ROCCosmicRayVetoInterface)
