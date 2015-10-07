if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./test-run.sh EVENTS"
    echo "  "
    exit -1
fi

export EVENTS=$1

export MYFILE=root://xrootd.unl.edu//store/mc/RunIISpring15DR74/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/60000/008E7FBF-9218-E511-81E0-001E675A5244.root

### rm -rf latino_stepB_numEvent${EVENTS}.root

cmsRun stepB.py print         \
    is50ns=False              \
    isPromptRecoData=False    \
    globalTag=74X_mcRun2_asymptotic_v2 \
    label=WZ                  \
    outputFile=stepB.root     \
    selection=LooseNoIso      \
    doNoFilter=True           \
    doMuonIsoId=True          \
    doEleIsoId=True           \
    doGen=False               \
    doBTag=True               \
    doLHE=False               \
    runPUPPISequence=True     \
    maxEvents=${EVENTS}       \
    inputFiles=${MYFILE}


# From https://hypernews.cern.ch/HyperNews/CMS/get/JetMET/1630.html
#
# The relevant GTs are:
#
#  for 50ns MC with startup conditions:          74X_mcRun2_startup_v2
#  for 25ns MC with design conditions:           74X_mcRun2_design_v2
#  for 25ns MC with 25ns data-taking conditions: 74X_mcRun2_asymptotic_v2
#  for data:                                     74X_dataRun2_v2 


### python cmssw2latino.py stepB_numEvent${EVENTS}.root

### rm -rf stepB_numEvent${EVENTS}.root

