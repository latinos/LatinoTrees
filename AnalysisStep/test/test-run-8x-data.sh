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
#    'run2_data'         :   '80X_dataRun2_v17',
# 


# dataset=/MuonEG/Run2016C-23Sep2016-v1/MINIAOD
export MYFILE=root://cms-xrd-global.cern.ch//store/data/Run2016C/MuonEG/MINIAOD/03Feb2017-v1/110000/0AEA5448-35EC-E611-B1AF-A0369F310374.root


rm -rf latino_stepB_data_numEvent${EVENTS}.root

cmsRun stepB.py print          \
    doCorrectMet=True          \
    reportEvery=10             \
    summary=False              \
    is50ns=False               \
    isPromptRecoData=False      \
    globalTag=80X_dataRun2_2016SeptRepro_v7 \
    label=MuEG                 \
    outputFile=stepB_data.root \
    selection=LooseNoIso       \
    doNoFilter=False           \
    doMuonIsoId=True           \
    doEleIsoId=True            \
    doBTag=True                \
    runPUPPISequence=True      \
    doPhotonID=True            \
    maxEvents=${EVENTS}        \
    inputFiles=${MYFILE}

python cmssw2latino.py stepB_data_numEvent${EVENTS}.root

rm -rf stepB_data_numEvent${EVENTS}.root
