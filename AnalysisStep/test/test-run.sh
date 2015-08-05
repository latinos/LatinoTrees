if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./test-run.sh EVENTS"
    echo "  "
    exit -1
fi

export EVENTS=$1
export MYFILE=root://xrootd.unl.edu//store/data/Run2015B/DoubleEG/MINIAOD/17Jul2015-v1/40000/02FC1E69-AF2E-E511-ABA6-0025905B858E.root

rm -rf latino_stepB_numEvent${EVENTS}.root

cmsRun stepB.py print     \
    globalTag=GR_P_V56    \
    label=DoubleEG        \
    outputFile=stepB.root \
    selection=LooseNoIso  \
    doNoFilter=False      \
    doMuonIsoId=True      \
    doEleIsoId=True       \
    doGen=False           \
    doBTag=True           \
    doLHE=False           \
    runPUPPISequence=True \
    maxEvents=${EVENTS}   \
    inputFiles=${MYFILE}

python cmssw2latino.py stepB_numEvent${EVENTS}.root

rm -rf stepB_numEvent${EVENTS}.root

