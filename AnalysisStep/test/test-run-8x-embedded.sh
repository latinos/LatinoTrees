# voms-proxy-init -voms cms

if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./test-run-8x-embedded.sh EVENTS"
    echo "  "
    exit -1
fi

export EVENTS=$1


MYFILE='root://cms-xrd-global.cern.ch//store/user/jbechtel/gc_storage/ElMu_data_2016_CMSSW826_freiburg/TauEmbedding_ElMu_data_2016_CMSSW826_Run2016B/1/merged_2000.root'


rm -rf latino_stepB_embedded_numEvent${EVENTS}.root

cmsRun stepB.py print                       \
    reportEvery=10                          \
    summary=False                           \
    is50ns=False                            \
    doTauEmbed=True                         \
    isPromptRecoData=False                  \
    globalTag=80X_dataRun2_2016SeptRepro_v7 \
    label=DYtautau_embedded                 \
    outputFile=stepB_embedded.root          \
    selection=LooseNoIso                    \
    doNoFilter=False                        \
    doMuonIsoId=True                        \
    doEleIsoId=True                         \
    doBTag=True                             \
    runPUPPISequence=True                   \
    doPhotonID=True                         \
    maxEvents=${EVENTS}                     \
    inputFiles=${MYFILE}                

python cmssw2latino.py stepB_embedded_numEvent${EVENTS}.root

rm -rf stepB_embedded_numEvent${EVENTS}.root






