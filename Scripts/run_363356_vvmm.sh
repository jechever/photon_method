cd ../Root 
source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/user/atlasLocalSetup.sh
lsetup ROOT
root -l -b -q 'GetBaseLineEvents.C+("363356","vv","/eos/atlas/atlascerngroupdisk/phys-susy/strong2L/v02-04/MC/","mm",false)' 
