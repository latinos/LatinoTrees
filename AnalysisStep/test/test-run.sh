if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./test-run.sh EVENTS"
    echo "  "
    exit -1
fi

export EVENTS=$1
#export MYFILE=file:/afs/cern.ch/user/p/piedra/work/store/mc/RunIISpring15DR74/TT_TuneCUETP8M1_13TeV-powheg-pythia8/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v1/50000/68EBC80B-4CFF-E411-8D52-00074305CFFB.root
#export MYFILE=root://xrootd.unl.edu//store/data/Run2015B/DoubleEG/MINIAOD/17Jul2015-v1/40000/02FC1E69-AF2E-E511-ABA6-0025905B858E.root
#export MYFILE=root://xrootd.unl.edu//store/data/Run2015C/SingleElectron/MINIAOD/PromptReco-v1/000/254/307/00000/624082FC-7A45-E511-B404-02163E01246F.root
export MYFILE=root://xrootd.unl.edu//store/data/Run2015C/DoubleMuon/MINIAOD/PromptReco-v1/000/254/231/00000/A8A4FD2D-F645-E511-A2CF-02163E0135AD.root

rm -rf latino_stepB_numEvent${EVENTS}.root

cmsRun stepB.py print         \
    is50ns=True               \
    isPromptRecoData=False    \
    globalTag=74X_dataRun2_v2 \
    label=DoubleMuon          \
    outputFile=stepB.root     \
    selection=LooseNoIso      \
    doNoFilter=False          \
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


python cmssw2latino.py stepB_numEvent${EVENTS}.root

rm -rf stepB_numEvent${EVENTS}.root

