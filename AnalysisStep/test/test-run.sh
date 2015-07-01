if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./test-run.sh EVENTS"
    echo "  "
    exit -1
fi

export EVENTS=$1
export MYFILE=root://xrootd.unl.edu//store/mc/RunIISpring15DR74/TT_TuneCUETP8M1_13TeV-powheg-pythia8/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v3/10000/029DB39E-DD08-E511-A454-003048FFD720.root

rm -rf latino_stepB_numEvent${EVENTS}.root

cmsRun stepB.py print doNoFilter=False doCut='nLep>=1' selection=Tight doMuonIsoId=True doEleIsoId=True runPUPPISequence=False maxEvents=${EVENTS} inputFiles=${MYFILE}

python cmssw2latino.py stepB_numEvent${EVENTS}.root

rm -rf stepB_numEvent${EVENTS}.root

