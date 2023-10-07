
cet_make_library(LIBRARY_NAME CRVInterface SOURCE
            CRVROC.cpp
            CRVFEB.cpp
            LIBRARIES mu2e_pcie_utils::DTCInterface
)


install_headers()
install_source()