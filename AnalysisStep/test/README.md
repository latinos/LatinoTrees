0. Documentation
====

Latino's twiki and indico pages.

    https://twiki.cern.ch/twiki/bin/view/CMS/LatinosFrameworkFor2015
    https://indico.cern.ch/category/3374/

External documentation.

    https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3ConfigurationFile
    https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3FAQ


1. Everything begins here
====

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240

    bash -l

    export SCRAM_ARCH=slc6_amd64_gcc493
    cmsrel CMSSW_7_6_3
    cd CMSSW_7_6_3/src/
    cmsenv


2. Get the material
====

    git cms-init
    git clone --branch 13TeV git@github.com:latinos/setup.git LatinosSetup
    source LatinosSetup/Setup.sh


3. Produce latino trees
====

*Do this only if you want to create a tag.*

    pushd LatinoTrees
    git tag -a 21October2015_v2 -m '1.28/fb miniAODv2 latino production'
    git push origin 21October2015_v2
    popd

*Do this only if you want to use a tag.*

    pushd LatinoTrees
    git checkout tags/18Jan2016_StarWars
    popd

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

    python multicrab.py samples/samples_spring15_miniaodv2_25ns.py
    python multicrab.py samples/samples_dataD_05Oct2015_25ns.py
    python multicrab.py samples/samples_dataD_PromptReco_25ns.py

Resubmit jobs.

    python multicrab.py crab_projects_21October resubmit

Check status.
    
    crab status crab_projects_21October/crab_Run2015D_PromptReco_25ns_DoubleMuon

    python multicrab.py crab_projects_21October status

6. How much luminosity?
====

*Only for data.* Use [brilcalc](http://cms-service-lumi.web.cern.ch/cms-service-lumi/brilwsdoc.html) to report the analyzed (or missed) luminosity.

    crab report crab_projects_21October/crab_Run2015D_PromptReco_25ns_DoubleMuon

    export PATH=$HOME/.local/bin:/afs/cern.ch/cms/lumi/brilconda-1.0.3/bin:$PATH

    brilcalc lumi --begin 256630 --end 258158 -u /fb -i Cert_246908-258750_13TeV_PromptReco_Collisions15_25ns_JSON.txt
    brilcalc lumi --begin 258159 --end 999999 -u /fb -i Cert_246908-258750_13TeV_PromptReco_Collisions15_25ns_JSON.txt

    brilcalc lumi -u /pb -i missingLumiSummary.json


7. Run cmssw2latino
====

You can choose between `multiLxbatchCmssw2latino.py` and `multiLxbatchCmssw2latino_split.py`.
    
    python multiLxbatchCmssw2latino.py       samples/listFiles.py
    python multiLxbatchCmssw2latino_split.py samples/listFiles.py [number of files per hadd, default is 200]


8. Postprocess the latino trees
====

To add the PU weight in the MC latino trees we need to be provided with a PU json file. Once we have it one person should produce and share the `pudata.root` file.

    pushd /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV
    pileupCalc.py \
        -i Cert_246908-258750_13TeV_PromptReco_Collisions15_25ns_JSON.txt \
        --inputLumiJSON=PileUp/pileup_latest.txt \
        --calcMode=true \
        --minBiasXsec=69000 \
        --maxPileupBin=50 \
        --numPileupBins=50 \
        /afs/cern.ch/user/p/piedra/work/pudata.root
    popd

Steps to be followed (in this order) for MC. Please have a look at the Gardener [README](https://github.com/latinos/LatinoAnalysis/blob/master/Gardener/test/README.md) for detailed instructions.

    gardener.py mcweightsfiller -r input_folder output_folder

    gardener.py puadder -r input_folder output_folder \
        --data=pudata.root \
        --HistName=pileup \
        --branch=puW \
        --kind=trpu

    gardener.py adder -v 'baseW/F=<number from google doc>' latino_input.root latino_output.root  

    gardener.py wwNLLcorrections -m 'powheg' latino_WW.root latino_WW_NLL.root

This step, to be applied on both data and MC, requires two good leptons and removes them from the jet collection.

    gardener.py l2selfiller -r input_folder output_folder


9. Example of file copy from EOS
====

Mount eos if you need to rearrange the source files.

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240
    eosmount  eos
    eosumount eos

Verify the location of the source files.

    eos ls /eos/cms/store/group/phys_higgs/cmshww/amassiro/RunII/21Oct/data/25ns/Run2015D_PromptReco

Login to gridui.

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240
    cd /gpfs/csic_projects/cms/piedra/work/CMSSW_7_5_3/src
    source /cvmfs/cms.cern.ch/cmsset_default.sh
    cmsenv

Go to the destination folder and copy all the files from the source directory.

    xrdcp --recursive root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshww/amassiro/RunII/21Oct/data/25ns/Run2015D_PromptReco .

