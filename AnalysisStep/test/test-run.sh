if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./test-run.sh EVENTS"
    echo "  "
    exit -1
fi

export EVENTS=$1


#####
### MC example
#####

export MYFILE=file:/afs/cern.ch/user/a/amassiro/public/xLatinos/76X/CC3AD3BC-E898-E511-8707-0025904C5182.root

rm -rf latino_stepB_numEvent${EVENTS}.root

cmsRun stepB.py print                   \
    reportEvery=10                      \
    summary=false                       \
    is50ns=False                        \
    isPromptRecoData=True               \
    globalTag=76X_mcRun2_asymptotic_v12 \
    label=ggH                           \
    outputFile=stepB.root               \
    selection=LooseNoIso                \
    doNoFilter=False                    \
    doMuonIsoId=True                    \
    doEleIsoId=True                     \
    doPhotonID=True                     \
    doGen=True                          \
    doBTag=True                         \
    doFatJet=True                       \
    doLHE=True                          \
    doMCweights=True                    \
    maxEvents=${EVENTS}                 \
    inputFiles=${MYFILE}
#   LHEweightSource=externalLHEProducer \
                    
python cmssw2latino.py stepB_numEvent${EVENTS}.root

rm -rf stepB_numEvent${EVENTS}.root


#####
### Data example
#####

export MYFILE=file:/afs/cern.ch/work/p/piedra/public/store/data/Run2015D/DoubleMuon/MINIAOD/16Dec2015-v1/10000/00998A06-23A8-E511-902A-0025907B4F64.root

rm -rf latino_stepB_data_numEvent${EVENTS}.root

cmsRun stepB.py print                   \
    reportEvery=10                      \
    summary=false                       \
    is50ns=False                        \
    isPromptRecoData=True               \
    globalTag=76X_dataRun2_v15          \
    label=DoubleMuon                    \
    outputFile=stepB_data.root          \
    selection=LooseNoIso                \
    doNoFilter=False                    \
    doMuonIsoId=True                    \
    doEleIsoId=True                     \
    doPhotonID=True                     \
    doGen=False                         \
    doBTag=True                         \
    doFatJet=True                       \
    doLHE=False                         \
    maxEvents=${EVENTS}                 \
    inputFiles=${MYFILE}

### FIXME isPromptRecoData is a bad name. How about isRecoLabel?

python cmssw2latino.py stepB_data_numEvent${EVENTS}.root

rm -rf stepB_data_numEvent${EVENTS}.root

