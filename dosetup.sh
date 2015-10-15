export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/ 
source $ATLAS_LOCAL_ROOT_BASE/user/atlasLocalSetup.sh
echo Setting up ROOT v5.34.12, gcc4.7
localSetupROOT --rootVersion=5.34.12-x86_64-slc6-gcc4.7 
