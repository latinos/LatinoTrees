if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./test-run.sh EVENTS"
    echo "  "
    exit -1
fi

export EVENTS=$1

rm -rf latino_stepB_MC_numEvent${EVENTS}.root

cmsRun stepB.py print \
    label=WZ \
    id=1111 \
    scale=1 \
    outputFile=stepB_MC.root \
    doNoFilter=True \
    doMuonIsoId=True \
    doEleIsoId=True \
    maxEvents=${EVENTS} \
    doLHE=False \
    doGen=True \
    doBTag=True \
    globalTag=MCRUN2_74_V9A \
    inputFiles=root://xrootd.unl.edu//store/mc/RunIISpring15DR74/TT_TuneCUETP8M1_13TeV-powheg-pythia8/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v3/10000/029DB39E-DD08-E511-A454-003048FFD720.root
   #inputFiles=file:/afs/cern.ch/user/p/piedra/work/store/mc/Phys14DR/WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/484D51C6-2673-E411-8AB0-001E67398412.root

python cmssw2latino.py stepB_MC_numEvent${EVENTS}.root

rm -rf stepB_MC_numEvent${EVENTS}.root

