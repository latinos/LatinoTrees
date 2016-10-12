import FWCore.ParameterSet.Config as cms
import os
#from WWAnalysis.AnalysisStep.wwMuons_cfi import Scenario6_ICHEP2012,Scenario2_KINK_MUONS,Scenario1_LP_MUONS
#from WWAnalysis.AnalysisStep.wwElectrons_cfi import Scenario6_ICHEP,Scenario4_BDT_ELECTRONS,Scenario3_LH_ELECTRONS,Scenario2_LP_ELECTRONS,Scenario1_LP_ELECTRONS

# NB: typedef ObjectMUltiplicitycounterl<reco::Vertex>    VertexMultiplicityCounter;
nverticesModule = cms.EDProducer("VertexMultiplicityCounter",
    probes = cms.InputTag("REPLACE_ME"),
    objects = cms.InputTag("offlineSlimmedPrimaryVertices"), # miniAOD goodPrimaryVertices
    objectSelection = cms.string("!isFake && ndof > 4 && abs(z) <= 25 && position.Rho <= 2"),
)



# option for PhilJetid
# 4 possible workingpoints
# jetId_WP
# 0 NONE no requirement
# 1 OLD Simple Jet ID
# 2 DZ ID (Not implemented ! )
# 3 HGG CutBased (Not implemented ! )
# 4 MVA LOOSE
# 5 MVA MEDIUM
# 6 MVA TIGHT
# 7 Run II jetID LOOSE
# 8 Run II jetID TIGHT

#jetId_WP="4" ----> FIXME: to be used!
#jetId_WP="0"
#jetId_WP = "7"
# -> DON'T define this here, since it is defined in "stepB.py"

import LatinoTrees.AnalysisStep.globalVariables as globalVariables
jetId_WP = globalVariables.jetId_WP


# JetCuts

CJVminPt = "30."
CJVmaxEta = "4.7"
#DphiJetVetominPt = "15."
#DphiJetVetominEta = "4.7"

DzBVeto="999999.9"
minPtBVeto="10.0"


