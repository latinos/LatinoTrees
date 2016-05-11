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
# cat /cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_5/src/Configuration/AlCa/python/autoCond.py | grep run2_data
#    'run2_data'         :   '80X_dataRun2_v13',
#    'run2_data_relval'  :   '80X_dataRun2_relval_v9',
# 


# dataset = /DoubleMuon/CMSSW_8_0_5-80X_dataRun2_relval_v9_RelVal_doubMu2015D-v1/MINIAOD
export MYFILE=root://xrootd.unl.edu//store/relval/CMSSW_8_0_5/DoubleMuon/MINIAOD/80X_dataRun2_relval_v9_RelVal_doubMu2015D-v1/00000/C0DD3955-A108-E611-957B-0025905B858E.root


rm -rf latino_stepB_data_numEvent${EVENTS}.root

cmsRun stepB.py print                \
    reportEvery=10                   \
    summary=false                    \
    is50ns=False                     \
    isPromptRecoData=True            \
    globalTag=80X_dataRun2_relval_v9 \
    label=DoubleMuon                 \
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

