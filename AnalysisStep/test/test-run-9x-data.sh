if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./test-run-8x-data.sh EVENTS"
    echo "  "
else

export EVENTS=$1


#
# GlobalTag choice
#
# cat /cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_17/src/Configuration/AlCa/python/autoCond.py | grep run2_data
#    'run2_data'         :   '80X_dataRun2_v17',
# 


# dataset=/MuonEG/Run2016C-23Sep2016-v1/MINIAOD
export MYFILE=root://cms-xrd-global.cern.ch//store/data/Run2017C/MuonEG/MINIAOD/17Nov2017-v1/50000/6C95A7DA-A1E1-E711-8A53-B499BAAC0658.root


rm -rf latino_stepB_data_numEvent${EVENTS}.root
rm -rf stepB_data_numEvent${EVENTS}.root

cmsRun stepB.py print          \
    doCorrectMet=True          \
    reportEvery=10             \
    summary=False              \
    is50ns=False               \
    isPromptRecoData=False      \
    globalTag=94X_dataRun2_ReReco_EOY17_v2 \
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
    inputFiles=${MYFILE}       \
    debug=5
python cmssw2latino.py stepB_data_numEvent${EVENTS}.root

#rm -rf stepB_data_numEvent${EVENTS}.root
fi