stepBTree = cms.EDFilter("GenericTreeProducer",
    src = cms.InputTag(""),
    maxVectorsLength = cms.int32(10),
#stepBTree = cms.EDFilter("ProbeTreeProducer",
# cut = cms.string("q(0)*q(1) > 0 && !isSTA(0) && !isSTA(1) && "+
    #cut = cms.string("q(0)*q(1) < 0 && !isSTA(0) && !isSTA(1) && "+
    cut = cms.string(
                     "1"  
                     #"!isSTA(0) && !isSTA(1) && "+
                     #"leptEtaCut(2.4,2.5) && ptMax > 17 && ptMin > 8"
                     
# previously ...
# "leptEtaCut(2.4,2.5) && ptMax > 20 && ptMin > 10"
# " && triggerMatchingCut('DATASET')"
# "nExtraLep(10) == 0 "
# +" && passesIP"
# +(" && triggerMatchingCut('DATASET')")
    ),
    variables = cms.PSet(
        #hypo = cms.string("hypo()"),
        #lepton1_pt = cms.string("lepton(0).x()"),
        
        # leptons
        channel = cms.string("channel()"),

        # keep these comments here only as a placeholder 
        # for different methods that can be used in filling the trees
        # (do NOT remove)
        
        #v_lepton1 = cms.string("lepton(0)"),  # Deprecated, use std_vector_lepton
        #v_lepton2 = cms.string("lepton(1)"),  # Deprecated, use std_vector_lepton
        #v_lepton3 = cms.string("lepton(2)"),  # Deprecated, use std_vector_lepton
        #v_lepton4 = cms.string("lepton(3)"),  # Deprecated, use std_vector_lepton

        #v_jet1 = cms.string("jet(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),  # Deprecated, use std_vector_jet
        #v_jet2 = cms.string("jet(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),  # Deprecated, use std_vector_jet
        #v_jet3 = cms.string("jet(2,0,"+CJVmaxEta+",1,"+jetId_WP+")"),  # Deprecated, use std_vector_jet
        #v_jet4 = cms.string("jet(3,0,"+CJVmaxEta+",1,"+jetId_WP+")"),  # Deprecated, use std_vector_jet

        #std_variable_vector_lepton_pt_max3 = cms.string("ptByPt/3"),
        #std_vector_lepton_pt_max3          = cms.string("ptByPt/3"),

        std_vector_lepton_pt            = cms.string("ptByPt"),
        std_vector_lepton_eta           = cms.string("etaByPt"),
        std_vector_lepton_phi           = cms.string("phiByPt"),
        std_vector_lepton_flavour       = cms.string("flavourByPt"),
        #std_vector_lepton_id = cms.string("pdgIdByPt"),  # Deprecated, use std_vector_lepton_flavour
        std_vector_lepton_isMediumMuon  = cms.string("isMediumMuonByPt"),
        #std_vector_lepton_isTrackerMuon = cms.string("isTrackerMuonByPt"),
        std_vector_lepton_isTightMuon   = cms.string("isTightMuonByPt"),
        std_vector_lepton_SIP3D         = cms.string("SIP3DByPt"),
        std_vector_lepton_ch            = cms.string("qByPt"),
        std_vector_lepton_d0            = cms.string("d0ByPt"),
        std_vector_lepton_dz            = cms.string("dzByPt"),
        std_vector_lepton_trackIso      = cms.string("trackIsoByPt"),
        std_vector_lepton_chargedHadronIso   =     cms.string("chargedHadronIsoByPt"),
        std_vector_lepton_chargedParticleIso =     cms.string("chargedParticleIsoByPt"),
        std_vector_lepton_neutralHadronIso   =     cms.string("neutralHadronIsoByPt"),
        std_vector_lepton_photonIso          =     cms.string("photonIsoByPt"),
        std_vector_lepton_sumPUPt            =     cms.string("sumPUPtByPt"), 
        std_vector_lepton_chargedHadronMiniIso =   cms.string("chargedHadronMiniIsoByPt"),
        std_vector_lepton_chargedPileUpMiniIso =   cms.string("chargedPileUpMiniIsoByPt"),
        std_vector_lepton_neutralHadronMiniIso =   cms.string("neutralHadronMiniIsoByPt"),
        std_vector_lepton_photonMiniIso        =   cms.string("photonMiniIsoByPt"),


        std_vector_lepton_PfIsoDeltaBeta= cms.string("allIsoByPt"),

        std_vector_puppijet_pt  = cms.string("leadingSecondJetPt"),
        std_vector_puppijet_eta = cms.string("leadingSecondJetEta"),
        std_vector_puppijet_phi = cms.string("leadingSecondJetPhi"),

        std_vector_jet_pt   = cms.string("leadingJetPt"),
        std_vector_jet_eta  = cms.string("leadingJetEta"),
        std_vector_jet_phi  = cms.string("leadingJetPhi"),
        std_vector_jet_mass = cms.string("leadingJetMass"),

        std_vector_jet_pt_raw = cms.string("leadingJetPtRaw"),
        std_vector_jet_pt_L1  = cms.string("leadingJetPtL1"),

        peaking = cms.string("peaking"),
        trigger = cms.string("guillelmoTrigger('DATASET')"),
        triggerFakeRate = cms.string("fakeRateTrigger('DATASET')"),
        #std_vector_trigger = cms.string("selectedRateTrigger/4"), --> defined in steB.py only for data
        
        nextra = cms.string("nExtraLep(10)"),

        # met
        metPfRaw = cms.string("metPfRaw"),
        metPfRawJetEnUp = cms.string("metPfRawJetEnUp"),
        metPfRawJetEnDn = cms.string("metPfRawJetEnDn"),
        metPfRawJetResUp = cms.string("metPfRawJetResUp"),
        metPfRawJetResDn = cms.string("metPfRawJetResDn"),
        metPfRawMuonEnUp = cms.string("metPfRawMuonEnUp"),
        metPfRawMuonEnDn = cms.string("metPfRawMuonEnDn"),
        metPfRawElecEnUp = cms.string("metPfRawElecEnUp"),
        metPfRawElecEnDn = cms.string("metPfRawElecEnDn"),
        metPfRawUnclEnUp = cms.string("metPfRawUnclEnUp"),
        metPfRawUnclEnDn = cms.string("metPfRawUnclEnDn"),

        metPfRawPhi = cms.string("metPfRawPhi"),
        metPfRawPhiJetEnUp = cms.string("metPfRawPhiJetEnUp"),
        metPfRawPhiJetEnDn = cms.string("metPfRawPhiJetEnDn"),
        metPfRawPhiJetResUp = cms.string("metPfRawPhiJetResUp"),
        metPfRawPhiJetResDn = cms.string("metPfRawPhiJetResDn"),
        metPfRawPhiMuonEnUp = cms.string("metPfRawPhiMuonEnUp"),
        metPfRawPhiMuonEnDn = cms.string("metPfRawPhiMuonEnDn"),
        metPfRawPhiElecEnUp = cms.string("metPfRawPhiElecEnUp"),
        metPfRawPhiElecEnDn = cms.string("metPfRawPhiElecEnDn"),
        metPfRawPhiUnclEnUp = cms.string("metPfRawPhiUnclEnUp"),
        metPfRawPhiUnclEnDn = cms.string("metPfRawPhiUnclEnDn"),

        metPfRawSumEt = cms.string("metPfRawSumEt"),

        metPfType1 = cms.string("metPfType1"),
        metPfType1JetEnUp = cms.string("metPfType1JetEnUp"),
        metPfType1JetEnDn = cms.string("metPfType1JetEnDn"),
        metPfType1JetResUp = cms.string("metPfType1JetResUp"),
        metPfType1JetResDn = cms.string("metPfType1JetResDn"),
        metPfType1MuonEnUp = cms.string("metPfType1MuonEnUp"),
        metPfType1MuonEnDn = cms.string("metPfType1MuonEnDn"),
        metPfType1ElecEnUp = cms.string("metPfType1ElecEnUp"),
        metPfType1ElecEnDn = cms.string("metPfType1ElecEnDn"),
        metPfType1UnclEnUp = cms.string("metPfType1UnclEnUp"),
        metPfType1UnclEnDn = cms.string("metPfType1UnclEnDn"),

        metPfType1Phi = cms.string("metPfType1Phi"),
        metPfType1SumEt = cms.string("metPfType1SumEt"),

        metPfNoHf = cms.string("metPfNoHf"),

        metPfProj = cms.string("metPfProj"),
        metPfProjJetEnUp = cms.string("metPfProjJetEnUp"),
        metPfProjJetEnDn = cms.string("metPfProjJetEnDn"),
        metPfProjJetResUp = cms.string("metPfProjJetResUp"),
        metPfProjJetResDn = cms.string("metPfProjJetResDn"),
        metPfProjMuonEnUp = cms.string("metPfProjMuonEnUp"),
        metPfProjMuonEnDn = cms.string("metPfProjMuonEnDn"),
        metPfProjElecEnUp = cms.string("metPfProjElecEnUp"),
        metPfProjElecEnDn = cms.string("metPfProjElecEnDn"),
        metPfProjUnclEnUp = cms.string("metPfProjUnclEnUp"),
        metPfProjUnclEnDn = cms.string("metPfProjUnclEnDn"),

        metPuppi = cms.string("pupMet"),
        metTtrk = cms.string("trkMet"),
        metTtrkPhi = cms.string("trkMetphi"),
        # mvamet = cms.string("mvaMet"),
        # mvamethi = cms.string("mvaMetPhi"),
        # pmvamet = cms.string("projMvaMet"),
        chSumEt = cms.string("chargedMetSmurfSumEt"),
        pfmetMEtSig = cms.string("pfMetMEtSig"),
        pfmetSignificance = cms.string("pfMetSignificance"),
        chmet = cms.string("chargedMetSmurf"),
        chmetphi = cms.string("chargedMetSmurfPhi"),
        pchmet = cms.string("projChargedMetSmurf"),
        redmet = cms.string("-9999"),
        predmet = cms.string("-9999"),
        mpmet = cms.string("min(metPfProj,projChargedMetSmurf)"), ##note: min of proj and proj of min are not the same
        imet = cms.string("min(metPfProj,projChargedMetSmurf)*cos(metPfRawPhi-chargedMetSmurfPhi)"),
	hEtaPlus_counts = cms.string("hEtaPlus_counts"),
	hEtaMinus_counts = cms.string("hEtaMinus_counts"),
	h0Barrel_counts = cms.string("h0Barrel_counts"),
	h0EndcapPlus_counts = cms.string("h0EndcapPlus_counts"),
	h0EndcapMinus_counts = cms.string("h0EndcapMinus_counts"),
	gammaBarrel_counts = cms.string("gammaBarrel_counts"),
	gammaEndcapPlus_counts = cms.string("gammaEndcapPlus_counts"),
	gammaEndcapMinus_counts = cms.string("gammaEndcapMinus_counts"),
	hHFPlus_counts = cms.string("hHFPlus_counts"),
	hHFMinus_counts = cms.string("hHFMinus_counts"),
	egammaHFPlus_counts = cms.string("egammaHFPlus_counts"),
	egammaHFMinus_counts = cms.string("egammaHFMinus_counts"),

	hEtaPlus_sumPt = cms.string("hEtaPlus_sumPt"),
	hEtaMinus_sumPt = cms.string("hEtaMinus_sumPt"),
	h0Barrel_sumPt = cms.string("h0Barrel_sumPt"),
	h0EndcapPlus_sumPt = cms.string("h0EndcapPlus_sumPt"),
	h0EndcapMinus_sumPt = cms.string("h0EndcapMinus_sumPt"),
	gammaBarrel_sumPt = cms.string("gammaBarrel_sumPt"),
	gammaEndcapPlus_sumPt = cms.string("gammaEndcapPlus_sumPt"),
	gammaEndcapMinus_sumPt = cms.string("gammaEndcapMinus_sumPt"),
	hHFPlus_sumPt = cms.string("hHFPlus_sumPt"),
	hHFMinus_sumPt = cms.string("hHFMinus_sumPt"),
	egammaHFPlus_sumPt = cms.string("egammaHFPlus_sumPt"),
	egammaHFMinus_sumPt = cms.string("egammaHFMinus_sumPt"),




        #gammaMRStar = cms.string("gammaMRStar"),
        njet = cms.string("nCentralJets("+CJVminPt+","+CJVmaxEta+",1,"+jetId_WP+")"),
        njetid = cms.string("nCentralJets("+CJVminPt+","+CJVmaxEta+",1,0)"),
        # here we do apply a dz cut cause we are actually counting bjets
        nbjettche = cms.string("bTaggedJetsOver("+CJVminPt+",2.1,'pfTrackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+")"),
        nbjet     = cms.string("bTaggedJetsOver("+CJVminPt+",1.05,'pfJetBProbabilityBJetTags',"+jetId_WP+","+DzBVeto+")"),

        jetRho = cms.string("getJetRhoIso()"),
        jetRhoCalo = cms.string("getJetRhoCaloIso()"),

        #eleIdVeto1   = cms.string("leptIdByPt(\"egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-veto\",\"\",0)"),
        #eleIdLoose1  = cms.string("leptIdByPt(\"egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-loose\",\"\",0)"),
        #eleIdMedium1 = cms.string("leptIdByPt(\"egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-medium\",\"\",0)"),
        #eleIdTight1  = cms.string("leptIdByPt(\"egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-tight\",\"\",0)"),

        #                                                                              FREQUENCY will be replaced to 25ns/50ns in stepB.py
        std_vector_lepton_eleIdVeto   = cms.string("leptIdByPt(\"egmGsfElectronIDs:cutBasedElectronID-Spring15-FREQUENCY-V1-standalone-veto\",\"\""),
        std_vector_lepton_eleIdLoose  = cms.string("leptIdByPt(\"egmGsfElectronIDs:cutBasedElectronID-Spring15-FREQUENCY-V1-standalone-loose\",\"\""),
        std_vector_lepton_eleIdMedium = cms.string("leptIdByPt(\"egmGsfElectronIDs:cutBasedElectronID-Spring15-FREQUENCY-V1-standalone-medium\",\"\""),
        std_vector_lepton_eleIdTight  = cms.string("leptIdByPt(\"egmGsfElectronIDs:cutBasedElectronID-Spring15-FREQUENCY-V1-standalone-tight\",\"\""),

        # in the 2012 selection, 2 BJet algorithms are used: softtche and hardbjpb !
        # softbdisc = cms.string("highestBDiscRange(10.0,30.0,'pfTrackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+")"),
        # hardbdisc = cms.string("highestBDiscRange(30.0,999999.,'pfJetBProbabilityBJetTags',"+jetId_WP+","+DzBVeto+",1)"),
        softtche = cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'pfTrackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+")"),
        hardtche = cms.string("highestBDiscRange("+CJVminPt+",999999.,'pfTrackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+",1)"),
        softbjpb = cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'pfJetBProbabilityBJetTags',"+jetId_WP+","+DzBVeto+")"),
        hardbjpb = cms.string("highestBDiscRange("+CJVminPt+",999999.,'pfJetBProbabilityBJetTags',"+jetId_WP+","+DzBVeto+",1)"),
        tightmu = cms.string("passesSmurfMuonID"),
        #worstJetLepPt = cms.string("max(matchedJetPt(0, 0.5)/pt(0), matchedJetPt(1, 0.5)/pt(1))"),
        dataset = cms.string("REPLACE_ME"),
        puW = cms.string("1"),
        kfW = cms.string("1"),
        baseW = cms.string("REPLACE_ME"),

        trpu = cms.InputTag("nPU:tr"),
        itpu = cms.InputTag("nPU:it"),
        ootpup1 = cms.InputTag("nPU:p1"),
        ootpum1 = cms.InputTag("nPU:m1"),

        effW = cms.string("1"),
        triggW = cms.string("1"),
        fakeW = cms.string("1"),

        # mc info
        mctruth = cms.string("-1"),

	# Vertex Information
	nGoodVtx     = cms.string("nGoodVertices"),


        # ZGstar Gen-Level
        Gen_ZGstar_mu1_pt  = cms.string("Gen_ZGstar_mu1_pt"),
        Gen_ZGstar_mu1_eta = cms.string("Gen_ZGstar_mu1_eta"),
        Gen_ZGstar_mu1_phi = cms.string("Gen_ZGstar_mu1_phi"),

        Gen_ZGstar_mu2_pt  = cms.string("Gen_ZGstar_mu2_pt"),
        Gen_ZGstar_mu2_eta = cms.string("Gen_ZGstar_mu2_eta"),
        Gen_ZGstar_mu2_phi = cms.string("Gen_ZGstar_mu2_phi"),

        Gen_ZGstar_ele1_pt  = cms.string("Gen_ZGstar_ele1_pt"),
        Gen_ZGstar_ele1_eta = cms.string("Gen_ZGstar_ele1_eta"),
        Gen_ZGstar_ele1_phi = cms.string("Gen_ZGstar_ele1_phi"),

        Gen_ZGstar_ele2_pt  = cms.string("Gen_ZGstar_ele2_pt"),
        Gen_ZGstar_ele2_eta = cms.string("Gen_ZGstar_ele2_eta"),
        Gen_ZGstar_ele2_phi = cms.string("Gen_ZGstar_ele2_phi"),

        Gen_ZGstar_mass = cms.string("Gen_ZGstar_mass"),
        Gen_ZGstar_deltaR = cms.string("Gen_ZGstar_deltaR"),

        Gen_ZGstar_MomId	= cms.string("Gen_ZGstar_MomId"),
        Gen_ZGstar_MomStatus	= cms.string("Gen_ZGstar_MomStatus"),
        Gen_ZGstar_MomInitStatus= cms.string("Gen_ZGstar_MomInitStatus"),

    ),
    flags = cms.PSet(
        # here we do apply a dz cut cause we are actually counting bjets
        bveto_ip       = cms.string("bTaggedJetsBetween(10,30,2.1,'pfTrackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+") == 0"),
        # bveto          = cms.string("bTaggedJetsBetween(10,30,2.1,'pfTrackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+") == 0 && nSoftMu(3) == 0"),
        # bveto_nj       = cms.string("bTaggedJetsBetween(10,30,2.1,'pfTrackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+") == 0 && nSoftMu(3,1) == 0"),
        # bveto_nj30     = cms.string("bTaggedJetsBetween(10,30,2.1,'pfTrackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+") == 0 && nSoftMu(3,30) == 0"),
        # bveto_nj05     = cms.string("bTaggedJetsBetween(10,30,2.1,'pfTrackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+") == 0 && nSoftMu(3,1,0.5) == 0"),
        # bveto_nj3005   = cms.string("bTaggedJetsBetween(10,30,2.1,'pfTrackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+") == 0 && nSoftMu(3,30,0.5) == 0"),
        # bveto_mu       = cms.string("nSoftMu(3) == 0"),
        # bveto_munj     = cms.string("nSoftMu(3,1) == 0"),
        # bveto_munj30   = cms.string("nSoftMu(3,30) == 0"),
        # bveto_munj05   = cms.string("nSoftMu(3,1,0.5) == 0"),
        # bveto_munj3005 = cms.string("nSoftMu(3,30,0.5) == 0"),
        # dphiveto       = cms.string("passesDPhillJet("+DphiJetVetominPt+","+DphiJetVetominEta+",1,"+jetId_WP+")"),

    ),
    addRunLumiInfo = cms.bool(True)
)









