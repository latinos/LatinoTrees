Crab3 usage
====


See details in:

    https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookCRAB3Tutorial#CRAB_configuration_parameters
    https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3ConfigurationFile

check if I have writing permissions:

    source /cvmfs/cms.cern.ch/crab3/crab.sh
    
    crab checkwrite --site=T2_CH_CERN
    crab checkwrite --site=T2_CH_CERN  --lfn=/store/group/phys_higgs/cmshww/amassiro/RunII/test/

run

    crab submit -c crab_example.py
    crab status
    
then:

    python ../cmssw2latino.py   /tmp/amassiro/stepB_MC_ggHww_1.root


details:

    it will save in something like:
    /eos/cms/store/group/phys_higgs/cmshww/amassiro/RunII/test/GluGluToHToWWTo2LAndTau2Nu_M-125_13TeV-powheg-pythia6/crab_MCtest/150116_163732/0000/stepB_MC_ggHww_1.root


    
Multicrab
====

    python multicrab.py

    