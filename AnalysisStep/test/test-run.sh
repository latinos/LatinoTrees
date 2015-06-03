if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./test-run.sh EVENTS"
    echo "  "
    exit -1
fi

export EVENTS=$1

rm -rf latino_stepB_MC_numEvent${EVENTS}.root

cmsRun stepB.py print \
    label=Top \
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
    inputFiles=file:/afs/cern.ch/user/p/piedra/work/store/mc/Phys14DR/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/00C90EFC-3074-E411-A845-002590DB9262.root

python cmssw2latino.py stepB_MC_numEvent${EVENTS}.root

rm -rf stepB_MC_numEvent${EVENTS}.root