def addMuonIdIsoVariables(process,pt):
    if hasattr(pt,"variables"):      
        setattr(pt.variables, "std_vector_muon_NValidHitsInTrk",      cms.string("muNValidHitsInTrkByPt")),
        setattr(pt.variables, "std_vector_muon_NValidFractInTrk",     cms.string("muNValidFractInTrkByPt")),
        setattr(pt.variables, "std_vector_muon_NormChi2GTrk",         cms.string("muNormChi2GTrkByPt")),
        setattr(pt.variables, "std_vector_muon_NValidHitsSATrk",      cms.string("muNValidHitsSATrkByPt")),
        setattr(pt.variables, "std_vector_muon_NumOfMatchedStations", cms.string("muNumOfMatchedStationsByPt")),
        setattr(pt.variables, "std_vector_muon_NValidPixelHitsInTrk", cms.string("muNValidPixelHitsInTrkByPt")),
        setattr(pt.variables, "std_vector_muon_NTkLayers",            cms.string("muNTkLayersByPt")),
        setattr(pt.variables, "std_vector_muon_TrkKink",              cms.string("muTrkKinkByPt")),
        setattr(pt.variables, "std_vector_muon_Chi2LocalPos",         cms.string("muChi2LocalPosByPt")), 
        setattr(pt.variables, "std_vector_muon_SegCompatibilty",      cms.string("muSegCompatibiltyByPt")),
    else:
        raise RuntimeError, "In addMuonIdIsoVariables, %s doesn't look like a ProbeTreeProducer object, it has no 'variables' attribute." % pt


