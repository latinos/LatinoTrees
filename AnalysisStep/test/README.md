Everything begins here
====

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240

    bash -l  # Needed only if the shell is not bash

    export SCRAM_ARCH=slc6_amd64_gcc481

    cmsrel CMSSW_7_2_2
    cd CMSSW_7_2_2/src/
    cmsenv


Setup CMS git connection
====

It is necessary to checkout CMSSW code (https://hypernews.cern.ch/HyperNews/CMS/get/git/200.html).

    git cms-init

    
Get the material
====

    git clone --branch 13TeV git@github.com:latinos/setup.git LatinosSetup

    source LatinosSetup/Setup.sh


Get a MiniAOD test file
====

    source /afs/cern.ch/cms/cmsset_default.sh

    voms-proxy-init

    xrdcp root://xrootd.unl.edu//store/mc/Phys14DR/GluGluToHToWWTo2LAndTau2Nu_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/08CFEF83-586C-E411-8D7C-002590A2CCF2.root LatinoTrees/AnalysisStep/test/.


Run step B
====

    scram b -j 10

    cd LatinoTrees/AnalysisStep/test/

    cmsRun stepB.py print \
                    inputFiles=file:08CFEF83-586C-E411-8D7C-002590A2CCF2.root \
                    label=WW \
                    id=123456789 \
                    scale=1 \
                    outputFile=stepB_latinosYieldSkim_MC_ggHww.root
                    doNoFilter=True

                    
If you want to activate PUPPI jets.
                    
                    runPUPPISequence=True


Create the final tree
====

    python cmssw2latino.py stepB_latinosYieldSkim_MC_ggHww.root

