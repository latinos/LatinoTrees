if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./test-run.sh EVENTS"
    echo "  "
    exit -1
fi

export EVENTS=$1
export MYFILE=root://xrootd.unl.edu//store/data/Run2015B/DoubleEG/MINIAOD/17Jul2015-v1/40000/02FC1E69-AF2E-E511-ABA6-0025905B858E.root
export MYFILE=file:/afs/cern.ch/user/p/piedra/work/store/mc/RunIISpring15DR74/TT_TuneCUETP8M1_13TeV-powheg-pythia8/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v1/50000/68EBC80B-4CFF-E411-8D52-00074305CFFB.root

rm -rf latino_stepB_numEvent${EVENTS}.root

cmsRun stepB.py print       \
    globalTag=MCRUN2_74_V9A \
    label=TT                \
    outputFile=stepB.root   \
    selection=LooseNoIso    \
    doNoFilter=False        \
    doMuonIsoId=True        \
    doEleIsoId=True         \
    doGen=False             \
    doBTag=True             \
    doLHE=False             \
    runPUPPISequence=False  \
    maxEvents=${EVENTS}     \
    inputFiles=${MYFILE}

# For DoubleEG data
#   globalTag=GR_P_V56 \
#   label=DoubleEG     \

python cmssw2latino.py stepB_numEvent${EVENTS}.root

rm -rf stepB_numEvent${EVENTS}.root

