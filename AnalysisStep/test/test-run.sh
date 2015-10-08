if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./test-run.sh EVENTS"
    echo "  "
    exit -1
fi

export EVENTS=$1

#export MYFILE=root://xrootd.unl.edu//store/data/Run2015D/DoubleMuon/MINIAOD/PromptReco-v4/000/258/159/00000/0C6D4AB0-6F6C-E511-8A64-02163E0133CD.root
export MYFILE=file:/afs/cern.ch/work/p/piedra/public/store/data/Run2015D/DoubleMuon/MINIAOD/PromptReco-v4/000/258/159/00000/28A6C3C3-6F6C-E511-94A4-02163E01459B.root


rm -rf latino_stepB_numEvent${EVENTS}.root

cmsRun stepB.py print         \
    is50ns=False              \
    isPromptRecoData=True     \
    globalTag=74X_dataRun2_Prompt_v2 \
    label=DoubleMuon          \
    outputFile=stepB.root     \
    selection=LooseNoIso      \
    doNoFilter=False          \
    doMuonIsoId=True          \
    doEleIsoId=True           \
    doGen=False               \
    doBTag=True               \
    doLHE=False               \
    runPUPPISequence=False    \
    maxEvents=${EVENTS}       \
    inputFiles=${MYFILE}

python cmssw2latino.py stepB_numEvent${EVENTS}.root

rm -rf stepB_numEvent${EVENTS}.root

