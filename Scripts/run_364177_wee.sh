cd ../Root 
source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/user/atlasLocalSetup.sh
lsetup ROOT
root -l -b -q 'GetBaseLineEvents.C+("364177","wjets","/eos/atlas/atlascerngroupdisk/phys-susy/strong2L/v02-04/MC/","ee",false)' 
