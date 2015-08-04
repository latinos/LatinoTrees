0. Documentation
====

    https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookCRAB3Tutorial#CRAB_configuration_parameters
    https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3ConfigurationFile


1. Everything begins here
====

The command `bash -l` is needed only if the shell is not bash.

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240

    bash -l

    export SCRAM_ARCH=slc6_amd64_gcc491
    cmsrel CMSSW_7_4_7
    cd CMSSW_7_4_7/src/
    cmsenv


2. Get the material
====

    git cms-init
    git clone --branch 13TeV git@github.com:latinos/setup.git LatinosSetup

    source LatinosSetup/Setup.sh


3. Produce latino trees
====

Follow this step if you want to use a specific tag.

    cd LatinoTrees
    git checkout tags/1July2015

Test the latino tree production

    scram b -j 10
    cd LatinoTrees/AnalysisStep/test/
    ./test-run.sh 100


4. Setup CRAB
====

    cd LatinoTrees/AnalysisStep/test/crab

    source /cvmfs/cms.cern.ch/crab3/crab.sh
    source /cvmfs/cms.cern.ch/crab3/crab.csh

Check if you have writing permissions in the common area.

    crab checkwrite --site=T2_CH_CERN
    crab checkwrite --site=T2_CH_CERN --lfn=/store/group/phys_higgs/cmshww/amassiro/RunII/test/


5. Run CRAB
====

Submit jobs.

    python multicrab.py samples/samples_spring15.py
    python multicrab.py samples/samples_dataB_PromptReco.py
    python multicrab.py samples/samples_dataB_17Jul2015.py

Resubmit jobs.

    python multicrab.py crab_projects_4August resubmit

Check status.
    
    crab status crab_projects_4August/crab_DoubleEG

    python multicrab.py crab_projects_4August status

6. Run cmssw2latino
====

You can choose between `multiCmssw2latino.py` and `multiLxbatchCmssw2latino.py`.
    
    python multiCmssw2latino.py        samples/listFiles50ns.py
    python multiLxbatchCmssw2latino.py samples/listFiles50ns.py

