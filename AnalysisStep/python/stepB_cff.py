import FWCore.ParameterSet.Config as cms
import os

nverticesModule = cms.EDProducer("VertexMultiplicityCounter",
    probes = cms.InputTag("REPLACE_ME"),
    objects = cms.InputTag("offlinePrimaryVertices"), # miniAOD goodPrimaryVertices
    objectSelection = cms.string("!isFake && ndof > 4 && abs(z) <= 25 && position.Rho <= 2"),
)

nPU = cms.EDProducer("PileUpMultiplicityCounter",
    puLabel = cms.InputTag("addPileupInfo")
)

#######################
#######################
#######################

def basicStepBTreeDefinition(process,jetId_WP = "0",pileupjetId_WP = "1", CJVminPt  = "30.", CJVmaxEta = "5.0", DphiJetVetominPt  = "15.", DphiJetVetominEta = "5.0",
                             DzBVeto    = "999999.9",minPtBVeto = "10.0"):

 
 process.stepBTree = cms.EDFilter("GenericTreeProducer",
    cut = cms.string("1"), ## no cut applied
    addRunLumiInfo  = cms.bool(True),
    variables = cms.PSet( ## variables to be added in the tree       
        channel   = cms.string("getChannel()"), ## type of channel
        dataset   = cms.string("MC"),
        ### store info up tp 4 leading lepton in the event (electron and muon) 
        v_lepton1 = cms.string("lepton(0)"),
        v_lepton2 = cms.string("lepton(1)"),
        v_lepton3 = cms.string("lepton(2)"),
        v_lepton4 = cms.string("lepton(3)"),

        std_vector_lepton_pt  = cms.string("getPtByPt"),
        std_vector_lepton_eta = cms.string("etaByPt"),
        std_vector_lepton_phi = cms.string("phiByPt"),

        mll  = cms.string("mll()"),
        ptll = cms.string("pTll()"),
        yll  = cms.string("yll()"), 
        pt1  = cms.string("getPtMax"),
        pt2  = cms.string("getPtMin"),
        pt3  = cms.string("getPtByPt(2)"),
        pt4  = cms.string("getPtByPt(3)"),
        dphill = cms.string("dPhill()"),
        drll   = cms.string("dRll()"),

        peaking = cms.string("peaking"), # possible Z candidates
        trigger = cms.string("guillelmoTrigger('DATASET')"),
        gammaMRStar = cms.string("gammaMRStar"),

        ## lepton info
        eta1 = cms.string("etaByPt(0)"),
        eta2 = cms.string("etaByPt(1)"),
        eta3 = cms.string("etaByPt(2)"),
        eta4 = cms.string("etaByPt(3)"),
        sceta1 = cms.string("etaSCByPt(0)"),
        sceta2 = cms.string("etaSCByPt(1)"),
        sceta3 = cms.string("etaSCByPt(2)"),
        sceta4 = cms.string("etaSCByPt(3)"),
        phi1 = cms.string("phiByPt(0)"),
        phi2 = cms.string("phiByPt(1)"),
        phi3 = cms.string("phiByPt(2)"),
        phi4 = cms.string("phiByPt(3)"),
        ch1 = cms.string("qByPt(0)"),
        ch2 = cms.string("qByPt(1)"),
        ch3 = cms.string("qByPt(2)"),
        ch4 = cms.string("qByPt(3)"),
        bdt1 = cms.string("leptBdtByPt(0)"),
        bdt2 = cms.string("leptBdtByPt(1)"),
        bdt3 = cms.string("leptBdtByPt(2)"),
        bdt4 = cms.string("leptBdtByPt(3)"),
        pdgid1 = cms.string("pdgIdByPt(0)"),
        pdgid2 = cms.string("pdgIdByPt(1)"),
        pdgid3 = cms.string("pdgIdByPt(2)"),
        pdgid4 = cms.string("pdgIdByPt(3)"),
        nbrem1 = cms.string("nBremByPt(0)"),
        nbrem2 = cms.string("nBremByPt(1)"),
        nbrem3 = cms.string("nBremByPt(2)"),
        nbrem4 = cms.string("nBremByPt(3)"),
        ## lepton iso
        isomva1 = cms.string("mvaIsoByPt(0)"),
        isomva2 = cms.string("mvaIsoByPt(1)"),
        isomva3 = cms.string("mvaIsoByPt(2)"),
        isomva4 = cms.string("mvaIsoByPt(3)"),

        ### muon ID         
        isTightMuon1 = cms.string("isTightMuon(0)"),
        isTightMuon2 = cms.string("isTightMuon(1)"),
        isTightMuon3 = cms.string("isTightMuon(2)"),
        isTightMuon4 = cms.string("isTightMuon(3)"),

        isLooseMuon1 = cms.string("isLooseMuon(0)"),
        isLooseMuon2 = cms.string("isLooseMuon(1)"),
        isLooseMuon3 = cms.string("isLooseMuon(2)"),
        isLooseMuon4 = cms.string("isLooseMuon(3)"),

        isSoftMuon1 = cms.string("isSoftMuon(0)"),
        isSoftMuon2 = cms.string("isSoftMuon(1)"),
        isSoftMuon3 = cms.string("isSoftMuon(2)"),
        isSoftMuon4 = cms.string("isSoftMuon(3)"),

        isHighPtMuon1 = cms.string("isHighPtMuon(0)"),
        isHighPtMuon2 = cms.string("isHighPtMuon(1)"),
        isHighPtMuon3 = cms.string("isHighPtMuon(2)"),
        isHighPtMuon4 = cms.string("isHighPtMuon(3)"),
        
        isSTA1 = cms.string("isSTAByPt(0)"),
        isSTA2 = cms.string("isSTAByPt(1)"),
        isSTA3 = cms.string("isSTAByPt(2)"),
        isSTA4 = cms.string("isSTAByPt(3)"),


        ## electron ID
        isTightElectron1 = cms.string("isTightElectron(0)"),
        isTightElectron2 = cms.string("isTightElectron(1)"),
        isTightElectron3 = cms.string("isTightElectron(2)"),
        isTightElectron4 = cms.string("isTightElectron(3)"),

        isLooseElectron1 = cms.string("isLooseElectron(0)"),
        isLooseElectron2 = cms.string("isLooseElectron(1)"),
        isLooseElectron3 = cms.string("isLooseElectron(2)"),
        isLooseElectron4 = cms.string("isLooseElectron(3)"),

        isLooseRobustElectron1 = cms.string("isLooseRobustElectron(0)"),
        isLooseRobustElectron2 = cms.string("isLooseRobustElectron(1)"),
        isLooseRobustElectron3 = cms.string("isLooseRobustElectron(2)"),
        isLooseRobustElectron4 = cms.string("isLooseRobustElectron(3)"),

        isTightRobustElectron1 = cms.string("isTightRobustElectron(0)"),
        isTightRobustElectron2 = cms.string("isTightRobustElectron(1)"),
        isTightRobustElectron3 = cms.string("isTightRobustElectron(2)"),
        isTightRobustElectron4 = cms.string("isTightRobustElectron(3)"),

        isRobustHighEnergyElectron1 = cms.string("isRobustHighEnergyElectron(0)"),
        isRobustHighEnergyElectron2 = cms.string("isRobustHighEnergyElectron(1)"),
        isRobustHighEnergyElectron3 = cms.string("isRobustHighEnergyElectron(2)"),
        isRobustHighEnergyElectron4 = cms.string("isRobustHighEnergyElectron(3)"),

        pfNeutralHadronIso1    = cms.string("pfNeutralHadronsIso(0)"),
        pfParticleAllIso1      = cms.string("pfParticleAllIso(0)"),
        pfPUfChargedHadronIso1 = cms.string("pfPUChargedHadronIso(0)"),
        pfChargedHadronIso1    = cms.string("pfChargedHadronsIso(0)"),
        pfGammaIso1            = cms.string("pfPhotonsIso(0)"),

        pfNeutralHadronIso2    = cms.string("pfNeutralHadronsIso(1)"),
        pfParticleAllIso2      = cms.string("pfParticleAllIso(1)"),
        pfPUfChargedHadronIso2 = cms.string("pfPUChargedHadronIso(1)"),
        pfChargedHadronIso2    = cms.string("pfChargedHadronsIso(1)"),
        pfGammaIso2            = cms.string("pfPhotonsIso(1)"),

        pfNeutralHadronIso3    = cms.string("pfNeutralHadronsIso(2)"),
        pfParticleAllIso3      = cms.string("pfParticleAllIso(2)"),
        pfPUfChargedHadronIso3 = cms.string("pfPUChargedHadronIso(2)"),
        pfChargedHadronIso3    = cms.string("pfChargedHadronsIso(2)"),
        pfGammaIso3            = cms.string("pfPhotonsIso(2)"),

        pfNeutralHadronIso4    = cms.string("pfNeutralHadronsIso(3)"),
        pfParticleAllIso4      = cms.string("pfParticleAllIso(3)"),
        pfPUfChargedHadronIso4 = cms.string("pfPUChargedHadronIso(3)"),
        pfChargedHadronIso4    = cms.string("pfChargedHadronsIso(3)"),
        pfGammaIso4            = cms.string("pfPhotonsIso(3)"),


    ),
    flags = cms.PSet(        
        nextra = cms.string("getNExtraLep(10)"),
        bveto_mu       = cms.string("getNSoftMu(3) == 0"),
        bveto_munj     = cms.string("getNSoftMu(3,1) == 0"),
        bveto_munj30   = cms.string("getNSoftMu(3,30) == 0"),
        bveto_munj05   = cms.string("getNSoftMu(3,1,0.5) == 0"),
        bveto_munj3005 = cms.string("getNSoftMu(3,30,0.5) == 0"),
        dphiveto   = cms.string("passesDPhillJet("+DphiJetVetominPt+","+DphiJetVetominEta+",1,"+jetId_WP+","+pileupjetId_WP+")"),
        bveto      = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",2.1,'trackCountingHighEffBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3) == 0"),
        bveto_ip   = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",2.1,'trackCountingHighEffBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0"),
        bveto_nj   = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",2.1,'trackCountingHighEffBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,1) == 0"),      
        bveto_nj30 = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",2.1,'trackCountingHighEffBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,30) == 0"),
        bveto_nj05 = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",2.1,'trackCountingHighEffBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,1,0.5) == 0"),
        bveto_nj3005 = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",2.1,'trackCountingHighEffBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,30,0.5) == 0"),
        bveto_csvm    = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",0.679,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3) == 0"),
        bveto_csvm_ip = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",0.679,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0"),
        bveto_csvm_nj = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",0.679,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,1) == 0"),
        bveto_csvm_nj30 = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",0.679,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,30) == 0"),
        bveto_csvm_nj05 = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",0.679,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,1,0.5) == 0"),
        bveto_csvm_nj3005 = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",0.679,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,30,0.5) == 0"),
        bveto_csvl    = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",0.244,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3) == 0"),
        bveto_csvl_ip = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",0.244,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0"),
        bveto_csvl_nj = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",0.244,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,1) == 0"),
        bveto_csvl_nj30 = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",0.244,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,30) == 0"),
        bveto_csvl_nj05 = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",0.244,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,1,0.5) == 0"),
        bveto_csvl_nj3005 = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",0.244,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,30,0.5) == 0"),
        bveto_csvt    = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",0.898,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3) == 0"),
        bveto_csvt_ip = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",0.898,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0"),
        bveto_csvt_nj = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",0.898,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,1) == 0"),
        bveto_csvt_nj30 = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",0.898,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,30) == 0"),
        bveto_csvt_nj05 = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",0.898,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,1,0.5) == 0"),
        bveto_csvt_nj3005 = cms.string("bTaggedJetsBetween("+minPtBVeto+","+CJVminPt+",0.898,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,30,0.5) == 0"),   
        # tagged jets for low pt jets with or without soft muons
    )
 )

 setattr(process.stepBTree.variables,"v_jet1",cms.string("jet(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")")) 
 setattr(process.stepBTree.variables,"v_jet2",cms.string("jet(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"v_jet3",cms.string("jet(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"v_jet4",cms.string("jet(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))         

 setattr(process.stepBTree.variables,"njet"  ,cms.string("nCentralJets("+CJVminPt+","+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")")) 
 setattr(process.stepBTree.variables,"njetid",cms.string("nCentralJets("+CJVminPt+","+CJVmaxEta+",1,0,0)"))
 
 setattr(process.stepBTree.variables,"nbjet",cms.string("bTaggedJetsOver("+CJVminPt+",1.03,'jetBProbabilityBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"nbjettche",cms.string("bTaggedJetsOver("+CJVminPt+",2.1,'trackCountingHighEffBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))    
 setattr(process.stepBTree.variables,"nbjettchp",cms.string("bTaggedJetsOver("+CJVminPt+",1.93,'trackCountingHighPurBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"nbjetcsvl",cms.string("bTaggedJetsOver("+CJVminPt+",0.244,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"nbjetcsvm",cms.string("bTaggedJetsOver("+CJVminPt+",0.679,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"nbjetcsvt",cms.string("bTaggedJetsOver("+CJVminPt+",0.898,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"nbjetcsvlMVA",cms.string("bTaggedJetsOver("+CJVminPt+",0.244,'combinedSecondaryVertexMVABJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"nbjetcsvmMVA",cms.string("bTaggedJetsOver("+CJVminPt+",0.679,'combinedSecondaryVertexMVABJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"nbjetcsvtMVA",cms.string("bTaggedJetsOver("+CJVminPt+",0.898,'combinedSecondaryVertexMVABJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 
 
 ### take the jet with higher bdiscriminator value in the range minPt, maxPt in disjoint ranges 
 setattr(process.stepBTree.variables,"softtche",cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'trackCountingHighEffBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"hardtche",cms.string("highestBDiscRange("+CJVminPt+",999999.,'trackCountingHighEffBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+",1)"))
 setattr(process.stepBTree.variables,"softtchp",cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'trackCountingHighPurBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"hardtchp",cms.string("highestBDiscRange("+CJVminPt+",999999.,'trackCountingHighPurBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+",1)"))
 setattr(process.stepBTree.variables,"softbjpb",cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'jetBProbabilityBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"hardbjpb",cms.string("highestBDiscRange("+CJVminPt+",999999.,'jetBProbabilityBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+",1)"))
 setattr(process.stepBTree.variables,"softbcsv",cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"hardbcsv",cms.string("highestBDiscRange("+CJVminPt+",999999.,'combinedSecondaryVertexBJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+",1)"))
 setattr(process.stepBTree.variables,"softbcsvMVA",cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'combinedSecondaryVertexMVABJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"hardbcsvMVA",cms.string("highestBDiscRange("+CJVminPt+",999999.,'combinedSecondaryVertexMVABJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+",1)"))

 
 setattr(process.stepBTree.variables,"worstJetLepPt",cms.string("max(matchedJetPt(0, 0.5)/getPt(0),matchedJetPt(1, 0.5)/getPt(1))"))
 
 # here we don't apply the dz cut, cause we just use the b-tag value of highest pt jets
 setattr(process.stepBTree.variables,"jetpt1" ,      cms.string("leadingJetPt(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jeteta1",      cms.string("leadingJetEta(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetphi1",      cms.string("leadingJetPhi(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetmass1",     cms.string("leadingJetMass(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetid1",       cms.string("leadingJetId(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetpileupidValue1", cms.string("leadingPileUpJetIdValue(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetpileupidFlag1",  cms.string("leadingPileUpJetIdFlag(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetmva1",      cms.string("leadingJetMva(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 
 setattr(process.stepBTree.variables,"jetbjpb1", cms.string("leadingJetBtag(0,'jetBProbabilityBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jettche1", cms.string("leadingJetBtag(0,'trackCountingHighEffBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jettchp1", cms.string("leadingJetBtag(0,'trackCountingHighPurBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jetcsv1",  cms.string("leadingJetBtag(0,'combinedSecondaryVertexBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jetcsvMVA1",cms.string("leadingJetBtag(0,'combinedSecondaryVertexMVABJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 
 #setattr(process.stepBTree.variables,"jetptd1",cms.string("leadingJetPtd(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetCHM1",cms.string("leadingJetChargedHadronMultiplicity(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNHM1",cms.string("leadingJetNeutralHadronMultiplicity(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetPhM1",cms.string("leadingJetPhotonMultiplicity(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetQCptD1",cms.string("leadingJetPtD(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCaxis11",cms.string("leadingJetQGaxis1(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCaxis21",cms.string("leadingJetQGaxis2(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCRMScand1",cms.string("leadingJetQGRMScand(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCRmax1",cms.string("leadingJetQGRmax(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetptD1",cms.string("leadingJetPtD(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetaxis11",cms.string("leadingJetQGaxis1(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetaxis21",cms.string("leadingJetQGaxis2(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetRMScand1",cms.string("leadingJetQGRMScand(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetRmax1",cms.string("leadingJetQGRmax(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetNChgQC1",cms.string("leadingJetNChgQC(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNChgptCut1",cms.string("leadingJetNChgptCut(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNNeutralptCut1",cms.string("leadingJetNNeutralptCut(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))

 
 setattr(process.stepBTree.variables,"jetpt2"   ,cms.string("leadingJetPt(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jeteta2",cms.string("leadingJetEta(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetphi2",cms.string("leadingJetPhi(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetmass2",cms.string("leadingJetMass(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetid2",cms.string("leadingJetId(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetpileupidValue2",cms.string("leadingPileUpJetIdValue(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetpileupidFlag2",cms.string("leadingPileUpJetIdFlag(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetmva2",cms.string("leadingJetMva(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetbjpb2",cms.string("leadingJetBtag(1,'jetBProbabilityBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jettche2",cms.string("leadingJetBtag(1,'trackCountingHighEffBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jettchp2",cms.string("leadingJetBtag(1,'trackCountingHighPurBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jetcsv2",cms.string("leadingJetBtag(1,'combinedSecondaryVertexBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jetcsvMVA2",cms.string("leadingJetBtag(1,'combinedSecondaryVertexMVABJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))

 #setattr(process.stepBTree.variables,"jetptd2",cms.string("leadingJetPtd(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetCHM2",cms.string("leadingJetChargedHadronMultiplicity(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNHM2",cms.string("leadingJetNeutralHadronMultiplicity(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetPhM2",cms.string("leadingJetPhotonMultiplicity(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetQCptD2",cms.string("leadingJetPtD(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCaxis12",cms.string("leadingJetQGaxis1(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCaxis22",cms.string("leadingJetQGaxis2(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCRMScand2",cms.string("leadingJetQGRMScand(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCRmax2",cms.string("leadingJetQGRmax(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetptD2",cms.string("leadingJetPtD(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetaxis12",cms.string("leadingJetQGaxis1(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetaxis22",cms.string("leadingJetQGaxis2(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetRMScand2",cms.string("leadingJetQGRMScand(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetRmax2",cms.string("leadingJetQGRmax(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetNChgQC2",cms.string("leadingJetNChgQC(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNChgptCut2",cms.string("leadingJetNChgptCut(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNNeutralptCut2",cms.string("leadingJetNNeutralptCut(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))

 
 setattr(process.stepBTree.variables,"jetpt3",cms.string("leadingJetPt(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jeteta3",cms.string("leadingJetEta(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetphi3",cms.string("leadingJetPhi(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetmass3",cms.string("leadingJetMass(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetid3",cms.string("leadingJetId(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetpileupidValue3",cms.string("leadingPileUpJetIdValue(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetpileupidFlag3",cms.string("leadingPileUpJetIdFlag(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetmva3",cms.string("leadingJetMva(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetbjpb3",cms.string("leadingJetBtag(2,'jetBProbabilityBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jettche3",cms.string("leadingJetBtag(2,'trackCountingHighEffBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jettchp3",cms.string("leadingJetBtag(2,'trackCountingHighPurBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jetcsv3",cms.string("leadingJetBtag(2,'combinedSecondaryVertexBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jetcsvMVA3",cms.string("leadingJetBtag(2,'combinedSecondaryVertexMVABJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))

 #setattr(process.stepBTree.variables,"jetptd3",cms.string("leadingJetPtd(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetCHM3",cms.string("leadingJetChargedHadronMultiplicity(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNHM3",cms.string("leadingJetNeutralHadronMultiplicity(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetPhM3",cms.string("leadingJetPhotonMultiplicity(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetQCptD3",cms.string("leadingJetPtD(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCaxis13",cms.string("leadingJetQGaxis1(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCaxis23",cms.string("leadingJetQGaxis2(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCRMScand3",cms.string("leadingJetQGRMScand(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCRmax3",cms.string("leadingJetQGRmax(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetptD3",cms.string("leadingJetPtD(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetaxis13",cms.string("leadingJetQGaxis1(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetaxis23",cms.string("leadingJetQGaxis2(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetRMScand3",cms.string("leadingJetQGRMScand(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetRmax3",cms.string("leadingJetQGRmax(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetNChgQC3",cms.string("leadingJetNChgQC(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNChgptCut3",cms.string("leadingJetNChgptCut(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNNeutralptCut3",cms.string("leadingJetNNeutralptCut(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))

 
 setattr(process.stepBTree.variables,"jetpt4",cms.string("leadingJetPt(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jeteta4",cms.string("leadingJetEta(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetphi4",cms.string("leadingJetPhi(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetmass4",cms.string("leadingJetMass(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetid4",cms.string("leadingJetId(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetpileupidValue4",cms.string("leadingPileUpJetIdValue(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetpileupidFlag4",cms.string("leadingPileUpJetIdFlag(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetmva4",cms.string("leadingJetMva(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetbjpb4",cms.string("leadingJetBtag(3,'jetBProbabilityBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jettche4",cms.string("leadingJetBtag(3,'trackCountingHighEffBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jettchp4",cms.string("leadingJetBtag(3,'trackCountingHighPurBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jetcsv4",cms.string("leadingJetBtag(3,'combinedSecondaryVertexBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jetcsvMVA4",cms.string("leadingJetBtag(3,'combinedSecondaryVertexMVABJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 #setattr(process.stepBTree.variables,"jetptd4",cms.string("leadingJetPtd(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetCHM4",cms.string("leadingJetChargedHadronMultiplicity(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNHM4",cms.string("leadingJetNeutralHadronMultiplicity(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetPhM4",cms.string("leadingJetPhotonMultiplicity(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetQCptD4",cms.string("leadingJetPtD(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCaxis14",cms.string("leadingJetQGaxis1(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCaxis24",cms.string("leadingJetQGaxis2(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCRMScand4",cms.string("leadingJetQGRMScand(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCRmax4",cms.string("leadingJetQGRmax(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetptD4",cms.string("leadingJetPtD(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetaxis14",cms.string("leadingJetQGaxis1(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetaxis24",cms.string("leadingJetQGaxis2(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetRMScand4",cms.string("leadingJetQGRMScand(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetRmax4",cms.string("leadingJetQGRmax(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetNChgQC4",cms.string("leadingJetNChgQC(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNChgptCut4",cms.string("leadingJetNChgptCut(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNNeutralptCut4",cms.string("leadingJetNNeutralptCut(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 
 setattr(process.stepBTree.variables,"jetRho",cms.string("getJetRhoIso()")) 
 setattr(process.stepBTree.variables,"pfSumEt",cms.string("pfSumEt()"))
 setattr(process.stepBTree.variables,"pfmet",cms.string("pfMet()"))
 setattr(process.stepBTree.variables,"pfmetphi",cms.string("pfMetPhi()"))
 setattr(process.stepBTree.variables,"ppfmet",cms.string("projPfMet()"))
 setattr(process.stepBTree.variables,"pfmetMEtSig",cms.string("pfMetMEtSig()"))
 setattr(process.stepBTree.variables,"pfmetSignificance",cms.string("pfMetSignificance()"))

 setattr(process.stepBTree.variables,"dphilljet",cms.string("dPhillLeadingJet("+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"dphilljetjet",cms.string("dPhilljetjet("+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))

 setattr(process.stepBTree.variables,"dphillmet",cms.string("dPhillMet('PFMET')"))
 setattr(process.stepBTree.variables,"dphilmet",cms.string("dPhilMet('PFMET')"))
 setattr(process.stepBTree.variables,"dphilmet1",cms.string("dPhilMetByPt(0,'PFMET')"))
 setattr(process.stepBTree.variables,"dphilmet2",cms.string("dPhilMetByPt(1,'PFMET')"))
 setattr(process.stepBTree.variables,"mtw1",cms.string("mTByPt(0,'PFMET')"))
 setattr(process.stepBTree.variables,"mtw2",cms.string("mTByPt(1,'PFMET')"))
 setattr(process.stepBTree.variables,"mth",cms.string("mTHiggs('PFMET')"))
 setattr(process.stepBTree.variables,"puW",cms.string("1")) ## pile up weight 
 #setattr(process.stepBTree.variables,"kfW",cms.InputTag("ptWeight")) ## pt weight 
 setattr(process.stepBTree.variables,"baseW",cms.string("1")) 
 setattr(process.stepBTree.variables,"fourW",cms.string("1"))
 setattr(process.stepBTree.variables,"fermiW",cms.string("1"))
 setattr(process.stepBTree.variables,"effAW",cms.string("1"))
 setattr(process.stepBTree.variables,"effBW",cms.string("1"))
 setattr(process.stepBTree.variables,"effW",cms.string("1"))
 setattr(process.stepBTree.variables,"triggAW",cms.string("1"))
 setattr(process.stepBTree.variables,"triggBW",cms.string("1"))
 setattr(process.stepBTree.variables,"triggW",cms.string("1"))
 setattr(process.stepBTree.variables,"fakeAW",cms.string("1"))
 setattr(process.stepBTree.variables,"fakeBW",cms.string("1"))
 setattr(process.stepBTree.variables,"fakeW",cms.string("1"))

 setattr(process.stepBTree.variables,"trpu",cms.InputTag("nPU:tr"))
 setattr(process.stepBTree.variables,"itpu",cms.InputTag("nPU:it"))
 setattr(process.stepBTree.variables,"ootpup1",cms.InputTag("nPU:p1"))
 setattr(process.stepBTree.variables,"ootpum1",cms.InputTag("nPU:m1"))

 setattr(process.stepBTree.variables,"njetvbf",cms.string("nJetVBF(30,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"mjj",cms.string("mjj(0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"detajj",cms.string("dEtajj(30,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetpt1",cms.string("leadingVBFJetPt(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjeteta1",cms.string("leadingVBFJetEta(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetphi1",cms.string("leadingVBFJetPhi(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetid1",cms.string("leadingVBFJetId(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetpileupidValue1",cms.string("leadingVBFPileUpJetIdValue(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetpileupidFlag1",cms.string("leadingVBFPileUpJetIdFlag(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetmva1",cms.string("leadingVBFJetMva(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetpt2",cms.string("leadingVBFJetPt(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjeteta2",cms.string("leadingVBFJetEta(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetphi2",cms.string("leadingVBFJetPhi(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetid2",cms.string("leadingVBFJetId(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetpileupidValue2",cms.string("leadingVBFPileUpJetIdValue(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetpileupidFlag2",cms.string("leadingVBFPileUpJetIdFlag(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetmva2",cms.string("leadingVBFJetMva(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))

 setattr(process.stepBTree.variables,"mctruth",cms.string("-1"))
 
#######################
#######################
#######################

def addPuppiVariables(process,jetId_WP = "0",pileupjetId_WP = "1", CJVminPt  = "30.", CJVmaxEta = "5.0", DphiJetVetominPt  = "15.", DphiJetVetominEta = "5.0",
                      DzBVeto    = "999999.9",minPtBVeto = "10.0"):


 setattr(process.stepBTree.variables,"v_jet1_puppi",cms.string("puppiJet(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"v_jet2_puppi",cms.string("puppiJet(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"v_jet3_puppi",cms.string("puppiJet(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"v_jet4_puppi",cms.string("puppiJet(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 
 setattr(process.stepBTree.variables,"njet_puppi",cms.string("nCentralPuppiJets("+CJVminPt+","+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"njetid_puppi",cms.string("nCentralPuppiJets("+CJVminPt+","+CJVmaxEta+",1,0,0)"))

 setattr(process.stepBTree.variables,"nbjettche_puppi",cms.string("bTaggedPuppiJetsOver("+CJVminPt+",2.1,'AK5trackCountingHighEffbPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"nbjet_puppi",        cms.string("bTaggedPuppiJetsOver("+CJVminPt+",1.05,'AK5jetBProbabilityBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"nbjettchp_puppi",cms.string("bTaggedPuppiJetsOver("+CJVminPt+",1.93,'AK5trackCountingHighPurBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"nbjetcsvl_puppi",cms.string("bTaggedPuppiJetsOver("+CJVminPt+",0.244,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"nbjetcsvm_puppi",cms.string("bTaggedPuppiJetsOver("+CJVminPt+",0.679,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"nbjetcsvt_puppi",cms.string("bTaggedPuppiJetsOver("+CJVminPt+",0.898,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"nbjetcsvlMVA_puppi",cms.string("bTaggedPuppiJetsOver("+CJVminPt+",0.244,'AK5combinedSecondaryVertexMVABPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"nbjetcsvmMVA_puppi",cms.string("bTaggedPuppiJetsOver("+CJVminPt+",0.679,'AK5combinedSecondaryVertexMVABPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"nbjetcsvtMVA_puppi",cms.string("bTaggedPuppiJetsOver("+CJVminPt+",0.898,'AK5combinedSecondaryVertexMVABPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))

 setattr(process.stepBTree.variables,"jetpt_puppi1",cms.string("leadingPuppiJetPt(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jeteta_puppi1",cms.string("leadingPuppiJetEta(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetphi_puppi1",cms.string("leadingPuppiJetPhi(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetmass_puppi1",cms.string("leadingPuppiJetMass(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetid_puppi1",cms.string("leadingPuppiJetId(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetpileupidValue_puppi1",cms.string("leadingPuppiPileUpJetIdValue(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetpileupidFlag_puppi1",cms.string("leadingPuppiPileUpJetIdFlag(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetmva_puppi1",cms.string("leadingPuppiJetMva(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetbjpb_puppi1",cms.string("leadingPuppiJetBtag(0,'jetBProbabilityBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jettche_puppi1",cms.string("leadingPuppiJetBtag(0,'trackCountingHighEffbPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jettchp_puppi1",cms.string("leadingPuppiJetBtag(0,'AK5trackCountingHighPurBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jetcsv_puppi1",cms.string("leadingPuppiJetBtag(0,'AK5combinedSecondaryVertexBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jetcsvMVA_puppi1",cms.string("leadingPuppiJetBtag(0,'AK5combinedSecondaryVertexMVABPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))

 #setattr(process.stepBTree.variables,"jetptd1_puppi",cms.string("leadingPuppiJetPtd(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetCHM1_puppi",cms.string("leadingPuppiJetChargedHadronMultiplicity(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNHM1_puppi",cms.string("leadingPuppiJetNeutralHadronMultiplicity(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetPhM1_puppi",cms.string("leadingPuppiJetPhotonMultiplicity(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetQCptD1_puppi",cms.string("leadingPuppiJetPtD(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCaxis11_puppi",cms.string("leadingPuppiJetQGaxis1(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCaxis21_puppi",cms.string("leadingPuppiJetQGaxis2(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCRMScand1_puppi",cms.string("leadingPuppiJetQGRMScand(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCRmax1_puppi",cms.string("leadingPuppiJetQGRmax(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetptD1_puppi",cms.string("leadingPuppiJetPtD(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetaxis11_puppi",cms.string("leadingPuppiJetQGaxis1(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetaxis21_puppi",cms.string("leadingPuppiJetQGaxis2(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetRMScand1_puppi",cms.string("leadingPuppiJetQGRMScand(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetRmax1_puppi",cms.string("leadingPuppiJetQGRmax(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetNChgQC1_puppi",cms.string("leadingPuppiJetNChgQC(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNChgptCut1_puppi",cms.string("leadingPuppiJetNChgptCut(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNNeutralptCut1_puppi",cms.string("leadingPuppiJetNNeutralptCut(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))

 setattr(process.stepBTree.variables,"jetpt_puppi2",cms.string("leadingPuppiJetPt(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jeteta_puppi2",cms.string("leadingPuppiJetEta(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetphi_puppi2",cms.string("leadingPuppiJetPhi(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetmass_puppi2",cms.string("leadingPuppiJetMass(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetid_puppi2",cms.string("leadingPuppiJetId(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetpileupidValue_puppi2",cms.string("leadingPuppiPileUpJetIdValue(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetpileupidFlag_puppi2",cms.string("leadingPuppiPileUpJetIdFlag(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetmva_puppi2",cms.string("leadingPuppiJetMva(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetbjpb_puppi2",cms.string("leadingPuppiJetBtag(1,'jetBProbabilityBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jettche_puppi2",cms.string("leadingPuppiJetBtag(1,'trackCountingHighEffbPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jettchp_puppi2",cms.string("leadingPuppiJetBtag(1,'AK5trackCountingHighPurBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jetcsv_puppi2",cms.string("leadingPuppiJetBtag(1,'AK5combinedSecondaryVertexBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jetcsvMVA_puppi2",cms.string("leadingPuppiJetBtag(1,'AK5combinedSecondaryVertexMVABPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))

 #setattr(process.stepBTree.variables,"jetptd2_puppi",cms.string("leadingPuppiJetPtd(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetCHM2_puppi",cms.string("leadingPuppiJetChargedHadronMultiplicity(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNHM2_puppi",cms.string("leadingPuppiJetNeutralHadronMultiplicity(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetPhM2_puppi",cms.string("leadingPuppiJetPhotonMultiplicity(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetQCptD2_puppi",cms.string("leadingPuppiJetPtD(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCaxis12_puppi",cms.string("leadingPuppiJetQGaxis1(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCaxis22_puppi",cms.string("leadingPuppiJetQGaxis2(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCRMScand2_puppi",cms.string("leadingPuppiJetQGRMScand(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCRmax2_puppi",cms.string("leadingPuppiJetQGRmax(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetptD2_puppi",cms.string("leadingPuppiJetPtD(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetaxis12_puppi",cms.string("leadingPuppiJetQGaxis1(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetaxis22_puppi",cms.string("leadingPuppiJetQGaxis2(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetRMScand2_puppi",cms.string("leadingPuppiJetQGRMScand(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetRmax2_puppi",cms.string("leadingPuppiJetQGRmax(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetNChgQC2_puppi",cms.string("leadingPuppiJetNChgQC(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNChgptCut2_puppi",cms.string("leadingPuppiJetNChgptCut(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNNeutralptCut2_puppi",cms.string("leadingPuppiJetNNeutralptCut(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))

 setattr(process.stepBTree.variables,"jetpt_puppi3",cms.string("leadingPuppiJetPt(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jeteta_puppi3",cms.string("leadingPuppiJetEta(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetphi_puppi3",cms.string("leadingPuppiJetPhi(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetmass_puppi3",cms.string("leadingPuppiJetMass(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetpileupidValue_puppi3",cms.string("leadingPuppiPileUpJetIdValue(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetpileupidFlag_puppi3",cms.string("leadingPuppiPileUpJetIdFlag(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetmva_puppi3",cms.string("leadingPuppiJetMva(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetbjpb_puppi3",cms.string("leadingPuppiJetBtag(2,'jetBProbabilityBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jettche_puppi3",cms.string("leadingPuppiJetBtag(2,'trackCountingHighEffbPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jettchp_puppi3",cms.string("leadingPuppiJetBtag(2,'AK5trackCountingHighPurBJetTagsPuppi',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jetcsv_puppi3",cms.string("leadingPuppiJetBtag(2,'AK5combinedSecondaryVertexBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jetcsvMVA_puppi3",cms.string("leadingPuppiJetBtag(2,'AK5combinedSecondaryVertexMVABPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))

 #setattr(process.stepBTree.variables,"jetptd3_puppi",cms.string("leadingPuppiJetPtd(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetCHM3_puppi",cms.string("leadingPuppiJetChargedHadronMultiplicity(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNHM3_puppi",cms.string("leadingPuppiJetNeutralHadronMultiplicity(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetPhM3_puppi",cms.string("leadingPuppiJetPhotonMultiplicity(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetQCptD3_puppi",cms.string("leadingPuppiJetPtD(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCaxis13_puppi",cms.string("leadingPuppiJetQGaxis1(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCaxis23_puppi",cms.string("leadingPuppiJetQGaxis2(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCRMScand3_puppi",cms.string("leadingPuppiJetQGRMScand(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCRmax3_puppi",cms.string("leadingPuppiJetQGRmax(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetptD3_puppi",cms.string("leadingPuppiJetPtD(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetaxis13_puppi",cms.string("leadingPuppiJetQGaxis1(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetaxis23_puppi",cms.string("leadingPuppiJetQGaxis2(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetRMScand3_puppi",cms.string("leadingPuppiJetQGRMScand(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetRmax3_puppi",cms.string("leadingPuppiJetQGRmax(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetNChgQC3_puppi",cms.string("leadingPuppiJetNChgQC(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNChgptCut3_puppi",cms.string("leadingPuppiJetNChgptCut(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNNeutralptCut3_puppi",cms.string("leadingPuppiJetNNeutralptCut(2,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))

 setattr(process.stepBTree.variables,"jetpt_puppi4",cms.string("leadingPuppiJetPt(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jeteta_puppi4",cms.string("leadingPuppiJetEta(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetphi_puppi4",cms.string("leadingPuppiJetPhi(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetmass_puppi4",cms.string("leadingPuppiJetMass(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetid_puppi4",cms.string("leadingPuppiJetId(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetpileupidValue_puppi4",cms.string("leadingPuppiPileUpJetIdValue(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetpileupidFlag_puppi4",cms.string("leadingPuppiPileUpJetIdFlag(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetmva_puppi4",cms.string("leadingPuppiJetMva(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"jetbjpb_puppi4",cms.string("leadingPuppiJetBtag(3,'jetBProbabilityBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jettche_puppi4",cms.string("leadingPuppiJetBtag(3,'trackCountingHighEffbPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jettchp_puppi4",cms.string("leadingPuppiJetBtag(3,'AK5trackCountingHighPurBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jetcsv_puppi4",cms.string("leadingPuppiJetBtag(3,'AK5combinedSecondaryVertexBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
 setattr(process.stepBTree.variables,"jetcsvMVA_puppi4",cms.string("leadingPuppiJetBtag(3,'AK5combinedSecondaryVertexMVABPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))

 #setattr(process.stepBTree.variables,"jetptd4_puppi",cms.string("leadingPuppiJetPtd(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetCHM4_puppi",cms.string("leadingPuppiJetChargedHadronMultiplicity(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNHM4_puppi",cms.string("leadingPuppiJetNeutralHadronMultiplicity(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetPhM4_puppi",cms.string("leadingPuppiJetPhotonMultiplicity(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetQCptD4_puppi",cms.string("leadingPuppiJetPtD(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCaxis14_puppi",cms.string("leadingPuppiJetQGaxis1(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCaxis24_puppi",cms.string("leadingPuppiJetQGaxis2(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCRMScand4_puppi",cms.string("leadingPuppiJetQGRMScand(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetQCRmax4_puppi",cms.string("leadingPuppiJetQGRmax(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
 #setattr(process.stepBTree.variables,"jetptD4_puppi",cms.string("leadingPuppiJetPtD(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetaxis14_puppi",cms.string("leadingPuppiJetQGaxis1(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetaxis24_puppi",cms.string("leadingPuppiJetQGaxis2(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetRMScand4_puppi",cms.string("leadingPuppiJetQGRMScand(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetRmax4_puppi",cms.string("leadingPuppiJetQGRmax(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
 #setattr(process.stepBTree.variables,"jetNChgQC4_puppi",cms.string("leadingPuppiJetNChgQC(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNChgptCut4_puppi",cms.string("leadingPuppiJetNChgptCut(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 #setattr(process.stepBTree.variables,"jetNNeutralptCut4_puppi",cms.string("leadingPuppiJetNNeutralptCut(3,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))

 setattr(process.stepBTree.variables,"pfSumEt_puppi",cms.string("pfPuppiSumEt"))
 setattr(process.stepBTree.variables,"pfmet_puppi",cms.string("pfPuppiMet"))
 setattr(process.stepBTree.variables,"pfmetphi_puppi",cms.string("pfPuppiMetPhi"))
 setattr(process.stepBTree.variables,"ppfmet_puppi",cms.string("projPuppiPfMet"))
 setattr(process.stepBTree.variables,"dphillJet_puppi",cms.string("dPhillLeadingPuppiJet("+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"dphilljetjet_puppi",cms.string("dPhillPuppijetjet("+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))

 setattr(process.stepBTree.variables,"dphillMet_puppi",cms.string("dPhillMet('PUPPIMET')"))
 setattr(process.stepBTree.variables,"dphilMet_puppi",cms.string("dPhilMet('PUPPIMET')"))
 setattr(process.stepBTree.variables,"dphilMet1_puppi",cms.string("dPhilMetByPt(0,'PUPPIMET')"))
 setattr(process.stepBTree.variables,"dphilMet2_puppi",cms.string("dPhilMetByPt(1,'PUPPIMET')"))


 setattr(process.stepBTree.variables,"mtw1_puppi",cms.string("mTByPt(0,'PUPPIMET')"))
 setattr(process.stepBTree.variables,"mtw2_puppi",cms.string("mTByPt(1,'PUPPIMET')"))
 setattr(process.stepBTree.variables,"mth_puppi",cms.string("mTHiggs('PUPPIMET')"))

 setattr(process.stepBTree.variables,"njetvb_puppi",cms.string("nPuppiJetVBF(30,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"mjj_puppi",cms.string("puppiMjj(0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"detajj_puppi",cms.string("puppidEtajj(30,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetpt1_puppi",cms.string("leadingVBFPuppiJetPt(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))

 setattr(process.stepBTree.variables,"cjeteta1_puppi",cms.string("leadingVBFPuppiJetEta(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetphi1_puppi",cms.string("leadingVBFPuppiJetPhi(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetid1_puppi",cms.string("leadingVBFPuppiJetId(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetpileupidValue1_puppi",cms.string("leadingVBFPuppiPileUpJetIdValue(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetpileupidFlag1_puppi",cms.string("leadingVBFPuppiPileUpJetIdFlag(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetmva1_puppi",cms.string("leadingVBFPuppiJetMva(0,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))

 setattr(process.stepBTree.variables,"cjetpt2_puppi",cms.string("leadingVBFPuppiJetPt(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjeteta2_puppi",cms.string("leadingVBFPuppiJetEta(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetphi2_puppi",cms.string("leadingVBFPuppiJetPhi(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetid2_puppi",cms.string("leadingVBFPuppiJetId(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetpileupidValue2_puppi",cms.string("leadingVBFPuppiPileUpJetIdValue(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetpileupidFlag2_puppi",cms.string("leadingVBFPuppiPileUpJetIdFlag(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 setattr(process.stepBTree.variables,"cjetmva2_puppi",cms.string("leadingVBFPuppiJetMva(1,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))


 setattr(process.stepBTree.flags,"bveto_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",2.1,'trackCountingHighEffbPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3) == 0"))
 setattr(process.stepBTree.flags,"bveto_ip_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",2.1,'trackCountingHighEffbPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0"))
 setattr(process.stepBTree.flags,"bveto_nj_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",2.1,'trackCountingHighEffbPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,1) == 0"))
 setattr(process.stepBTree.flags,"bveto_nj30_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",2.1,'trackCountingHighEffbPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,30) == 0"))
 setattr(process.stepBTree.flags,"bveto_nj05_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",2.1,'trackCountingHighEffbPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,1,0.5) == 0"))
 setattr(process.stepBTree.flags,"bveto_nj3005_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",2.1,'trackCountingHighEffbPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,30,0.5) == 0"))

 setattr(process.stepBTree.flags,"bveto_csvm_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",0.679,'AK5combinedSecondaryVertexBPuppiPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3) == 0"))
 setattr(process.stepBTree.flags,"bveto_csvm_ip_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",0.679,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0"))
 setattr(process.stepBTree.flags,"bveto_csvm_nj_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",0.679,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,1) == 0"))
 setattr(process.stepBTree.flags,"bveto_csvm_nj30_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",0.679,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,30) == 0"))
 setattr(process.stepBTree.flags,"bveto_csvm_nj05_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",0.679,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,1,0.5) == 0"))
 setattr(process.stepBTree.flags,"bveto_csvm_nj3005_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",0.679,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,30,0.5) == 0"))


 setattr(process.stepBTree.flags,"bveto_csvl_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",0.244,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3) == 0"))
 setattr(process.stepBTree.flags,"bveto_csvl_ip_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",0.244,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0"))
 setattr(process.stepBTree.flags,"bveto_csvl_nj_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",0.244,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,1) == 0"))
 setattr(process.stepBTree.flags,"bveto_csvl_nj30_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",0.244,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,30) == 0"))
 setattr(process.stepBTree.flags,"bveto_csvl_nj05_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",0.244,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,1,0.5) == 0"))
 setattr(process.stepBTree.flags,"bveto_csvl_nj3005_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",0.244,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,30,0.5) == 0"))


 setattr(process.stepBTree.flags,"bveto_csvt_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",0.898,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3) == 0"))
 setattr(process.stepBTree.flags,"bveto_csvt_ip_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",0.898,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0"))
 setattr(process.stepBTree.flags,"bveto_csvt_nj_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",0.898,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,1) == 0"))
 setattr(process.stepBTree.flags,"bveto_csvt_nj30_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",0.898,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,30) == 0"))
 setattr(process.stepBTree.flags,"bveto_csvt_nj05_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",0.898,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,1,0.5) == 0"))
 setattr(process.stepBTree.flags,"bveto_csvt_nj3005_puppi",cms.string("bTaggedPuppiJetsBetween("+minPtBVeto+","+CJVminPt+",0.898,'AK5combinedSecondaryVertexBPuppiJetTags',"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+") == 0 && getNSoftMu(3,30,0.5) == 0"))


 setattr(process.stepBTree.flags,"dphiveto",cms.string("passesDPhillPuppiJet("+DphiJetVetominPt+","+DphiJetVetominEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
 

#######################
#######################
#######################

def addEleIdIsoVariables(process,pt):
    if hasattr(pt,"variables"):      
        setattr(pt.variables, "std_vector_deltaEtaIn" ,    cms.string("deltaEtaSuperClusterTrackAtVtxByPt")),
        setattr(pt.variables, "std_vector_deltaPhiIn" ,    cms.string("deltaPhiSuperClusterTrackAtVtxByPt")),
        setattr(pt.variables, "std_vector_sigmaIetaIeta" , cms.string("sigmaIetaIetaByPt")),
        setattr(pt.variables, "std_vector_HoE" ,           cms.string("hadronicOverEmByPt")),
        setattr(pt.variables, "std_vector_numHits" ,       cms.string("numberOfHitsByPt")),
    else:
        raise RuntimeError, "In addEleIdIsoVariables, %s doesn't look like a ProbeTreeProducer object, it has no 'variables' attribute." % pt


#######################
#######################
#######################

def addLHEVariables(process,pt):
    if hasattr(pt,"variables"):
        setattr(pt.variables, "jetLHEPartonpt1" , cms.string("leadingLHEJetPt(0)")),
        setattr(pt.variables, "jetLHEPartonpid1" , cms.string("leadingLHEJetPID(0)")),
        setattr(pt.variables, "jetLHEPartonphi1" , cms.string("leadingLHEJetPhi(0)")),
        setattr(pt.variables, "jetLHEPartoneta1" , cms.string("leadingLHEJetEta(0)")),
        setattr(pt.variables, "jetLHEPartonpt2" , cms.string("leadingLHEJetPt(1)")),
        setattr(pt.variables, "jetLHEPartonpid2" , cms.string("leadingLHEJetPID(1)")),
        setattr(pt.variables, "jetLHEPartonphi2" , cms.string("leadingLHEJetPhi(1)")),
        setattr(pt.variables, "jetLHEPartoneta2" , cms.string("leadingLHEJetEta(1)")),
        setattr(pt.variables, "jetLHEPartonpt3" , cms.string("leadingLHEJetPt(2)")),
        setattr(pt.variables, "jetLHEPartonpid3" , cms.string("leadingLHEJetPID(2)")),
        setattr(pt.variables, "jetLHEPartonphi3" , cms.string("leadingLHEJetPhi(2)")),
        setattr(pt.variables, "jetLHEPartoneta3" , cms.string("leadingLHEJetEta(2)")),
        setattr(pt.variables, "leptonLHEpt1" , cms.string("leadingLHELeptonPt(0)")),
        setattr(pt.variables, "leptonLHEpid1" , cms.string("leadingLHELeptonPID(0)")),
        setattr(pt.variables, "leptonLHEphi1" , cms.string("leadingLHELeptonPhi(0)")),
        setattr(pt.variables, "leptonLHEeta1" , cms.string("leadingLHELeptonEta(0)")),
        setattr(pt.variables, "leptonLHEpt2" , cms.string("leadingLHELeptonPt(1)")),
        setattr(pt.variables, "leptonLHEpid2" , cms.string("leadingLHELeptonPID(1)")),
        setattr(pt.variables, "leptonLHEphi2" , cms.string("leadingLHELeptonPhi(1)")),
        setattr(pt.variables, "leptonLHEeta2" , cms.string("leadingLHELeptonEta(1)")),
        setattr(pt.variables, "leptonLHEpt3" , cms.string("leadingLHELeptonPt(2)")),
        setattr(pt.variables, "leptonLHEpid3" , cms.string("leadingLHELeptonPID(2)")),
        setattr(pt.variables, "leptonLHEphi3" , cms.string("leadingLHELeptonPhi(2)")),
        setattr(pt.variables, "leptonLHEeta3" , cms.string("leadingLHELeptonEta(2)")),
        setattr(pt.variables, "neutrinoLHEpt1" , cms.string("leadingLHENeutrinoPt(0)")),
        setattr(pt.variables, "neutrinoLHEpid1" , cms.string("leadingLHENeutrinoPID(0)")),
        setattr(pt.variables, "neutrinoLHEphi1" , cms.string("leadingLHENeutrinoPhi(0)")),
        setattr(pt.variables, "neutrinoLHEeta1" , cms.string("leadingLHENeutrinoEta(0)")),
        setattr(pt.variables, "neutrinoLHEpt2" , cms.string("leadingLHENeutrinoPt(1)")),
        setattr(pt.variables, "neutrinoLHEpid2" , cms.string("leadingLHENeutrinoPID(1)")),
        setattr(pt.variables, "neutrinoLHEphi2" , cms.string("leadingLHENeutrinoPhi(1)")),
        setattr(pt.variables, "neutrinoLHEeta2" , cms.string("leadingLHENeutrinoEta(1)")),
        setattr(pt.variables, "neutrinoLHEpt3" , cms.string("leadingLHENeutrinoPt(2)")),
        setattr(pt.variables, "neutrinoLHEpid3" , cms.string("leadingLHENeutrinoPID(2)")),
        setattr(pt.variables, "neutrinoLHEphi3" , cms.string("leadingLHENeutrinoPhi(2)")),
        setattr(pt.variables, "neutrinoLHEeta3" , cms.string("leadingLHENeutrinoEta(2)")),

        setattr(pt.variables, "metLHEpt" , cms.string("LHEMetPt()")),
        setattr(pt.variables, "metLHEphi" , cms.string("LHEMetPhi()")),
        setattr(pt.variables, "metLHEeta" , cms.string("LHEMetEta()")),

        setattr(pt.variables, "higgsLHEpt" , cms.string("higgsLHEPt()")),

    else:
        raise addLHEVariables, "In addLHEVariables, %s doesn't look like a ProbeTreeProducer object, it has no 'variables' attribute." % pt

###################
###################
###################

def addGenVariables(process,pt):
    if hasattr(pt,"variables"):
        setattr(pt.variables, "jetGenPartonpt1" , cms.string("leadingGenJetPartonPt(0)")),
        setattr(pt.variables, "jetGenPartonpid1" , cms.string("leadingGenJetPartonPID(0)")),
        setattr(pt.variables, "jetGenPartonphi1" , cms.string("leadingGenJetPartonPhi(0)")),
        setattr(pt.variables, "jetGenPartoneta1" , cms.string("leadingGenJetPartonEta(0)")),
        setattr(pt.variables, "jetGenPartonpt2" , cms.string("leadingGenJetPartonPt(1)")),
        setattr(pt.variables, "jetGenPartonpid2" , cms.string("leadingGenJetPartonPID(1)")),
        setattr(pt.variables, "jetGenPartonphi2" , cms.string("leadingGenJetPartonPhi(1)")),
        setattr(pt.variables, "jetGenPartoneta2" , cms.string("leadingGenJetPartonEta(1)")),
        setattr(pt.variables, "jetGenPartonpt3" , cms.string("leadingGenJetPartonPt(2)")),
        setattr(pt.variables, "jetGenPartonpid3" , cms.string("leadingGenJetPartonPID(2)")),
        setattr(pt.variables, "jetGenPartonphi3" , cms.string("leadingGenJetPartonPhi(2)")),
        setattr(pt.variables, "jetGenPartoneta3" , cms.string("leadingGenJetPartonEta(2)")),

        setattr(pt.variables, "leptonGenpt1" , cms.string("leadingGenLeptonPt(0)")),
        setattr(pt.variables, "leptonGenpid1" , cms.string("leadingGenLeptonPID(0)")),
        setattr(pt.variables, "leptonGenphi1" , cms.string("leadingGenLeptonPhi(0)")),
        setattr(pt.variables, "leptonGeneta1" , cms.string("leadingGenLeptonEta(0)")),
        setattr(pt.variables, "leptonGenpt2" , cms.string("leadingGenLeptonPt(1)")),
        setattr(pt.variables, "leptonGenpid2" , cms.string("leadingGenLeptonPID(1)")),
        setattr(pt.variables, "leptonGenphi2" , cms.string("leadingGenLeptonPhi(1)")),
        setattr(pt.variables, "leptonGeneta2" , cms.string("leadingGenLeptonEta(1)")),
        setattr(pt.variables, "leptonGenpt3" , cms.string("leadingGenLeptonPt(2)")),
        setattr(pt.variables, "leptonGenpid3" , cms.string("leadingGenLeptonPID(2)")),
        setattr(pt.variables, "leptonGenphi3" , cms.string("leadingGenLeptonPhi(2)")),
        setattr(pt.variables, "leptonGeneta3" , cms.string("leadingGenLeptonEta(2)")),

        setattr(pt.variables, "neutrinoGenpt1" , cms.string("leadingGenNeutrinoPt(0)")),
        setattr(pt.variables, "neutrinoGenpid1" , cms.string("leadingGenNeutrinoPID(0)")),
        setattr(pt.variables, "neutrinoGenphi1" , cms.string("leadingGenNeutrinoPhi(0)")),
        setattr(pt.variables, "neutrinoGeneta1" , cms.string("leadingGenNeutrinoEta(0)")),
        setattr(pt.variables, "neutrinoGenpt2" , cms.string("leadingGenNeutrinoPt(1)")),
        setattr(pt.variables, "neutrinoGenpid2" , cms.string("leadingGenNeutrinoPID(1)")),
        setattr(pt.variables, "neutrinoGenphi2" , cms.string("leadingGenNeutrinoPhi(1)")),
        setattr(pt.variables, "neutrinoGeneta2" , cms.string("leadingGenNeutrinoEta(1)")),
        setattr(pt.variables, "neutrinoGenpt3" , cms.string("leadingGenNeutrinoPt(2)")),
        setattr(pt.variables, "neutrinoGenpid3" , cms.string("leadingGenNeutrinoPID(2)")),
        setattr(pt.variables, "neutrinoGenphi3" , cms.string("leadingGenNeutrinoPhi(2)")),
        setattr(pt.variables, "neutrinoGeneta3" , cms.string("leadingGenNeutrinoEta(2)")),

        setattr(pt.variables, "higggsGenpt" , cms.string("getHiggsPt()")),

        setattr(pt.variables, "metGenpt" , cms.string("genMetPt()")),
        setattr(pt.variables, "metGeneta" , cms.string("genMetEta()")),
        setattr(pt.variables, "metGenphi" , cms.string("genMetPhi()")),

        setattr(pt.variables, "jetGenpt1" , cms.string("leadingGenJetPt(0)")),
        setattr(pt.variables, "jetGenphi1" , cms.string("leadingGenJetPhi(0)")),
        setattr(pt.variables, "jetGeneta1" , cms.string("leadingGenJetEta(0)")),
        setattr(pt.variables, "jetGenpt2" , cms.string("leadingGenJetPt(1)")),
        setattr(pt.variables, "jetGenphi2" , cms.string("leadingGenJetPhi(1)")),
        setattr(pt.variables, "jetGeneta2" , cms.string("leadingGenJetEta(1)")),
        setattr(pt.variables, "jetGenpt3" , cms.string("leadingGenJetPt(2)")),
        setattr(pt.variables, "jetGenphi3" , cms.string("leadingGenJetPhi(2)")),
        setattr(pt.variables, "jetGeneta3" , cms.string("leadingGenJetEta(2)")),
        setattr(pt.variables, "jetGenpt4" , cms.string("leadingGenJetPt(3)")),
        setattr(pt.variables, "jetGenphi4" , cms.string("leadingGenJetPhi(3)")),
        setattr(pt.variables, "jetGeneta4" , cms.string("leadingGenJetEta(3)")),
        setattr(pt.variables, "jetGenpt5" , cms.string("leadingGenJetPt(4)")),
        setattr(pt.variables, "jetGenphi5" , cms.string("leadingGenJetPhi(4)")),
        setattr(pt.variables, "jetGeneta5" , cms.string("leadingGenJetEta(4)")),


    else:
        raise addGenVariables, "In addGenVariables, %s doesn't look like a ProbeTreeProducer object, it has no 'variables' attribute." % pt


###################
###################
###################

def addFatJets(process,pt):

        fatjetId_WP = "1"

        if hasattr(pt,"variables"):

            setattr(pt.variables, "fatjetpt1", cms.string("leadingFatJetPt(0,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjeteta1", cms.string("leadingFatJetEta(0,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjetphi1", cms.string("leadingFatJetPhi(0,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjettrimmedmass1", cms.string("leadingFatJetTrimmedMass(0,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjetfilteredmass1", cms.string("leadingFatJetFilteredMass(0,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjetprunedmass1", cms.string("leadingFatJetPrunedMass(0,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjetmassdrop1", cms.string("leadingFatJetMassDrop(0,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjettau2tau11", cms.string("leadingFatJetPrunedTau2Tau1(0,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjettau11", cms.string("leadingFatJetPrunedTau1(0,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjettau21", cms.string("leadingFatJetPrunedTau2(0,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjettau31", cms.string("leadingFatJetPrunedTau3(0,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjettau41", cms.string("leadingFatJetPrunedTau4(0,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),


            setattr(pt.variables, "fatjetpt2", cms.string("leadingFatJetPt(1,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjeteta2", cms.string("leadingFatJetEta(1,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjetphi2", cms.string("leadingFatJetPhi(1,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjettrimmedmass2", cms.string("leadingFatJetTrimmedMass(1,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjetfilteredmass2", cms.string("leadingFatJetFilteredMass(1,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjetprunedmass2", cms.string("leadingFatJetPrunedMass(1,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjetmassdrop2", cms.string("leadingFatJetMassDrop(1,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjettau2tau12", cms.string("leadingFatJetPrunedTau2Tau1(1,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjettau12", cms.string("leadingFatJetPrunedTau1(1,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjettau22", cms.string("leadingFatJetPrunedTau2(1,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjettau32", cms.string("leadingFatJetPrunedTau3(1,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjettau42", cms.string("leadingFatJetPrunedTau4(1,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),


            setattr(pt.variables, "fatjetpt3", cms.string("leadingFatJetPt(2,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjeteta3", cms.string("leadingFatJetEta(2,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjetphi3", cms.string("leadingFatJetPhi(2,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjettrimmedmass3", cms.string("leadingFatJetTrimmedMass(2,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjetfilteredmass3", cms.string("leadingFatJetFilteredMass(2,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjetprunedmass3", cms.string("leadingFatJetPrunedMass(2,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjetmassdrop3", cms.string("leadingFatJetMassDrop(2,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjettau2tau13", cms.string("leadingFatJetPrunedTau2Tau1(2,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjettau13", cms.string("leadingFatJetPrunedTau1(2,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjettau23", cms.string("leadingFatJetPrunedTau2(2,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjettau33", cms.string("leadingFatJetPrunedTau3(2,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),
            setattr(pt.variables, "fatjettau43", cms.string("leadingFatJetPrunedTau4(2,0,"+CJVmaxEta+",1,"+fatjetId_WP+")")),




###################
###################
###################

def addAdditionalJets(process,pt,jetId_WP = "0",pileupjetId_WP = "1", CJVminPt  = "30.", CJVmaxEta = "5.0", DphiJetVetominPt  = "15.", DphiJetVetominEta = "5.0",
                      DzBVeto    = "999999.9",minPtBVeto = "10.0"):

 if hasattr(pt,"variables"):

  setattr(process.stepBTree.variables,"v_jet5",cms.string("jet(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")")) 
  setattr(process.stepBTree.variables,"v_jet6",cms.string("jet(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"v_jet7",cms.string("jet(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"v_jet8",cms.string("jet(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))         

  setattr(process.stepBTree.variables,"jetpt5",cms.string("leadingJetPt(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jeteta5",cms.string("leadingJetEta(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetphi5",cms.string("leadingJetPhi(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetmass5",cms.string("leadingJetMass(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetid5",cms.string("leadingJetId(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetpileupidValue5",cms.string("leadingPileUpJetIdValue(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetpileupidFlag5",cms.string("leadingPileUpJetIdFlag(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetmva5",cms.string("leadingJetMva(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetbjpb5",cms.string("leadingJetBtag(4,'jetBProbabilityBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jettche5",cms.string("leadingJetBtag(4,'trackCountingHighEffBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jettchp5",cms.string("leadingJetBtag(4,'trackCountingHighPurBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jetcsv5",cms.string("leadingJetBtag(4,'combinedSecondaryVertexBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jetcsvMVA5",cms.string("leadingJetBtag(4,'combinedSecondaryVertexMVABJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  #setattr(process.stepBTree.variables,"jetptd5",cms.string("leadingJetPtd(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetCHM5",cms.string("leadingJetChargedHadronMultiplicity(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNHM5",cms.string("leadingJetNeutralHadronMultiplicity(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetPhM5",cms.string("leadingJetPhotonMultiplicity(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetQCptD5",cms.string("leadingJetPtD(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCaxis15",cms.string("leadingJetQGaxis1(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCaxis25",cms.string("leadingJetQGaxis2(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCRMScand5",cms.string("leadingJetQGRMScand(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCRmax5",cms.string("leadingJetQGRmax(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetptD5",cms.string("leadingJetPtD(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetaxis15",cms.string("leadingJetQGaxis1(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetaxis25",cms.string("leadingJetQGaxis2(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetRMScand5",cms.string("leadingJetQGRMScand(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetRmax5",cms.string("leadingJetQGRmax(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetNChgQC5",cms.string("leadingJetNChgQC(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNChgptCut5",cms.string("leadingJetNChgptCut(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNNeutralptCut5",cms.string("leadingJetNNeutralptCut(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))

  setattr(process.stepBTree.variables,"jetpt6",cms.string("leadingJetPt(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jeteta6",cms.string("leadingJetEta(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetphi6",cms.string("leadingJetPhi(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetmass6",cms.string("leadingJetMass(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetid6",cms.string("leadingJetId(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetpileupidValue6",cms.string("leadingPileUpJetIdValue(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetpileupidFlag6",cms.string("leadingPileUpJetIdFlag(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetmva6",cms.string("leadingJetMva(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetbjpb6",cms.string("leadingJetBtag(5,'jetBProbabilityBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jettche6",cms.string("leadingJetBtag(5,'trackCountingHighEffBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jettchp6",cms.string("leadingJetBtag(5,'trackCountingHighPurBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jetcsv6",cms.string("leadingJetBtag(5,'combinedSecondaryVertexBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jetcsvMVA6",cms.string("leadingJetBtag(5,'combinedSecondaryVertexMVABJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  #setattr(process.stepBTree.variables,"jetptd6",cms.string("leadingJetPtd(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetCHM6",cms.string("leadingJetChargedHadronMultiplicity(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNHM6",cms.string("leadingJetNeutralHadronMultiplicity(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetPhM6",cms.string("leadingJetPhotonMultiplicity(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetQCptD6",cms.string("leadingJetPtD(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCaxis16",cms.string("leadingJetQGaxis1(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCaxis26",cms.string("leadingJetQGaxis2(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCRMScand6",cms.string("leadingJetQGRMScand(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCRmax6",cms.string("leadingJetQGRmax(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetptD6",cms.string("leadingJetPtD(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetaxis16",cms.string("leadingJetQGaxis1(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetaxis26",cms.string("leadingJetQGaxis2(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetRMScand6",cms.string("leadingJetQGRMScand(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetRmax6",cms.string("leadingJetQGRmax(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetNChgQC6",cms.string("leadingJetNChgQC(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNChgptCut6",cms.string("leadingJetNChgptCut(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNNeutralptCut6",cms.string("leadingJetNNeutralptCut(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))

  setattr(process.stepBTree.variables,"jetpt7",cms.string("leadingJetPt(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jeteta7",cms.string("leadingJetEta(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetphi7",cms.string("leadingJetPhi(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetmass7",cms.string("leadingJetMass(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetid7",cms.string("leadingJetId(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetpileupidValue7",cms.string("leadingPileUpJetIdValue(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetpileupidFlag7",cms.string("leadingPileUpJetIdFlag(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetmva7",cms.string("leadingJetMva(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetbjpb7",cms.string("leadingJetBtag(6,'jetBProbabilityBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jettche7",cms.string("leadingJetBtag(6,'trackCountingHighEffBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jettchp7",cms.string("leadingJetBtag(6,'trackCountingHighPurBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jetcsv7",cms.string("leadingJetBtag(6,'combinedSecondaryVertexBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jetcsvMVA7",cms.string("leadingJetBtag(6,'combinedSecondaryVertexMVABJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  #setattr(process.stepBTree.variables,"jetptd7",cms.string("leadingJetPtd(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetCHM7",cms.string("leadingJetChargedHadronMultiplicity(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNHM7",cms.string("leadingJetNeutralHadronMultiplicity(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetPhM7",cms.string("leadingJetPhotonMultiplicity(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetQCptD7",cms.string("leadingJetPtD(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCaxis17",cms.string("leadingJetQGaxis1(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCaxis27",cms.string("leadingJetQGaxis2(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCRMScand7",cms.string("leadingJetQGRMScand(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCRmax7",cms.string("leadingJetQGRmax(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetptD7",cms.string("leadingJetPtD(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetaxis17",cms.string("leadingJetQGaxis1(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetaxis27",cms.string("leadingJetQGaxis2(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetRMScand7",cms.string("leadingJetQGRMScand(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetRmax7",cms.string("leadingJetQGRmax(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetNChgQC7",cms.string("leadingJetNChgQC(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNChgptCut7",cms.string("leadingJetNChgptCut(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNNeutralptCut7",cms.string("leadingJetNNeutralptCut(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))

  setattr(process.stepBTree.variables,"jetpt8",cms.string("leadingJetPt(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jeteta8",cms.string("leadingJetEta(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetphi8",cms.string("leadingJetPhi(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetmass8",cms.string("leadingJetMass(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetid8",cms.string("leadingJetId(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetpileupidValue8",cms.string("leadingPileUpJetIdValue(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetpileupidFlag8",cms.string("leadingPileUpJetIdFlag(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetmva8",cms.string("leadingJetMva(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetbjpb8",cms.string("leadingJetBtag(7,'jetBProbabilityBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jettche8",cms.string("leadingJetBtag(7,'trackCountingHighEffBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jettchp8",cms.string("leadingJetBtag(7,'trackCountingHighPurBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jetcsv8",cms.string("leadingJetBtag(7,'combinedSecondaryVertexBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jetcsvMVA8",cms.string("leadingJetBtag(7,'combinedSecondaryVertexMVABJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  #setattr(process.stepBTree.variables,"jetptd8",cms.string("leadingJetPtd(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetCHM8",cms.string("leadingJetChargedHadronMultiplicity(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNHM8",cms.string("leadingJetNeutralHadronMultiplicity(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetPhM8",cms.string("leadingJetPhotonMultiplicity(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetQCptD8",cms.string("leadingJetPtD(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCaxis18",cms.string("leadingJetQGaxis1(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCaxis28",cms.string("leadingJetQGaxis2(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCRMScand8",cms.string("leadingJetQGRMScand(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCRmax8",cms.string("leadingJetQGRmax(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetptD8",cms.string("leadingJetPtD(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetaxis18",cms.string("leadingJetQGaxis1(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetaxis28",cms.string("leadingJetQGaxis2(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetRMScand8",cms.string("leadingJetQGRMScand(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetRmax8",cms.string("leadingJetQGRmax(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetNChgQC8",cms.string("leadingJetNChgQC(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNChgptCut8",cms.string("leadingJetNChgptCut(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNNeutralptCut8",cms.string("leadingJetNNeutralptCut(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))


  setattr(process.stepBTree.variables,"v_jet5_puppi",cms.string("puppiJet(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")")) 
  setattr(process.stepBTree.variables,"v_jet6_puppi",cms.string("puppiJet(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"v_jet7_puppi",cms.string("puppiJet(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"v_jet8_puppi",cms.string("puppiJet(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))         

  setattr(process.stepBTree.variables,"jetpt_puppi5",cms.string("leadingPuppiJetPt(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jeteta_puppi5",cms.string("leadingPuppiJetEta(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetphi_puppi5",cms.string("leadingPuppiJetPhi(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetmass_puppi5",cms.string("leadingPuppiJetMass(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetid_puppi5",cms.string("leadingPuppiJetId(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetpileupidValue_puppi5",cms.string("leadingPuppiPileUpJetIdValue(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetpileupidFlag_puppi5",cms.string("leadingPuppiPileUpJetIdFlag(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetmva_puppi5",cms.string("leadingPuppiJetMva(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetbjpb_puppi5",cms.string("leadingPuppiJetBtag(0,'jetBProbabilityBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jettche_puppi5",cms.string("leadingPuppiJetBtag(0,'trackCountingHighEffbPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jettchp_puppi5",cms.string("leadingPuppiJetBtag(0,'AK5trackCountingHighPurBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jetcsv_puppi5",cms.string("leadingPuppiJetBtag(0,'AK5combinedSecondaryVertexBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jetcsvMVA_puppi5",cms.string("leadingPuppiJetBtag(0,'AK5combinedSecondaryVertexMVABPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))

  #setattr(process.stepBTree.variables,"jetptd5_puppi",cms.string("leadingPuppiJetPtd(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetCHM5_puppi",cms.string("leadingPuppiJetChargedHadronMultiplicity(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNHM5_puppi",cms.string("leadingPuppiJetNeutralHadronMultiplicity(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetPhM5_puppi",cms.string("leadingPuppiJetPhotonMultiplicity(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetQCptD5_puppi",cms.string("leadingPuppiJetPtD(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCaxis15_puppi",cms.string("leadingPuppiJetQGaxis1(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCaxis25_puppi",cms.string("leadingPuppiJetQGaxis2(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCRMScand5_puppi",cms.string("leadingPuppiJetQGRMScand(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCRmax5_puppi",cms.string("leadingPuppiJetQGRmax(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetptD5_puppi",cms.string("leadingPuppiJetPtD(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetaxis15_puppi",cms.string("leadingPuppiJetQGaxis1(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetaxis25_puppi",cms.string("leadingPuppiJetQGaxis2(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetRMScand5_puppi",cms.string("leadingPuppiJetQGRMScand(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetRmax5_puppi",cms.string("leadingPuppiJetQGRmax(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetNChgQC5_puppi",cms.string("leadingPuppiJetNChgQC(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNChgptCut5_puppi",cms.string("leadingPuppiJetNChgptCut(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNNeutralptCut5_puppi",cms.string("leadingPuppiJetNNeutralptCut(4,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))


  setattr(process.stepBTree.variables,"jetpt_puppi6",cms.string("leadingPuppiJetPt(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jeteta_puppi6",cms.string("leadingPuppiJetEta(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetphi_puppi6",cms.string("leadingPuppiJetPhi(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetmass_puppi6",cms.string("leadingPuppiJetMass(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetid_puppi6",cms.string("leadingPuppiJetId(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetpileupidValue_puppi6",cms.string("leadingPuppiPileUpJetIdValue(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetpileupidFlag_puppi6",cms.string("leadingPuppiPileUpJetIdFlag(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetmva_puppi6",cms.string("leadingPuppiJetMva(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetbjpb_puppi6",cms.string("leadingPuppiJetBtag(0,'jetBProbabilityBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jettche_puppi6",cms.string("leadingPuppiJetBtag(0,'trackCountingHighEffbPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jettchp_puppi6",cms.string("leadingPuppiJetBtag(0,'AK5trackCountingHighPurBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jetcsv_puppi6",cms.string("leadingPuppiJetBtag(0,'AK5combinedSecondaryVertexBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jetcsvMVA_puppi6",cms.string("leadingPuppiJetBtag(0,'AK5combinedSecondaryVertexMVABPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))

  #setattr(process.stepBTree.variables,"jetptd6_puppi",cms.string("leadingPuppiJetPtd(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetCHM6_puppi",cms.string("leadingPuppiJetChargedHadronMultiplicity(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNHM6_puppi",cms.string("leadingPuppiJetNeutralHadronMultiplicity(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetPhM6_puppi",cms.string("leadingPuppiJetPhotonMultiplicity(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetQCptD6_puppi",cms.string("leadingPuppiJetPtD(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCaxis16_puppi",cms.string("leadingPuppiJetQGaxis1(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCaxis26_puppi",cms.string("leadingPuppiJetQGaxis2(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCRMScand6_puppi",cms.string("leadingPuppiJetQGRMScand(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCRmax6_puppi",cms.string("leadingPuppiJetQGRmax(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetptD6_puppi",cms.string("leadingPuppiJetPtD(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetaxis16_puppi",cms.string("leadingPuppiJetQGaxis1(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetaxis26_puppi",cms.string("leadingPuppiJetQGaxis2(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetRMScand6_puppi",cms.string("leadingPuppiJetQGRMScand(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetRmax6_puppi",cms.string("leadingPuppiJetQGRmax(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetNChgQC6_puppi",cms.string("leadingPuppiJetNChgQC(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNChgptCut6_puppi",cms.string("leadingPuppiJetNChgptCut(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNNeutralptCut6_puppi",cms.string("leadingPuppiJetNNeutralptCut(5,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))


  setattr(process.stepBTree.variables,"jetpt_puppi7",cms.string("leadingPuppiJetPt(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jeteta_puppi7",cms.string("leadingPuppiJetEta(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetphi_puppi7",cms.string("leadingPuppiJetPhi(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetmass_puppi7",cms.string("leadingPuppiJetMass(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetid_puppi7",cms.string("leadingPuppiJetId(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetpileupidValue_puppi7",cms.string("leadingPuppiPileUpJetIdValue(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetpileupidFlag_puppi7",cms.string("leadingPuppiPileUpJetIdFlag(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetmva_puppi7",cms.string("leadingPuppiJetMva(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetbjpb_puppi7",cms.string("leadingPuppiJetBtag(0,'jetBProbabilityBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jettche_puppi7",cms.string("leadingPuppiJetBtag(0,'trackCountingHighEffbPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jettchp_puppi7",cms.string("leadingPuppiJetBtag(0,'AK5trackCountingHighPurBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jetcsv_puppi7",cms.string("leadingPuppiJetBtag(0,'AK5combinedSecondaryVertexBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jetcsvMVA_puppi7",cms.string("leadingPuppiJetBtag(0,'AK5combinedSecondaryVertexMVABPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))

  #setattr(process.stepBTree.variables,"jetptd7_puppi",cms.string("leadingPuppiJetPtd(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetCHM7_puppi",cms.string("leadingPuppiJetChargedHadronMultiplicity(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNHM7_puppi",cms.string("leadingPuppiJetNeutralHadronMultiplicity(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetPhM7_puppi",cms.string("leadingPuppiJetPhotonMultiplicity(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetQCptD7_puppi",cms.string("leadingPuppiJetPtD(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCaxis17_puppi",cms.string("leadingPuppiJetQGaxis1(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCaxis27_puppi",cms.string("leadingPuppiJetQGaxis2(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCRMScand7_puppi",cms.string("leadingPuppiJetQGRMScand(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCRmax7_puppi",cms.string("leadingPuppiJetQGRmax(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetptD7_puppi",cms.string("leadingPuppiJetPtD(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetaxis17_puppi",cms.string("leadingPuppiJetQGaxis1(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetaxis27_puppi",cms.string("leadingPuppiJetQGaxis2(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetRMScand7_puppi",cms.string("leadingPuppiJetQGRMScand(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetRmax7_puppi",cms.string("leadingPuppiJetQGRmax(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetNChgQC7_puppi",cms.string("leadingPuppiJetNChgQC(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNChgptCut7_puppi",cms.string("leadingPuppiJetNChgptCut(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNNeutralptCut7_puppi",cms.string("leadingPuppiJetNNeutralptCut(6,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))

  setattr(process.stepBTree.variables,"jetpt_puppi8",cms.string("leadingPuppiJetPt(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jeteta_puppi8",cms.string("leadingPuppiJetEta(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetphi_puppi8",cms.string("leadingPuppiJetPhi(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetmass_puppi8",cms.string("leadingPuppiJetMass(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetid_puppi8",cms.string("leadingPuppiJetId(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetpileupidValue_puppi8",cms.string("leadingPuppiPileUpJetIdValue(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetpileupidFlag_puppi8",cms.string("leadingPuppiPileUpJetIdFlag(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetmva_puppi8",cms.string("leadingPuppiJetMva(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  setattr(process.stepBTree.variables,"jetbjpb_puppi8",cms.string("leadingPuppiJetBtag(0,'jetBProbabilityBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jettche_puppi8",cms.string("leadingPuppiJetBtag(0,'trackCountingHighEffbPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jettchp_puppi8",cms.string("leadingPuppiJetBtag(0,'AK5trackCountingHighPurBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jetcsv_puppi8",cms.string("leadingPuppiJetBtag(0,'AK5combinedSecondaryVertexBPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))
  setattr(process.stepBTree.variables,"jetcsvMVA_puppi8",cms.string("leadingPuppiJetBtag(0,'AK5combinedSecondaryVertexMVABPuppiJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+","+DzBVeto+")"))

  #setattr(process.stepBTree.variables,"jetptd8_puppi",cms.string("leadingPuppiJetPtd(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetCHM8_puppi",cms.string("leadingPuppiJetChargedHadronMultiplicity(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNHM8_puppi",cms.string("leadingPuppiJetNeutralHadronMultiplicity(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetPhM8_puppi",cms.string("leadingPuppiJetPhotonMultiplicity(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetQCptD8_puppi",cms.string("leadingPuppiJetPtD(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCaxis18_puppi",cms.string("leadingPuppiJetQGaxis1(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCaxis28_puppi",cms.string("leadingPuppiJetQGaxis2(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCRMScand8_puppi",cms.string("leadingPuppiJetQGRMScand(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetQCRmax8_puppi",cms.string("leadingPuppiJetQGRmax(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",1)"))
  #setattr(process.stepBTree.variables,"jetptD8_puppi",cms.string("leadingPuppiJetPtD(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetaxis18_puppi",cms.string("leadingPuppiJetQGaxis1(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetaxis28_puppi",cms.string("leadingPuppiJetQGaxis2(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetRMScand8_puppi",cms.string("leadingPuppiJetQGRMScand(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetRmax8_puppi",cms.string("leadingPuppiJetQGRmax(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+",0)"))
  #setattr(process.stepBTree.variables,"jetNChgQC8_puppi",cms.string("leadingPuppiJetNChgQC(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNChgptCut8_puppi",cms.string("leadingPuppiJetNChgptCut(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))
  #setattr(process.stepBTree.variables,"jetNNeutralptCut8_puppi",cms.string("leadingPuppiJetNNeutralptCut(7,0,"+CJVmaxEta+",1,"+jetId_WP+","+pileupjetId_WP+")"))

###################
###################
###################

def addIsoStudyVariables(process,pt):
    if hasattr(pt,"variables"):
      for i,l in enumerate(["lep1", "lep2"]):
        setattr(pt.variables, l+"isoMergePf" , cms.string("? abs(candByPt({0}).pdgId) == 13 ? candByPt({0}).userFloat('muSmurfPF') : candByPt({0}).userFloat('eleSmurfPF04')".format(i)))
        setattr(pt.variables, l+"isoRecoTracks" , cms.string("? abs(candByPt({0}).pdgId) == 13 ? candByPt({0}).isolationR03().sumPt : candByPt({0}).dr03TkSumPt".format(i)))
        setattr(pt.variables, l+"isoRecoEcal" , cms.string("? abs(candByPt({0}).pdgId) == 13 ? candByPt({0}).isolationR03().emEt : ".format(i) +
                                                              " ( max(0,candByPt({0}).dr03EcalRecHitSumEt - 1)*candByPt({0}).isEB + (1-candByPt({0}).isEB)*candByPt({0}).dr03EcalRecHitSumEt )".format(i)))
        setattr(pt.variables, l+"isoRecoHCal" , cms.string("? abs(candByPt({0}).pdgId) == 13 ? candByPt({0}).isolationR03().hadEt : candByPt({0}).dr03HcalTowerSumEt ".format(i)))
        setattr(pt.variables, l+"isoRecoHCalFull", cms.string("? abs(candByPt({0}).pdgId) == 13 ? candByPt({0}).isolationR03().hadEt : candByPt({0}).userFloat('hcalFull')".format(i)))
        setattr(pt.variables, l+"isoPfCharged" , cms.string("candByPt({0}).userFloat('pfCharged')".format(i)))
        setattr(pt.variables, l+"isoPfNeutral" , cms.string("candByPt({0}).userFloat('pfNeutral')".format(i)))
        setattr(pt.variables, l+"isoPfPhoton" , cms.string("candByPt({0}).userFloat('pfPhoton')".format(i)))
        setattr(pt.variables, l+"isoSmurfCharged", cms.string("candByPt({0}).userFloat('smurfCharged')".format(i)))
        setattr(pt.variables, l+"isoSmurfNeutral", cms.string("candByPt({0}).userFloat('smurfNeutral')".format(i)))
        setattr(pt.variables, l+"isoSmurfPhoton" , cms.string("candByPt({0}).userFloat('smurfPhoton')".format(i)))
        setattr(pt.variables, l+"isoSmurfNoOverCharged", cms.string("candByPt({0}).userFloat('smurfNoOverCharged')".format(i)))
        setattr(pt.variables, l+"isoSmurfNoOverNeutral", cms.string("candByPt({0}).userFloat('smurfNoOverNeural')".format(i)))
        setattr(pt.variables, l+"isoSmurfNoOverPhoton" , cms.string("candByPt({0}).userFloat('smurfNoOverPhoton')".format(i)))
        for algo in ("JetCone", "FixCone03", "FixCone04", "MaxCone03", "MaxCone04", "SumCone02", "SumCone04"):
            for name in ("Charged", "ChargedNoOvRem"): #, "NeutralHadAll", "NeutralHadPt05", "NeutralHadPt1", "Photons", "PhotonsMuStrip"):
                setattr(pt.variables, "%sjetiso%s%s"%(l,algo,name), cms.string("candByPt(%d).userFloat('jetIso%s%s')"%(i,algo,name)))
    else:
        raise RuntimeError, "In addIsoStudyVariables, %s doesn't look like a ProbeTreeProducer object, it has no 'variables' attribute." % pt
    if not hasattr(process,"isoStudySequence"):
        process.load("WWAnalysis.AnalysisStep.isoStudySequence_cff")

###################
###################
###################
###################


def addExtraPUWeights(process,tree,X,seq):
    print "WARNING, all the distro's haven't been designed yet, don't turn addExtraPUWeights on yet"
    from WWAnalysis.AnalysisStep.pileupReweighting_cfi import lpOld, lpNew, currentOld, currentNew, mcNominal, mcTemplate
    dataDistros = {
        "LPOld": lpOld,
        "LPNew": lpNew,
        "CurrentOld": currentOld,
        "CurrentNew": currentNew,
    }
    mcDistros = {
        "MCNominal": mcNominal,
        "MCTemplate": mcTemplate,
    }
    for data in dataDistros:
        for mc in mcDistros:
            newName = X+mc+data
            setattr(tree.variables, 'pu'+mc+data, cms.InputTag(newName))
            setattr(tree.variables, 'puOOT'+mc+data, cms.InputTag(newName+"OOT"))
            setattr(process, newName, process.puWeight.clone(src = cms.InputTag("ww%s"% (X)), dataDist = dataDistros[data][:], s4Dist = mcDistros[mc][:]))
            setattr(process, newName+"OOT", process.puWeight.clone(src = cms.InputTag("ww%s"% (X)), dataDist = dataDistros[data][:], s4Dist = mcDistros[mc][:], useOOT=True))
            seq += getattr(process, newName)
            seq += getattr(process, newName+"OOT")

