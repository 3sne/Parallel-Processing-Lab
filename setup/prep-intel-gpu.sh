#!/bin/bash
echo; echo; echo
echo -e ' \t '@Author:   3sne ( Mukur Panchani )
echo -e ' \t '@File:     prep-intel-gpu.sh
echo -e ' \t '@Location: www.github.com/3sne/Parallel-Processing-Lab/setup/prep-intel-gpu.sh
echo; echo; echo
echo Start...
mkdir temp_xdr
cd temp_xdr
sudo apt install wget
wget https://github.com/intel/compute-runtime/releases/download/19.06.12357/intel-gmmlib_18.4.1_amd64.deb
wget https://github.com/intel/compute-runtime/releases/download/19.06.12357/intel-igc-core_18.50.1270_amd64.deb
wget https://github.com/intel/compute-runtime/releases/download/19.06.12357/intel-igc-opencl_18.50.1270_amd64.deb
wget https://github.com/intel/compute-runtime/releases/download/19.06.12357/intel-opencl_19.06.12357_amd64.deb
wget https://github.com/intel/compute-runtime/releases/download/19.06.12357/intel-ocloc_19.06.12357_amd64.deb
sudo dpkg -i *.deb
cd ..
sudo rm -R temp_xdr
sudo apt update
sudo apt install clinfo
clinfo
echo; echo; echo
echo -e ' \t '"***************************************"
echo -e ' \t '"*                                     *"
echo -e ' \t '"* Should be able to use intel gpu now *"
echo -e ' \t '"*                                     *"
echo -e ' \t '"***************************************"