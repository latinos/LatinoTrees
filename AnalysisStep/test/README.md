0. Documentation
====

Latino's twiki and indico pages.

    https://twiki.cern.ch/twiki/bin/view/CMS/LatinosFrameworkFor2015
    https://twiki.cern.ch/twiki/bin/view/CMS/LatinosFrameworkTutorials
    https://indico.cern.ch/category/3374/

External documentation.

    https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3ConfigurationFile
    https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3FAQ


1. Everything begins here
====

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240

    bash -l

    export SCRAM_ARCH=slc6_amd64_gcc530
    cmsrel CMSSW_8_0_17
    cd CMSSW_8_0_17/src/
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
    git checkout tags/3June2016_NinjaTurtles_v3
    popd

Test the latino tree production.

    scram b -j 10
    cd LatinoTrees/AnalysisStep/test/

    ./test-run-8x-data.sh 100
    ./test-run-8x-mc.sh   100

For 76x 2015 data and MC the frozen version is *18Jan2016_StarWars_v3_frozenFor76x*.

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

    brilcalc lumi --begin 256630 --end 258158 -u /fb -i Cert_246908-258750_13TeV_PromptReco_Collisions15_25ns_JSON.txt
    brilcalc lumi --begin 258159 --end 999999 -u /fb -i Cert_246908-258750_13TeV_PromptReco_Collisions15_25ns_JSON.txt

    brilcalc lumi -u /pb -i missingLumiSummary.json

The luminosity for Moriond is 2.318/fb.
        
    brilcalc lumi --begin 254230 --end 999999 -u /fb \
        --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/moriond16_normtag.json \
        -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Reprocessing/Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON.txt


7. Run cmssw2latino
====

The default number of files per *hadd* is 100. Please change it to smaller values if the output file size is bigger than 1 GB.

    python multiLxbatchCmssw2latino_split.py samples/listFiles.py 100

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

