Crab3 usage
====


See details in:

    https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookCRAB3Tutorial#CRAB_configuration_parameters
    https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3ConfigurationFile

check if I have writing permissions:

    source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.sh
    source /cvmfs/cms.cern.ch/crab3/crab.sh
    crab checkwrite --site=T2_CH_CERN
    crab checkwrite --site=T2_CH_CERN  --lfn=/store/group/phys_higgs/cmshww/amassiro/RunII/test/

run

    crab submit -c crab_example.py
    



