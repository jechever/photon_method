cd ../Root 
source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/user/atlasLocalSetup.sh
lsetup ROOT
root -l -b -q 'GetBaseLineEvents.C+("410111","tt","/eos/atlas/atlascerngroupdisk/phys-susy/strong2L/v02-04/MC/","mm",false)' 