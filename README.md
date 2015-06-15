LatinoTrees
===========

    AnalysisStep
    DataFormats



    
    
    
Additional packages (temporary solutions, then they will appear in cmssw release):

~~~
puppi
~~~

From
    https://twiki.cern.ch/twiki/bin/view/CMS/Phys14JMERecipes#PUPPI_example_for_AK8_jets
    
cmssw release needed:

    >= CMSSW_7_3_0

install:

(for jettoolbox)

    git cms-merge-topic alefisico:jetToolbox_73X
    scram b -j 16

(for puppi)
    
    git cms-addpkg CommonTools/PileupAlgos
    git remote add nhan-remote https://github.com/nhanvtran/cmssw.git
    git fetch nhan-remote puppi-bugfix-for-miniaod
    git cherry-pick 0585bf21ae098f14f144b9a3d361178e6cc830e6
    scram b

    
    
backup
====

    LXPLUS-LatinosFramework
    /afs/cern.ch/user/a/amassiro/work/Latinos/Framework/CMSSW_7_0_6_patch1/src/  --> old
    /afs/cern.ch/user/a/amassiro/work/Latinos/Framework/CMSSW_7_0_7_patch1/src/
    on slc6 machine
    scp amassiro@cmsneu:/data/amassiro/CMSSWRoot/Spring14/miniAOD/VBF_HToWWTo2LAndTau2Nu_M-125_13TeV-powheg-pythia6/40F65A6C-2209-E411-9E44-003048CDCEC2.root /tmp/amassiro/


Phys14:

    /afs/cern.ch/user/a/amassiro/work/Latinos/Framework/CMSSW_7_2_0/src
    /afs/cern.ch/user/a/amassiro/work/Latinos/Framework/CMSSW_7_3_0/src
    /afs/cern.ch/user/a/amassiro/work/Latinos/Framework/CMSSW_7_3_1/src
    /afs/cern.ch/user/a/amassiro/work/Latinos/Framework/CMSSW_7_3_2/src (almost unused)
    /afs/cern.ch/user/a/amassiro/work/Latinos/Framework/CMSSW_7_3_3/src
    /afs/cern.ch/user/a/amassiro/work/Latinos/Framework/CMSSW_7_4_3/src -> not working
    /afs/cern.ch/user/a/amassiro/work/Latinos/Framework/CMSSW_7_4_4/src

    scp amassiro@cmsneu:/data/amassiro/CMSSWRoot/Phys14/miniAOD/GluGluToHToWWTo2LAndTau2Nu_M-125_13TeV-powheg-pythia6/C667E84D-9D18-E411-99D8-02163E00ECE6.root  /tmp/amassiro/
    ln -s /tmp/amassiro/C667E84D-9D18-E411-99D8-02163E00ECE6.root    08CFEF83-586C-E411-8D7C-002590A2CCF2.root
    ln -s /tmp/amassiro/C667E84D-9D18-E411-99D8-02163E00ECE6.root    440AA9AF-9988-E411-9786-00266CFFA038.root

    xrdcp root://xrootd.unl.edu//store/mc/Phys14DR/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU30bx50_PHYS14_25_V1-v1/00000/003B6371-8D81-E411-8467-003048F0E826.root /tmp/amassiro/
    scp amassiro@cmsneu:/media/data/CMSSWRoot/Phys14DR/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/003B6371-8D81-E411-8467-003048F0E826.root  /tmp/amassiro/003B6371-8D81-E411-8467-003048F0E826.root
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
                    globalTag=MCRUN2_74_V9A \
                    inputFiles=file:/tmp/amassiro/003B6371-8D81-E411-8467-003048F0E826.root

    xrdcp root://xrootd.unl.edu//store/mc/RunIISpring15DR74/WWTo2L2Nu_13TeV-powheg/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v1/60000/0AA28275-5001-E511-8A45-0CC47A4DEDE0.root  /tmp/amassiro/
    scp amassiro@cmsneu:/media/data/CMSSWRoot/RunIISpring15DR74/WWTo2L2Nu_13TeV-powheg/MINIAODSIM/0AA28275-5001-E511-8A45-0CC47A4DEDE0.root /tmp/amassiro/
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
                    globalTag=MCRUN2_74_V9A \
                    inputFiles=file:/tmp/amassiro/0AA28275-5001-E511-8A45-0CC47A4DEDE0.root
                    
       
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
                    globalTag=MCRUN2_74_V9A \
                    selection=LooseNoIso \
                    inputFiles=file:/tmp/amassiro/0AA28275-5001-E511-8A45-0CC47A4DEDE0.root
       
       
       
    scp amassiro@cmsneu:/media/data/CMSSWRoot/RunIISpring15DR74/WWTo2L2Nu_13TeV-powheg/MINIAODSIM/25ns/082EF100-DC05-E511-AD3F-A0040420FE80.root /tmp/amassiro/
    scp amassiro@cmsneu:/media/data/CMSSWRoot/RunIISpring15DR74/WWTo2L2Nu_13TeV-powheg/MINIAODSIM/50ns/1EF48EA6-DE0B-E511-99EF-0002C92A1024.root /tmp/amassiro/
       
    cmsRun stepB.py print \
                    label=WW \
                    id=123456789 \
                    scale=1 \
                    outputFile=stepB_MC_25ns.root \
                    doNoFilter=True \
                    doMuonIsoId=True \
                    doLHE=False \
                    doGen=True \
                    doBTag=True \
                    globalTag=MCRUN2_74_V9A \
                    selection=LooseNoIso \
                    inputFiles=file:/tmp/amassiro/082EF100-DC05-E511-AD3F-A0040420FE80.root
    
    cmsRun stepB.py print \
                    label=WW \
                    id=123456789 \
                    scale=1 \
                    outputFile=stepB_MC_50ns.root \
                    doNoFilter=True \
                    doMuonIsoId=True \
                    doLHE=False \
                    doGen=True \
                    doBTag=True \
                    globalTag=MCRUN2_74_V9A \
                    selection=LooseNoIso \
                    inputFiles=file:/tmp/amassiro/1EF48EA6-DE0B-E511-99EF-0002C92A1024.root
    
    latino->Draw("std_vector_jet_pt[0]","std_vector_jet_pt[0]>0")
    latino->Draw("std_vector_leptonGen_fromHardProcessBeforeFSR[0]","std_vector_leptonGen_pt[0]>0")
    latino->Draw("std_vector_neutrinoGen_fromHardProcessBeforeFSR[0]","std_vector_neutrinoGen_pt[0]>0")
    latino->Draw("std_vector_neutrinoGen_pt[0]","std_vector_neutrinoGen_pt[0]>0")
     
    

    