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

# export MYFILE=file:/afs/cern.ch/user/a/amassiro/public/xLatinos/76X/CC3AD3BC-E898-E511-8707-0025904C5182.root
export MYFILE=file:/tmp/amassiro/161321CF-5FB8-E511-8E6F-008CFA056400.root
# export MYFILE=root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluHToWWTo2L2Nu_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/161321CF-5FB8-E511-8E6F-008CFA056400.root



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

# # #export MYFILE=file:/afs/cern.ch/work/p/piedra/public/store/data/Run2015D/DoubleMuon/MINIAOD/16Dec2015-v1/10000/926D0103-DBA7-E511-AE1C-24BE05C6E561.root
# # export MYFILE=file:/afs/cern.ch/user/p/piedra/work/store/data/Run2015D/MET/MINIAOD/16Dec2015-v1/50000/00EA1DB2-90AA-E511-AEEE-0025905C2CE6.root
# # 
# # 
# # rm -rf latino_stepB_data_numEvent${EVENTS}.root
# # 
# # cmsRun stepB.py print                   \
# #     reportEvery=10                      \
# #     summary=false                       \
# #     is50ns=False                        \
# #     isPromptRecoData=True               \
# #     globalTag=76X_dataRun2_v15          \
# #     label=MET                           \
# #     outputFile=stepB_data.root          \
# #     selection=LooseNoIso                \
# #     doNoFilter=False                    \
# #     doMuonIsoId=True                    \
# #     doEleIsoId=True                     \
# #     doBTag=True                         \
# #     runPUPPISequence=True               \
# #     doPhotonID=True                     \
# #     maxEvents=${EVENTS}                 \
# #     inputFiles=${MYFILE}
# # 
# # ### FIXME isPromptRecoData is a bad name. How about isRecoLabel?
# # 
# # python cmssw2latino.py stepB_data_numEvent${EVENTS}.root
# # 
# # rm -rf stepB_data_numEvent${EVENTS}.root
