1. Everything begins here
====

The command `bash -l` is needed only if the shell is not bash.

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240

    bash -l

    export SCRAM_ARCH=slc6_amd64_gcc491
    cmsrel CMSSW_7_4_6
    cd CMSSW_7_4_6/src/
    cmsenv


2. Get the material
====

    git cms-init
    git clone --branch 13TeV git@github.com:latinos/setup.git LatinosSetup

    source LatinosSetup/Setup.sh


3. Produce common latino trees
====

Follow this step ONLY if you want to produce common latino trees.

    cd LatinoTrees
    git checkout tags/1July2015

    scram b -j 10
    cd LatinoTrees/AnalysisStep/test/crab
    source /cvmfs/cms.cern.ch/crab3/crab.sh
    python multicrab.py samples/samples_spring15.py

    cmsLs /store/group/phys_higgs/cmshww/amassiro/RunII/test/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/crab_25ns_WZTo3LNu/150703_170518/0000/ | grep stepB_MC | awk -v p="" '{if ($5!="") p=p" root://eoscms//eos/cms"$5}; END{print "hadd test.root" p}' | sh


4. Produce a test latino tree
====

    scram b -j 10
    cd LatinoTrees/AnalysisStep/test/
    ./test-run.sh 100

