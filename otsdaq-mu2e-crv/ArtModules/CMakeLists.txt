include(BuildPlugins) 

cet_build_plugin(CrvDQM art::module LIBRARIES REG
    art_root_io::TFileService_service
    artdaq_core_mu2e::artdaq-core-mu2e_Data
    otsdaq_mu2e::otsdaq-mu2e_ArtModules
    otsdaq::NetworkUtilities
    ROOT::Hist
    ROOT::Tree
    ROOT::Core
    ROOT::RIO
    ROOT::Gui
    ROOT::RHTTP 
)

install_headers(SUBDIRS detail)
install_source(SUBDIRS detail)
install_fhicl(SUBDIRS fcl)
