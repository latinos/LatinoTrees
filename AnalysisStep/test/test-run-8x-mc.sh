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


#export MYFILE=root://eoscms.cern.ch//store/mc/RunIISpring16MiniAODv2/TTTo2L2Nu_13TeV-powheg/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/60000/0005201C-D41B-E611-8E37-002481E0D398.root
export MYFILE=root://eoscms.cern.ch//store/mc/RunIISpring16MiniAODv2/GluGluToContinToZZTo4e_13TeV_MCFM701_pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/60000/3C5F44A8-D31B-E611-8707-0026B93F48E7.root

rm -rf latino_stepB_mc_numEvent${EVENTS}.root

cmsRun stepB.py print                   \
    reportEvery=10                      \
    summary=false                       \
    is50ns=False                        \
    isPromptRecoData=False              \
    globalTag=80X_mcRun2_asymptotic_2016_TrancheIV_v7 \
    label=ggZZ                          \
    outputFile=stepB_mc.root            \
    selection=LooseNoIso                \
    doNoFilter=False                    \
    doMuonIsoId=True                    \
    doEleIsoId=True                     \
    doBTag=True                         \
    runPUPPISequence=True               \
    doPhotonID=True                     \
    doGen=True                          \
    doLHE=True                          \
    doMCweights=True                    \
    maxEvents=${EVENTS}                 \
    inputFiles=${MYFILE}                \
    LHEweightSource=source              \
    LHERunInfo=""

python cmssw2latino.py stepB_mc_numEvent${EVENTS}.root

rm -rf stepB_mc_numEvent${EVENTS}.root