def addEleIdIsoVariables(process,pt):
    if hasattr(pt,"variables"):      
        setattr(pt.variables, "std_vector_electron_dEtaSeedCalo" ,            cms.string("deltaEtaSeedClusterTrackAtCaloByPt")),
        setattr(pt.variables, "std_vector_electron_dEtaClusterCalo" ,         cms.string("deltaEtaEleClusterTrackAtCaloByPt")),
        setattr(pt.variables, "std_vector_electron_dEtaIn" ,                  cms.string("deltaEtaSuperClusterTrackAtVtxByPt")),
        setattr(pt.variables, "std_vector_electron_dPhiSeedCalo" ,            cms.string("deltaPhiSeedClusterTrackAtCaloByPt")),
        setattr(pt.variables, "std_vector_electron_dPhiClusterCalo" ,         cms.string("deltaPhiEleClusterTrackAtCaloByPt")),
        setattr(pt.variables, "std_vector_electron_dPhiIn" ,                  cms.string("deltaPhiSuperClusterTrackAtVtxByPt")),
        setattr(pt.variables, "std_vector_electron_full5x5_sigmaIetaIeta",    cms.string("full5x5_sigmaIetaIetaByPt")),
        setattr(pt.variables, "std_vector_electron_hOverE" ,                  cms.string("hcalOverEcalByPt")),
        setattr(pt.variables, "std_vector_electron_ooEmooP",                  cms.string("ooEmooPByPt")),
        setattr(pt.variables, "std_vector_electron_effectiveArea",            cms.string("GetElectronEffectiveAreaByPt")),
        setattr(pt.variables, "std_vector_electron_scEta",                    cms.string("etaSCByPt")),
        setattr(pt.variables, "std_vector_electron_expectedMissingInnerHits", cms.string("expectedMissingInnerHitsByPt")),
        setattr(pt.variables, "std_vector_electron_expectedMissingOuterHits", cms.string("expectedMissingOuterHitsByPt")),
        setattr(pt.variables, "std_vector_electron_expectedMissingTrackHits", cms.string("expectedMissingTrackHitsByPt")),
        setattr(pt.variables, "std_vector_electron_passConversionVeto",       cms.string("passConversionVetoByPt")),
        setattr(pt.variables, "std_vector_electron_ecalPFClusterIso",         cms.string("ecalPFClusterIsoByPt")),
        setattr(pt.variables, "std_vector_electron_hcalPFClusterIso",         cms.string("hcalPFClusterIsoByPt")),
        # tracker
        setattr(pt.variables, "std_vector_electron_gsfchi2",                  cms.string("gsfchi2ByPt")),
        setattr(pt.variables, "std_vector_electron_gsfndof",                  cms.string("gsfndofByPt")),
        setattr(pt.variables, "std_vector_electron_gsfnormalizedchi2",        cms.string("gsfnormalizedchi2ByPt")),

        # ECAL
        setattr(pt.variables, "std_vector_electron_full5x5R9",         cms.string("full5x5R9ByPt")),
        setattr(pt.variables, "std_vector_electron_R9",                cms.string("R9ByPt")),
        
        setattr(pt.variables, "std_vector_electron_fbrem",             cms.string("FbremByPt")),
        setattr(pt.variables, "std_vector_electron_seedEnergy",        cms.string("SeedEnergyByPt")),
        setattr(pt.variables, "std_vector_electron_energy5x5",         cms.string("Energy5x5ByPt")),

	setattr(pt.variables, "std_vector_electron_tripleChargeAgreement",    cms.string("tripleChargeAgreementByPt")),        

    else:
        raise RuntimeError, "In addEleIdIsoVariables, %s doesn't look like a ProbeTreeProducer object, it has no 'variables' attribute." % pt



