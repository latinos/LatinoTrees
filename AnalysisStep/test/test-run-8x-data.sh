if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./test-run-8x-data.sh EVENTS"
    echo "  "
    exit -1
fi

export EVENTS=$1


#
# GlobalTag choice
#
# cat /cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_17/src/Configuration/AlCa/python/autoCond.py | grep run2_data
#    'run2_data'         :   '80X_dataRun2_v13',
#    'run2_data_relval'  :   '80X_dataRun2_relval_v9',
# 


# dataset=/MuonEG/Run2016E-PromptReco-v2/MINIAOD
export MYFILE=root://eoscms.cern.ch//store/data/Run2016E/MuonEG/MINIAOD/PromptReco-v2/000/276/831/00000/4656561B-084D-E611-86C4-02163E011E0F.root


rm -rf latino_stepB_data_numEvent${EVENTS}.root

cmsRun stepB.py print                \
    doCorrectMet=False               \
    reportEvery=10                   \
    summary=False                    \
    is50ns=False                     \
    isPromptRecoData=True            \
    globalTag=80X_dataRun2_v17       \
    label=MuEG                       \
    outputFile=stepB_data.root       \
    selection=LooseNoIso             \
    doNoFilter=False                 \
    doMuonIsoId=True                 \
    doEleIsoId=True                  \
    doBTag=True                      \
    runPUPPISequence=True            \
    doPhotonID=True                  \
    maxEvents=${EVENTS}              \
    inputFiles=${MYFILE}

python cmssw2latino.py stepB_data_numEvent${EVENTS}.root

rm -rf stepB_data_numEvent${EVENTS}.root
