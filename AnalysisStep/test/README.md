Everything begins here
====

The command `bash -l` is needed only if the shell is not bash.

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240

    bash -l

    export SCRAM_ARCH=slc6_amd64_gcc491
    cmsrel CMSSW_7_3_0
    cd CMSSW_7_3_0/src/
    cmsenv


Setup CMS git connection
====

Necessary to checkout CMSSW code (https://hypernews.cern.ch/HyperNews/CMS/get/git/200.html).

    git cms-init

    
Get the material
====

    git clone --branch 13TeV git@github.com:latinos/setup.git LatinosSetup

    source LatinosSetup/Setup.sh


Get a MiniAOD test file
====

Useful for faster code test.

    cd LatinoTrees/AnalysisStep/test

    source /afs/cern.ch/cms/cmsset_default.sh
    source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.sh
    voms-proxy-init -voms cms

    xrdcp root://xrootd.unl.edu//store/mc/Phys14DR/GluGluToHToWWTo2LAndTau2Nu_M-125_13TeV-powheg-pythia6/MINIAODSIM/AVE30BX50_tsg_PHYS14_ST_V1-v1/10000/440AA9AF-9988-E411-9786-00266CFFA038.root .


Run step B
====

    scram b -j 10

    cd LatinoTrees/AnalysisStep/test/

    cmsRun stepB.py print \
                    label=WW \
                    id=123456789 \
                    scale=1 \
                    outputFile=stepB_MC.root \
                    doNoFilter=True \
                    doMuonIsoId=True \
                    maxEvents=200 \
                    doLHE=True \
                    doGen=True \
                    doBTag=True \
                    inputFiles=root://xrootd.unl.edu//store/mc/Phys14DR/GluGluToHToWWTo2LAndTau2Nu_M-125_13TeV-powheg-pythia6/MINIAODSIM/AVE30BX50_tsg_PHYS14_ST_V1-v1/10000/440AA9AF-9988-E411-9786-00266CFFA038.root

If the input file is local.

                    inputFiles=file:440AA9AF-9988-E411-9786-00266CFFA038.root
                    

Create the final tree
====

    python cmssw2latino.py stepB_MC_numEvent200.root


    
    
Details
====

To add LHE information.

    doLHE=True

To add GEN information like `genjets` or `genleptons`.

    doGen=True

To add alternative b-tagging variables like `jetcsvv2ivf`.

    doBTag=True
    
To apply a cut at tree creation step:

    doCut=ptMin\>20
    
    NB: doNoFilter overrides this cut 



    