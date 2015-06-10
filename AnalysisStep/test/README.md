1. Everything begins here
====

The command `bash -l` is needed only if the shell is not bash.

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240

    bash -l

    export SCRAM_ARCH=slc6_amd64_gcc491
    cmsrel CMSSW_7_4_4_ROOT5
    cd CMSSW_7_4_4_ROOT5/src/
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
    git checkout tags/10June2015

    scram b -j 10
    cd LatinoTrees/AnalysisStep/test/crab
    source /cvmfs/cms.cern.ch/crab3/crab.sh
    python multicrab.py samples/samples_spring15.py


4. Produce a test latino tree
====

    scram b -j 10
    cd LatinoTrees/AnalysisStep/test/
    ./test-run.sh 100

