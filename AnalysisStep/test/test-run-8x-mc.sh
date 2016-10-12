if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./test-run-8x-mc.sh EVENTS"
    echo "  "
    exit -1
fi

export EVENTS=$1


#
# GlobalTag choice
#
# cat /cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_5/src/Configuration/AlCa/python/autoCond.py | grep run2_mc
#    'run2_mc_50ns'      :   '80X_mcRun2_startup_v12',
#    'run2_mc'           :   '80X_mcRun2_asymptotic_v12',
#


# dataset = /RelValTTbar_13/CMSSW_8_0_5-PU25ns_80X_mcRun2_asymptotic_v12_gs7120p2-v1/MINIAODSIM
# export MYFILE=root://eoscms.cern.ch//store/relval/CMSSW_8_0_5/RelValTTbar_13/MINIAODSIM/PU25ns_80X_mcRun2_asymptotic_v12_gs7120p2-v1/00000/C8D488AF-F308-E611-8C6B-0025905A607E.root
# dataset = /WWTo2L2Nu_13TeV-powheg/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM
#export MYFILE=root://eoscms.cern.ch//store/mc/RunIISpring16MiniAODv1/WWTo2L2Nu_13TeV-powheg/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/50000/7A35BF96-9009-E611-A4DF-0090FAA57E94.root
export MYFILE=root://eoscms.cern.ch//store/mc/RunIISpring16MiniAODv2/TTTo2L2Nu_13TeV-powheg/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/60000/0005201C-D41B-E611-8E37-002481E0D398.root

rm -rf latino_stepB_mc_numEvent${EVENTS}.root

cmsRun stepB.py print                   \
    reportEvery=10                      \
    summary=false                       \
    is50ns=False                        \
    isPromptRecoData=False              \
    globalTag=80X_mcRun2_asymptotic_v12 \
    label=TTbar                         \
    outputFile=stepB_mc.root            \
    selection=LooseNoIso                \
    doNoFilter=False                    \
    doMuonIsoId=True                    \
    doEleIsoId=True                     \
    doBTag=True                         \
    runPUPPISequence=True               \
    doPhotonID=True                     \
    doGen=True                          \
    doLHE=False                         \
    doMCweights=False                   \
    maxEvents=${EVENTS}                 \
    inputFiles=${MYFILE}

python cmssw2latino.py stepB_mc_numEvent${EVENTS}.root

rm -rf stepB_mc_numEvent${EVENTS}.root
