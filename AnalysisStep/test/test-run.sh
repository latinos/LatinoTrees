if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./test-run.sh EVENTS"
    echo "  "
    exit -1
fi

export EVENTS=$1

#export MYFILE=root://xrootd.unl.edu//store/data/Run2015D/DoubleMuon/MINIAOD/PromptReco-v4/000/258/159/00000/0C6D4AB0-6F6C-E511-8A64-02163E0133CD.root
# export MYFILE=file:/afs/cern.ch/work/p/piedra/public/store/data/Run2015D/DoubleMuon/MINIAOD/PromptReco-v4/000/258/159/00000/28A6C3C3-6F6C-E511-94A4-02163E01459B.root
#export MYFILE=file:/afs/cern.ch/work/p/piedra/public/store/data/Run2015D/DoubleEG/MINIAOD/PromptReco-v4/000/258/175/00000/FA1AB2EB-436D-E511-8DF2-02163E011E2B.root

# export MYFILE=file:/afs/cern.ch/user/a/amassiro/public/xLatinos/76X/CC3AD3BC-E898-E511-8707-0025904C5182.root
# 
# 
# rm -rf latino_stepB_numEvent${EVENTS}.root
# 
# cmsRun stepB.py print                   \
#     is50ns=False                        \
#     isPromptRecoData=False              \
#     globalTag=76X_mcRun2_asymptotic_v12 \
#     label=ggH                           \
#     outputFile=stepB.root               \
#     selection=LooseNoIso                \
#     doNoFilter=False                    \
#     doMuonIsoId=True                    \
#     doEleIsoId=True                     \
#     doPhotonID=True                     \
#     doGen=True                          \
#     doBTag=True                         \
#     doFatJet=True                       \
#     doLHE=True                          \
#     doMCweights=True                    \
#     maxEvents=${EVENTS}                 \
#     inputFiles=${MYFILE}
# 
#     
# # # #                     doNoFilter=True \
# #                     LHEweightSource=externalLHEProducer \
#                     
# python cmssw2latino.py stepB_numEvent${EVENTS}.root
# 
# rm -rf stepB_numEvent${EVENTS}.root


#####
### a data example
#####

export MYFILE=file:/afs/cern.ch/user/a/amassiro/public/xLatinos/76X/DATA_MuEG_1ED2F05D-E8AF-E511-BDF3-0002C94D575E.root

rm -rf latino_stepB_data_numEvent${EVENTS}.root

cmsRun stepB.py print                   \
    is50ns=False                        \
    isPromptRecoData=True               \
    globalTag=76X_dataRun2_v15          \
    label=SingleElectron                \
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

python cmssw2latino.py stepB_data_numEvent${EVENTS}.root

rm -rf stepB_data_numEvent${EVENTS}.root

