0. Documentation
====

Latino's twiki and indico pages.

    https://twiki.cern.ch/twiki/bin/view/CMS/LatinosFrameworkFor2017
    https://twiki.cern.ch/twiki/bin/view/CMS/LatinosFrameworkTutorials
    https://indico.cern.ch/category/3374/


1. Everything begins here
====

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240

    bash -l

    export SCRAM_ARCH=slc6_amd64_gcc530
    cmsrel CMSSW_8_0_26_patch1
    cd CMSSW_8_0_26_patch1/src/
    cmsenv


2. Get the material
====

First you need to [generate an SSH key](https://help.github.com/articles/generating-an-ssh-key/).

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
    git checkout tags/Feb2017_TheGreatWall_v1
    popd

Test the latino tree production.

    scram b -j 10
    cd LatinoTrees/AnalysisStep/test/

    ./test-run-8x-data.sh 100
    ./test-run-8x-mc.sh   100

Old tags.

    18Jan2016_StarWars_v3_frozenFor76xFor -- frozen tag for 76x 2015 data and MC
    26Oct2016_last801X                    -- last tag that works with 8_0_1X


4. Setup CRAB
====

    cd LatinoTrees/AnalysisStep/test/crab

The CRAB client can be sourced using the command below *after cmsenv*.

    source /cvmfs/cms.cern.ch/crab3/crab.sh

Check if you have writing permissions in the common area.

    crab checkwrite --site=T2_CH_CERN --lfn=/store/group/phys_higgs/cmshww/amassiro/RunII/


5. Run CRAB
====

Submit jobs.

    python multicrab.py samples/samples_spring15_miniaodv2_25ns.py
    python multicrab.py samples/samples_dataD_05Oct2015_25ns.py

Resubmit jobs.

    python multicrab.py crab_projects_21October resubmit

Check status.
    
    python multicrab.py crab_projects_21October status


6. How much luminosity?
====

*Only for data.* Use [brilcalc](http://cms-service-lumi.web.cern.ch/cms-service-lumi/brilwsdoc.html) to report the analyzed (or missed) luminosity.

    crab report crab_projects_21October/crab_Run2015D_PromptReco_25ns_DoubleMuon

    export PATH=$HOME/.local/bin:/afs/cern.ch/cms/lumi/brilconda-1.0.3/bin:$PATH

    brilcalc lumi -u /pb -i missingLumiSummary.json

The luminosity for Moriond 2016 was 2.318/fb.

    brilcalc lumi --begin 254230 --end 999999 -u /fb \
        --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/moriond16_normtag.json \
        -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Reprocessing/Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON.txt


7. Run cmssw2latino
====

First source the CRAB client if you have not done it yet.

    source /cvmfs/cms.cern.ch/crab3/crab.sh

The script *translate_samples_config.py* can be used on any samples*.py file in AnalysisStep/test/crab/samples/ and it will create the input file for the cmssw2latino step.

    python translate_samples_config.py samples/samples_data_2016_ReReco.py list_data_2016_ReReco.py

The next step submits jobs for producing the latino trees with a maximum size of 1GB. In order to work on different T2s it makes use of the batchTools that are also used in the post-processing which requires the following steps:

    - Go to LatinosAnalysis/Tools/python
    - cp userConfig_TEMPLATE.py userConfig.py
    - Edit userConfig.py 'baseDir' variable (You need a location with some reasonable space for job log files) 

By default the python script doesn't submit jobs so you can check if it correctly finds all the samples and directories. To submit the jobs run the command with "1" as the last argument:

    python multiLxbatchCmssw2latino_split_autoComplete.py list_data_2016_ReReco.py [0 = no jobs are submitted (default) / 1 = submit jobs ]

Do not forget to delete all stepB.root files from old latino productions. We have a limited eos space.


8. Postprocess the latino trees
====

    https://twiki.cern.ch/twiki/bin/view/CMS/LatinosTreesRun2


A. Combine
====

This page documents the CombineHarvester framework for the production and
analysis of datacards for use with the CMS combine tool. The central part of
this framework is the CombineHarvester class, which provides a representation
of the text-format datacards and the associated shape input.

    http://cms-analysis.github.io/CombineHarvester/
    https://twiki.cern.ch/twiki/bin/view/CMS/HWWCombineTools


B. Share IT EOS space
====

Please follow the steps below to share your (cernbox) IT EOS space with the latino community.

1. Login to [cernbox](cernbox.cern.ch).
2. Create a HWW2016 folder.
3. Click on the "share" icon (it looks like a "<" sign) for that folder.
4. Share it in read-only mode with the latinos-hep e-group.
5. Share it in write+read mode with these users: janssen, jlauwers, ddicroce, amassiro, calderon.