def addJetsVariables(pt,dzCut=99999):
    if hasattr(pt,"variables"):

        pt.variables.std_vector_jet_puid = cms.string("leadingJetPUid")

def addFakeRateVariables(pt):
    if hasattr(pt,"variables"):

        pt.variables.std_vector_lepton_closejet_pt            = cms.string("leadingJetCloseLeptonPt")
        pt.variables.std_vector_lepton_closejet_eta           = cms.string("leadingJetCloseLeptonEta")
        pt.variables.std_vector_lepton_closejet_phi           = cms.string("leadingJetCloseLeptonPhi")
        pt.variables.std_vector_lepton_closejet_PartonFlavour = cms.string("leadingJetCloseLeptonFlavour")
        pt.variables.std_vector_lepton_closejet_drlj          = cms.string("leadingJetCloseLeptonDR")



def addBTaggingVariables(pt,dzCut=99999):
    if hasattr(pt,"variables"):

        pt.variables.softcsvv2ivf = cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'pfCombinedInclusiveSecondaryVertexV2BJetTags',"+jetId_WP+",%f)"%dzCut)
        pt.variables.hardcsvv2ivf = cms.string("highestBDiscRange("+CJVminPt+",999999.,'pfCombinedInclusiveSecondaryVertexV2BJetTags',"+jetId_WP+",%f,1)"%dzCut)
        pt.variables.softssvhe    = cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'pfSimpleSecondaryVertexHighEffBJetTags',"+jetId_WP+",%f)"%dzCut)
        pt.variables.hardssvhe    = cms.string("highestBDiscRange("+CJVminPt+",999999.,'pfSimpleSecondaryVertexHighEffBJetTags',"+jetId_WP+",%f,1)"%dzCut)
        pt.variables.softssvhb    = cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'pfSimpleSecondaryVertexHighPurBJetTags',"+jetId_WP+",%f)"%dzCut)
        pt.variables.hardssvhb    = cms.string("highestBDiscRange("+CJVminPt+",999999.,'pfSimpleSecondaryVertexHighPurBJetTags',"+jetId_WP+",%f,1)"%dzCut)
        pt.variables.softpfcsv    = cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'pfCombinedSecondaryVertexBJetTags',"+jetId_WP+",%f)"%dzCut)
        pt.variables.hardpfcsv    = cms.string("highestBDiscRange("+CJVminPt+",999999.,'pfCombinedSecondaryVertexBJetTags',"+jetId_WP+",%f,1)"%dzCut)
        pt.variables.softcmvav2     = cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'pfCombinedMVAV2BJetTags',"+jetId_WP+",%f)"%dzCut)
        pt.variables.hardcmvav2     = cms.string("highestBDiscRange("+CJVminPt+",999999.,'pfCombinedMVAV2BJetTags',"+jetId_WP+",%f,1)"%dzCut)

        # FIXME: move the index of jets as last step and allow saving of jets as above
        #        no need to give information to skimeventproducer via 2 roads!
        
        pt.variables.std_vector_jet_csvv2ivf  = cms.string("jetcsvv2ivfByPt")
        pt.variables.std_vector_jet_ssvhe     = cms.string("jetssvheByPt")
        pt.variables.std_vector_jet_ssvhb     = cms.string("jetssvhbByPt")
        pt.variables.std_vector_jet_pfcsv     = cms.string("jetpfcsvByPt")
        pt.variables.std_vector_jet_cmvav2    = cms.string("jetcmvav2ByPt")
        pt.variables.std_vector_jet_tche      = cms.string("jettcheByPt")
        pt.variables.std_vector_jet_tchp      = cms.string("jettchpByPt")
        pt.variables.std_vector_jet_cCVSLjet  = cms.string("jetpfCombinedCvsLJetTagsByPt")
        pt.variables.std_vector_jet_cCVSBjet  = cms.string("jetpfCombinedCvsBJetTagsByPt")

        pt.variables.std_vector_jet_softMuPt  = cms.string("jetSoftMuonPtByPt")
        pt.variables.std_vector_jet_softMuEta = cms.string("jetSoftMuonEtaByPt")
        pt.variables.std_vector_jet_softMuPhi = cms.string("jetSoftMuonPhiByPt")
        pt.variables.std_vector_jet_softMuIso = cms.string("jetSoftMuonIsoByPt")
        pt.variables.std_vector_jet_softMuD0  = cms.string("jetSoftMuonD0ByPt")
        pt.variables.std_vector_jet_softMuDz  = cms.string("jetSoftMuonDzByPt")
        pt.variables.std_vector_jet_NumberSoftMu = cms.string("jetSoftMuonCountingByPt")

        pt.variables.std_vector_softMuPt      = cms.string("SoftMuonPtByPt")
        pt.variables.std_vector_softMuEta     = cms.string("SoftMuonEtaByPt")
        pt.variables.std_vector_softMuPhi     = cms.string("SoftMuonPhiByPt")
        pt.variables.std_vector_softMuIso     = cms.string("SoftMuonIsoByPt")
        pt.variables.std_vector_softMuDz      = cms.string("SoftMuonDzByPt")
        pt.variables.std_vector_softMuD0      = cms.string("SoftMuonDxyByPt")
        pt.variables.std_vector_softMuIsTrackerMuon = cms.string("SoftMuonIsTrackerMuonByPt")
        pt.variables.std_vector_softMuTMLastStationAngTight = cms.string("TMLastStationAngTightByPt")

        pt.variables.jetcsvv2ivf1 = cms.string("leadingJetBtag(0,'pfCombinedInclusiveSecondaryVertexV2BJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)
        pt.variables.jetcsvv2ivf2 = cms.string("leadingJetBtag(1,'pfCombinedInclusiveSecondaryVertexV2BJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)
        pt.variables.jetssvhe1 = cms.string("leadingJetBtag(0,'pfSimpleSecondaryVertexHighEffBJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)
        pt.variables.jetssvhe2 = cms.string("leadingJetBtag(1,'pfSimpleSecondaryVertexHighEffBJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)
        pt.variables.jetssvhb1 = cms.string("leadingJetBtag(0,'pfSimpleSecondaryVertexHighPurBJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)
        pt.variables.jetssvhb2 = cms.string("leadingJetBtag(1,'pfSimpleSecondaryVertexHighPurBJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)
        pt.variables.jetpfcsv1 = cms.string("leadingJetBtag(0,'pfCombinedSecondaryVertexBJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)
        pt.variables.jetpfcsv2 = cms.string("leadingJetBtag(1,'pfCombinedSecondaryVertexBJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)
        pt.variables.jetcmvav21 = cms.string("leadingJetBtag(0,'pfCombinedMVAV2BJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)
        pt.variables.jetcmvav22 = cms.string("leadingJetBtag(1,'pfCombinedMVAV2BJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)

    else:
        raise RuntimeError, "In addBTaggingVariables, %s doesn't look like a ProbeTreeProducer object, it has no 'variables' attribute." % pt

# LHE information
def addLHEVariables(process,pt):
    if hasattr(pt,"variables"):
        setattr(pt.variables, "std_vector_LHElepton_pt" , cms.string("leadingLHELeptonPt/5")),
        setattr(pt.variables, "std_vector_LHElepton_eta" , cms.string("leadingLHELeptonEta/5")),
        setattr(pt.variables, "std_vector_LHElepton_phi" , cms.string("leadingLHELeptonPhi/5")),
        setattr(pt.variables, "std_vector_LHElepton_id" , cms.string("leadingLHELeptonPID/5")),

        setattr(pt.variables, "std_vector_LHEparton_pt" , cms.string("leadingLHEJetPt/5")),
        setattr(pt.variables, "std_vector_LHEparton_eta" , cms.string("leadingLHEJetEta/5")),
        setattr(pt.variables, "std_vector_LHEparton_phi" , cms.string("leadingLHEJetPhi/5")),
        setattr(pt.variables, "std_vector_LHEparton_id" , cms.string("leadingLHEJetPID/5")),

        setattr(pt.variables, "std_vector_LHEneutrino_pt" , cms.string("leadingLHENeutrinoPt/5")),
        setattr(pt.variables, "std_vector_LHEneutrino_eta" , cms.string("leadingLHENeutrinoEta/5")),
        setattr(pt.variables, "std_vector_LHEneutrino_phi" , cms.string("leadingLHENeutrinoPhi/5")),
        setattr(pt.variables, "std_vector_LHEneutrino_id" , cms.string("leadingLHENeutrinoPID/5")),

        setattr(pt.variables, "metLHEpt" , cms.string("LHEMetPt()")),
        setattr(pt.variables, "metLHEphi" , cms.string("LHEMetPhi()")),
        setattr(pt.variables, "metLHEeta" , cms.string("LHEMetEta()")),

        setattr(pt.variables, "higgsLHEpt"  , cms.string("higgsLHEPt()")),
        setattr(pt.variables, "higgsLHEeta" , cms.string("higgsLHEEta()")),
        setattr(pt.variables, "higgsLHEphi" , cms.string("higgsLHEPhi()")),
        setattr(pt.variables, "higgsLHEmass", cms.string("higgsLHEmass()")),

    else:
        raise addLHEVariables, "In addLHEVariables, %s doesn't look like a ProbeTreeProducer object, it has no 'variables' attribute." % pt

# gen information
def addGenVariables(process,pt):
    if hasattr(pt,"variables"):
        setattr(pt.variables, "std_vector_partonGen_pt",            cms.string("leadingGenJetPartonPt/15")),
        setattr(pt.variables, "std_vector_partonGen_eta" ,          cms.string("leadingGenJetPartonEta/15")),
        setattr(pt.variables, "std_vector_partonGen_phi",           cms.string("leadingGenJetPartonPhi/15")),
        setattr(pt.variables, "std_vector_partonGen_pid",           cms.string("leadingGenJetPartonPID/15")),
        setattr(pt.variables, "std_vector_partonGen_isPrompt",      cms.string("leadingGenJetPartonIsPrompt/15")),
        setattr(pt.variables, "std_vector_partonGen_isHardProcess", cms.string("leadingGenJetPartonIsHardProcess/15")),

        setattr(pt.variables, "std_vector_leptonGen_pt",                            cms.string("genLeptonPt")),
        setattr(pt.variables, "std_vector_leptonGen_eta" ,                          cms.string("genLeptonEta")),
        setattr(pt.variables, "std_vector_leptonGen_phi",                           cms.string("genLeptonPhi")),
        setattr(pt.variables, "std_vector_leptonGen_pid",                           cms.string("genLeptonPID")),
        setattr(pt.variables, "std_vector_leptonGen_status",                        cms.string("genLeptonStatus")),
        setattr(pt.variables, "std_vector_leptonGen_isPrompt",                      cms.string("genLeptonIsPrompt")),
        setattr(pt.variables, "std_vector_leptonGen_isDirectPromptTauDecayProduct", cms.string("genLeptonIsDirectPromptTauDecayProduct")),
        setattr(pt.variables, "std_vector_leptonGen_isTauDecayProduct",             cms.string("genLeptonIsTauDecayProduct")),
        setattr(pt.variables, "std_vector_leptonGen_isDirectHadronDecayProduct",    cms.string("genLeptonIsDirectHadronDecayProduct")),
        setattr(pt.variables, "std_vector_leptonGen_fromHardProcess",               cms.string("genLeptonFromHardProcess")),
        setattr(pt.variables, "std_vector_leptonGen_MotherPID",                     cms.string("genLeptonMotherPID")),
        setattr(pt.variables, "std_vector_leptonGen_MotherStatus",                  cms.string("genLeptonMotherStatus")),
        #setattr(pt.variables, "std_vector_leptonGen_fromHardProcessBeforeFSR", cms.string("genLeptonFromHardProcessBeforeFSR")),

        setattr(pt.variables, "std_vector_VBoson_pt",      cms.string("genVBosonPt/5")),
        setattr(pt.variables, "std_vector_VBoson_eta" ,    cms.string("genVBosonEta/5")),
        setattr(pt.variables, "std_vector_VBoson_phi",     cms.string("genVBosonPhi/5")),
        setattr(pt.variables, "std_vector_VBoson_mass",    cms.string("genVBosonMass/5")),
        setattr(pt.variables, "std_vector_VBoson_pid",     cms.string("genVBosonPID/5")),
        #setattr(pt.variables, "std_vector_VBoson_status",  cms.string("genVBosonStatus/5")),
        #setattr(pt.variables, "std_vector_VBoson_fromHardProcessBeforeFSR", cms.string("genVBosonFromHardProcessBeforeFSR/5")),

        setattr(pt.variables, "std_vector_neutrinoGen_pt",      cms.string("leadingGenNeutrinoPt")),
        setattr(pt.variables, "std_vector_neutrinoGen_eta" ,    cms.string("leadingGenNeutrinoEta")),
        setattr(pt.variables, "std_vector_neutrinoGen_phi",     cms.string("leadingGenNeutrinoPhi")),
        setattr(pt.variables, "std_vector_neutrinoGen_pid",     cms.string("leadingGenNeutrinoPID")),
        setattr(pt.variables, "std_vector_neutrinoGen_isPrompt",                          cms.string("leadingGenNeutrinoIsPrompt")),
        setattr(pt.variables, "std_vector_neutrinoGen_isDirectPromptTauDecayProduct",     cms.string("leadingGenNeutrinoIsDirectPromptTauDecayProduct")),
        setattr(pt.variables, "std_vector_neutrinoGen_MotherPID",      cms.string("leadingGenNeutrinoMotherPID")),
        setattr(pt.variables, "std_vector_neutrinoGen_MotherStatus",      cms.string("leadingGenNeutrinoMotherStatus")),
        #setattr(pt.variables, "std_vector_neutrinoGen_fromHardProcessBeforeFSR", cms.string("leadingGenNeutrinoFromHardProcessBeforeFSR")),
        
        setattr(pt.variables, "std_vector_photonGen_pt",   cms.string("genPhotonPt")),
        setattr(pt.variables, "std_vector_photonGen_phi",  cms.string("genPhotonPhi")),
        setattr(pt.variables, "std_vector_photonGen_eta",  cms.string("genPhotonEta")),
        setattr(pt.variables, "std_vector_photonGen_MotherPID",  cms.string("genPhotonMotherPID")),
        setattr(pt.variables, "std_vector_photonGen_MotherStatus",  cms.string("genPhotonMotherStatus")),

        setattr(pt.variables, "higgsGenpt" , cms.string("getHiggsPt()")),
        setattr(pt.variables, "higgsGeneta" , cms.string("getHiggsEta()")),
        setattr(pt.variables, "higgsGenphi" , cms.string("getHiggsPhi()")),
        setattr(pt.variables, "higgsGenmass" , cms.string("getHiggsMass()")),

        setattr(pt.variables, "metGenpt" , cms.string("genMetPt()")),
        setattr(pt.variables, "metGeneta", cms.string("genMetEta()")),
        setattr(pt.variables, "metGenphi", cms.string("genMetPhi()")),

        setattr(pt.variables, "std_vector_jetGen_pt" , cms.string("leadingGenJetPt")),
        setattr(pt.variables, "std_vector_jetGen_phi", cms.string("leadingGenJetPhi")),
        setattr(pt.variables, "std_vector_jetGen_eta", cms.string("leadingGenJetEta")),

        setattr(pt.variables, "std_vector_jet_HadronFlavour", cms.string("leadingJetHadronFlavour")),
        setattr(pt.variables, "std_vector_jet_PartonFlavour", cms.string("leadingJetPartonFlavour")),

        setattr(pt.variables, "std_vector_dressedLeptonGen_pt",                                cms.string("dressedLepton_ptByPt")),
        setattr(pt.variables, "std_vector_dressedLeptonGen_eta" ,                              cms.string("dressedLepton_etaByPt")),
        setattr(pt.variables, "std_vector_dressedLeptonGen_phi",                               cms.string("dressedLepton_phiByPt")),
        setattr(pt.variables, "std_vector_dressedLeptonGen_pid",                               cms.string("dressedLepton_pdgIdByPt")),


    else:
        raise addGenVariables, "In addGenVariables, %s doesn't look like a ProbeTreeProducer object, it has no 'variables' attribute." % pt

    
def addMCweights(process,pt):

        if hasattr(pt,"variables"):
          #pt.variables.std_vector_LHE_weightID = cms.string("LHEMCweightID/300")

          pt.variables.std_vector_LHE_weight = cms.string("LHEMCweight/250")
          pt.variables.std_vector_GEN_weight = cms.string("GENMCweight/250")
          pt.variables.LHE_weight_SM = cms.string("LHEMCweight(-1)")
          pt.variables.GEN_weight_SM = cms.string("GENMCweight(-1)")


def addTau(process,pt):

        if hasattr(pt,"variables"):
          pt.variables.std_vector_tau_pt  = cms.string("leadingTauPt")
          pt.variables.std_vector_tau_eta = cms.string("leadingTauEta")
          pt.variables.std_vector_tau_phi = cms.string("leadingTauPhi")


def addFatJets(process,pt):

        fatjetId_WP = "1"

        if hasattr(pt,"variables"):

            #std_vector_fatjet_pt   = cms.string("leadingFatJetPt(0," + CJVmaxEta + "1," + fatjetId_WP + ",\"\""),
            setattr(pt.variables, "std_vector_fatjet_pt",            cms.string("leadingFatJetPt(0,"+CJVmaxEta+",1,"+fatjetId_WP+"")),
            setattr(pt.variables, "std_vector_fatjet_eta",           cms.string("leadingFatJetEta(0,"+CJVmaxEta+",1,"+fatjetId_WP+"")),
            setattr(pt.variables, "std_vector_fatjet_phi",           cms.string("leadingFatJetPhi(0,"+CJVmaxEta+",1,"+fatjetId_WP+"")),
            setattr(pt.variables, "std_vector_fatjet_trimmedmass",   cms.string("leadingFatJetTrimmedMass(0,"+CJVmaxEta+",1,"+fatjetId_WP+"")),
            setattr(pt.variables, "std_vector_fatjet_filteredmass",  cms.string("leadingFatJetFilteredMass(0,"+CJVmaxEta+",1,"+fatjetId_WP+"")),
            setattr(pt.variables, "std_vector_fatjet_prunedmass",    cms.string("leadingFatJetPrunedMass(0,"+CJVmaxEta+",1,"+fatjetId_WP+"")),
            setattr(pt.variables, "std_vector_fatjet_massdrop",      cms.string("leadingFatJetMassDrop(0,"+CJVmaxEta+",1,"+fatjetId_WP+"")),
            #setattr(pt.variables, "std_vector_fatjet_tau2tau1",      cms.string("leadingFatJetPrunedTau2Tau1(0,"+CJVmaxEta+",1,"+fatjetId_WP+"")),
            setattr(pt.variables, "std_vector_fatjet_tau1",          cms.string("leadingFatJetPrunedTau1(0,"+CJVmaxEta+",1,"+fatjetId_WP+"")),
            setattr(pt.variables, "std_vector_fatjet_tau2",          cms.string("leadingFatJetPrunedTau2(0,"+CJVmaxEta+",1,"+fatjetId_WP+"")),
            setattr(pt.variables, "std_vector_fatjet_tau3",          cms.string("leadingFatJetPrunedTau3(0,"+CJVmaxEta+",1,"+fatjetId_WP+"")),
            #setattr(pt.variables, "std_vector_fatjet_tau4",          cms.string("leadingFatJetPrunedTau4(0,"+CJVmaxEta+",1,"+fatjetId_WP+"")),


def addQGJets(process,pt):

        if hasattr(pt,"variables"):
         
            #pt.variables.std_vector_jet_QGaxis1      = cms.string("leadingJetQGaxis1")
            #pt.variables.std_vector_jet_QGaxis2      = cms.string("leadingJetQGaxis2")
            #pt.variables.std_vector_jet_QGRMScand    = cms.string("leadingJetQGRMScand")
            #pt.variables.std_vector_jet_QGRmax       = cms.string("leadingJetQGRmax")
            pt.variables.std_vector_jet_QGlikelihood = cms.string("leadingJetQGlikelihood")


def addSoftActivityVariables(process,pt):
     if hasattr(pt,"variables"):
         # Require final lepton selection
         #setattr(pt.variables, "sumHtSoft",  cms.string("sumHtTrackJets")),
         #setattr(pt.variables, "sumHtSoftDensity", cms.string("sumHtTrackJetsDensity")),
         #setattr(pt.variables, "multiplicitySoft",  cms.string("multiplicityTrackJets")),
         #setattr(pt.variables, "multiplicitySoftDensity",  cms.string("multiplicityTrackJetsDensity")),
         setattr(pt.variables, "std_vector_trackjet_pt",  cms.string("trackJetPt/50")),
         setattr(pt.variables, "std_vector_trackjet_eta",  cms.string("trackJetEta/50")),
         setattr(pt.variables, "std_vector_trackjet_phi",  cms.string("trackJetPhi/50")),
         setattr(pt.variables, "std_vector_trackjet_probabilityB",  cms.string("trackJetProbabilityB/50")),
         

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



########## Photon variables

def addPhotonVariables(process,pt):
    if hasattr(pt,"variables"):
        #setattr(pt.variables, "v_photon1" ,    cms.string("photon(0)")),     # Deprecated, use std_vector_photon
        #setattr(pt.variables, "v_photon2" ,    cms.string("photon(1)")),     # Deprecated, use std_vector_photon
        #setattr(pt.variables, "v_photon3" ,    cms.string("photon(2)")),     # Deprecated, use std_vector_photon
        #setattr(pt.variables, "v_photon1id" ,  cms.string("photon_id(0)")),  # Deprecated, use std_vector_photon

        setattr(pt.variables, "std_vector_photon_pt",    cms.string("photon_ptByPt")),
        setattr(pt.variables, "std_vector_photon_eta",   cms.string("photon_etaByPt")),
        setattr(pt.variables, "std_vector_photon_phi",   cms.string("photon_phiByPt")),
        setattr(pt.variables, "std_vector_photonid_pt",  cms.string("photonid_ptByPt")),
        setattr(pt.variables, "std_vector_photonid_eta", cms.string("photonid_etaByPt")),
        setattr(pt.variables, "std_vector_photonid_phi", cms.string("photonid_phiByPt")),

        setattr(pt.variables, "nPhos",    cms.string("nPhos()")),
        setattr(pt.variables, "pho_n_id", cms.string("Pho_n_ID()")),
        setattr(pt.variables, "mllg",     cms.string("mllg()")),
        setattr(pt.variables, "mllgid",   cms.string("mllg()")),

def addPhotonIDVariables(process,pt):
        setattr(pt.variables, "pho_sietaieta" ,    cms.string("Pho_sigmaIetaIeta(0)")),
        setattr(pt.variables, "pho_HoE" ,          cms.string("Pho_hadronicOverEm(0)")),
        setattr(pt.variables, "pho_chIso" ,        cms.string("Pho_rhoChargedHadronIso(0)")),
        setattr(pt.variables, "pho_nhIso" ,        cms.string("Pho_rhoNeutralHadronIso(0)")),
        setattr(pt.variables, "pho_phIso" ,        cms.string("Pho_rhoPhotonIso(0)")),
        setattr(pt.variables, "pho_passElecVeto" , cms.string("Pho_PassElectronVeto(0)")),
        setattr(pt.variables, "pho_hasPixelSeed",  cms.string("Pho_HasPixelSeed(0)")),



