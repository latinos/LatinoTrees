CRAB3
====

See details here:

    https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookCRAB3Tutorial#CRAB_configuration_parameters
    https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3ConfigurationFile

Check if you have writing permissions:

    source /cvmfs/cms.cern.ch/crab3/crab.sh
    source /cvmfs/cms.cern.ch/crab3/crab.csh
    
    crab checkwrite --site=T2_CH_CERN
    crab checkwrite --site=T2_CH_CERN --lfn=/store/group/phys_higgs/cmshww/amassiro/RunII/test/

Send the jobs:

    crab submit -c crab_example1.py
    crab status
    
And create the final tree:

    python ../cmssw2latino.py   /tmp/amassiro/stepB_MC_ggHww_1.root

You should get something like:

    /eos/cms/store/group/phys_higgs/cmshww/amassiro/RunII/test/GluGluToHToWWTo2LAndTau2Nu_M-125_13TeV-powheg-pythia6/crab_MCtest/150116_163732/0000/stepB_MC_ggHww_1.root


MultiCRAB
====

    python multicrab.py

    crab status
    
    crab status folder_name

    
    python multicrab_template.py samples/multicrab_samples_phys14.py

