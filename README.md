=============
LatinoTrees
=============

    cmsrel CMSSW_7_X_Y 
    cd  CMSSW_7_X_Y
    cmsenv

    git clone https://github.com/latinos/LatinoTrees.git
    cd LatinosTrees
    git checkout latino_SLHC_miniAOD
    cd ..

    // special fixes to egamma object to run the miniAOD outside 7XY reco
    cp -r /afs/cern.ch/user/r/rgerosa/public/xTP/CMSSW_7_1_0/src/RecoEgamma
  
    // for making puppi jets (tmp recipe since puppi is not in release, will be there for 73X, added for consumes wrt to Phill code)
    cp -r /afs/cern.ch/user/r/rgerosa/public/xTP/CMSSW_7_1_0/src/Dummy

    scramv1 b -j 16

=======================================================================
To Run the miniAOD production starting from SLHC samples in 62X
=======================================================================

    cd LatinosTrees/AnalysisStep/test/ 
    cmsRun miniAOD_stepB.py <options>

    has the same options of steB-py in the master branch but be carefull that the generictree producer is not a filter but an analyzer (no filtering of the event)  since
    it is added in the end pat when you produce miniAOD and latino tree in one shot
 
    If you want you can decide to produce only miniAOD or miniAOD+latino tree (both the output file are produced) using the following option : options.stopAtMiniAOD

    Puppi sequence is called by default, to be added the dump of puppi jets

===========
backup
===========

    LXPLUS-LatinosFramework
    /afs/cern.ch/user/a/amassiro/work/Latinos/Framework/CMSSW_7_0_6_patch1/src/  --> old
    /afs/cern.ch/user/a/amassiro/work/Latinos/Framework/CMSSW_7_0_7_patch1/src/
    on slc6 machine
    scp amassiro@cmsneu:/data/amassiro/CMSSWRoot/Spring14/miniAOD/VBF_HToWWTo2LAndTau2Nu_M-125_13TeV-powheg-pythia6/40F65A6C-2209-E411-9E44-003048CDCEC2.root /tmp/amassiro/
