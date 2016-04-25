if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./test-run-8x-data.sh EVENTS"
    echo "  "
    exit -1
fi

export EVENTS=$1

# dataset = /DoubleMuon/CMSSW_8_0_0-80X_dataRun2_relval_v0_RelVal_doubMu2015D-v1/MINIAOD
export MYFILE=root://xrootd.unl.edu//store/relval/CMSSW_8_0_0/DoubleMuon/MINIAOD/80X_dataRun2_relval_v0_RelVal_doubMu2015D-v1/10000/42F381A3-97DA-E511-9077-0CC47A4D76CC.root

rm -rf latino_stepB_data_numEvent${EVENTS}.root

cmsRun stepB.py print                   \
    reportEvery=10                      \
    summary=false                       \
    is50ns=False                        \
    isPromptRecoData=True               \
    globalTag=80X_mcRun2_asymptotic_v10 \
    label=DoubleMuon                    \
    outputFile=stepB_data.root          \
    selection=LooseNoIso                \
    doNoFilter=False                    \
    doMuonIsoId=True                    \
    doEleIsoId=True                     \
    doBTag=True                         \
    runPUPPISequence=True               \
    doPhotonID=True                     \
    maxEvents=${EVENTS}                 \
    inputFiles=${MYFILE}

python cmssw2latino.py stepB_data_numEvent${EVENTS}.root

rm -rf stepB_data_numEvent${EVENTS}.root
