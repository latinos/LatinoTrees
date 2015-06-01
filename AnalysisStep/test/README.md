Everything begins here
====

The command `bash -l` is needed only if the shell is not bash.

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240

    bash -l

    export SCRAM_ARCH=slc6_amd64_gcc491
    cmsrel CMSSW_7_3_3
    cd CMSSW_7_3_3/src/
    cmsenv


Last release tested and supported:

    CMSSW_7_4_2   (--> problems while running)

    
    
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

    xrdcp root://xrootd.unl.edu//store/mc/Phys14DR/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/00C90EFC-3074-E411-A845-002590DB9262.root .


Run step B
====

    scram b -j 10

    cd LatinoTrees/AnalysisStep/test/

    cmsRun stepB.py print \
                    label=Top \
                    id=123456789 \
                    scale=1 \
                    outputFile=stepB_MC.root \
                    doNoFilter=True \
                    doMuonIsoId=True \
                    maxEvents=200 \
                    doLHE=False \
                    doGen=True \
                    doBTag=True \
                    inputFiles=root://xrootd.unl.edu//store/mc/Phys14DR/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/00C90EFC-3074-E411-A845-002590DB9262.root

If the input file is local.

                    inputFiles=file:/afs/cern.ch/user/p/piedra/work/store/mc/Phys14DR/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/00C90EFC-3074-E411-A845-002590DB9262.root


Parameter details
====

To add LHE information.

    doLHE=True

To add GEN information like `genjets` or `genleptons`.

    doGen=True

To add alternative b-tagging variables like `jetcsvv2ivf`.

    doBTag=True
    
To apply a cut.

    doNoFilter=False \
    doCut=ptMin\>20

To activate the dumping of MC weights. It requires GEN/LHE information directly from edm products. Default is `False`
    
    doMCweights=True

    
    
Create the final latino tree
====

    python cmssw2latino.py stepB_MC_numEvent200.root

