0. Documentation
====

    https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookCRAB3Tutorial#CRAB_configuration_parameters
    https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3ConfigurationFile
    https://twiki.cern.ch/CMSPublic/WorkBookXrootdService


1. Everything begins here
====

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240

    bash -l

    export SCRAM_ARCH=slc6_amd64_gcc491
    cmsrel CMSSW_7_4_9
    cd CMSSW_7_4_9/src/
    cmsenv


2. Get the material
====

    git cms-init
    git clone --branch 13TeV git@github.com:latinos/setup.git LatinosSetup
    source LatinosSetup/Setup.sh


3. Produce latino trees
====

*Do this only if you want to create a tag.*

    git tag -a 17September2015_v6 -m 'Ready for the next production'
    git push origin 17September2015_v6

*Do this only if you want to use a tag.*

    cd LatinoTrees
    git checkout tags/17September2015_v6

Test the latino tree production.

    scram b -j 10
    cd LatinoTrees/AnalysisStep/test/
    ./test-run.sh 100


4. Setup CRAB
====

    cd LatinoTrees/AnalysisStep/test/crab

    source /cvmfs/cms.cern.ch/crab3/crab.sh

Check if you have writing permissions in the common area.

    crab checkwrite --site=T2_CH_CERN --lfn=/store/group/phys_higgs/cmshww/amassiro/RunII/


5. Run CRAB
====

Submit jobs.

    python multicrab.py samples/samples_spring15.py
    python multicrab.py samples/samples_dataB_PromptReco.py
    python multicrab.py samples/samples_dataB_17Jul2015.py
    python multicrab.py samples/samples_dataC_PromptReco_25ns.py
    python multicrab.py samples/samples_dataC_PromptReco_50ns.py

Resubmit jobs.

    python multicrab.py crab_projects_17September resubmit

Check status.
    
    crab status crab_projects_17September/PromptReco_crab_DoubleEG

    python multicrab.py crab_projects_17September status

*Only for data.* Report the non-analyzed luminosity sections.

    crab report -d crab_projects_17September/PromptReco_crab_DoubleEG


6. Run cmssw2latino
====

You can choose between `multiCmssw2latino.py` and `multiLxbatchCmssw2latino.py`.
    
    python multiCmssw2latino.py        samples/listFiles50ns.py
    python multiLxbatchCmssw2latino.py samples/listFiles50ns.py


7. Example of file copy from EOS
====

Verify that the files are available at the source.

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240
    eos ls /eos/cms/store/group/phys_higgs/cmshww/amassiro/RunII/5Aug/25ns/

Copy to the destination.

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240
    cd /gpfs/csic_projects/cms/piedra/work/CMSSW_7_4_6/src
    source /cvmfs/cms.cern.ch/cmsset_default.sh
    cmsenv

    cd /gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns
    xrdcp --force --recursive --silent root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshww/amassiro/RunII/5Aug/25ns .

