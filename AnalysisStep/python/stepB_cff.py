import FWCore.ParameterSet.Config as cms
import os
#from WWAnalysis.AnalysisStep.wwMuons_cfi import Scenario6_ICHEP2012,Scenario2_KINK_MUONS,Scenario1_LP_MUONS
#from WWAnalysis.AnalysisStep.wwElectrons_cfi import Scenario6_ICHEP,Scenario4_BDT_ELECTRONS,Scenario3_LH_ELECTRONS,Scenario2_LP_ELECTRONS,Scenario1_LP_ELECTRONS

# NB: typedef ObjectMultiplicityCounter<reco::Vertex>    VertexMultiplicityCounter;
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

#jetId_WP="4" ----> FIXME: to be used!
jetId_WP="0"

# JetCuts

CJVminPt="30."
CJVmaxEta="4.7"
DphiJetVetominPt="15."
DphiJetVetominEta="4.7"

DzBVeto="999999.9"
minPtBVeto="10.0"


stepBTree = cms.EDFilter("GenericTreeProducer",
    src = cms.InputTag(""),
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
        channel = cms.string("channel()"),
        v_lepton1 = cms.string("lepton(0)"),
        v_lepton2 = cms.string("lepton(1)"),
        v_lepton3 = cms.string("lepton(2)"),
        v_lepton4 = cms.string("lepton(3)"),

        v_jet1 = cms.string("jet(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        v_jet2 = cms.string("jet(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        v_jet3 = cms.string("jet(2,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        v_jet4 = cms.string("jet(3,0,"+CJVmaxEta+",1,"+jetId_WP+")"),

        std_vector_lepton_pt  = cms.string("ptByPt"),
        std_vector_lepton_eta = cms.string("etaByPt"),
        std_vector_lepton_phi = cms.string("phiByPt"),
        std_vector_lepton_id  = cms.string("pdgIdByPt"),

        std_vector_lepton_isTightMuon = cms.string("isTightMuonByPt"),



        std_vector_puppijet_pt  = cms.string("leadingSecondJetPt"),
        std_vector_puppijet_eta = cms.string("leadingSecondJetEta"),
        std_vector_puppijet_phi = cms.string("leadingSecondJetPhi"),

        std_vector_jet_pt  = cms.string("leadingJetPt"),
        std_vector_jet_eta = cms.string("leadingJetEta"),
        std_vector_jet_phi = cms.string("leadingJetPhi"),



        mll = cms.string("mll()"),
        ptll = cms.string("pTll()"),
        yll = cms.string("yll()"), #fixed! returns (p4a+p4b).Rapidity()
        pt1 = cms.string("ptMax"),
        pt2 = cms.string("ptMin"),
        pt3 = cms.string("ptByPt(2)"),
        pt4 = cms.string("ptByPt(3)"),
        
        isTightMuon1 = cms.string("isTightMuon(0)"),
        isTightMuon2 = cms.string("isTightMuon(1)"),
        isTightMuon3 = cms.string("isTightMuon(2)"),
        isTightMuon4 = cms.string("isTightMuon(3)"),
        
        isSTA1 = cms.string("isSTAByPt(0)"),
        isSTA2 = cms.string("isSTAByPt(1)"),
        isSTA3 = cms.string("isSTAByPt(2)"),
        isSTA4 = cms.string("isSTAByPt(3)"),
        peaking = cms.string("peaking"),
        trigger = cms.string("guillelmoTrigger('DATASET')"),
        nextra = cms.string("nExtraLep(10)"),
        pfSumEt = cms.string("pfSumEt"),
        pfmet = cms.string("pfMet"),
        pfmetphi = cms.string("pfMetPhi"),
        ppfmet = cms.string("projPfMet"),
        pupmet = cms.string("pupMet"),
        trkmet = cms.string("trkMet"),
# mvamet = cms.string("mvaMet"),
# mvamethi = cms.string("mvaMetPhi"),
# pmvamet = cms.string("projMvaMet"),
        chSumEt = cms.string("chargedMetSmurfSumEt"),
        pfmetMEtSig = cms.string("pfMetMEtSig"),
        pfmetSignificance = cms.string("pfMetSignificance"),
        chmet = cms.string("chargedMetSmurf"),
        chmetphi = cms.string("chargedMetSmurfPhi"),
        pchmet = cms.string("projChargedMetSmurf"),
        dymva0 = cms.string("userFloat('dymva0')"),
        dymva1 = cms.string("userFloat('dymva1')"),
        redmet = cms.string("-9999"),
        predmet = cms.string("-9999"),
        mpmet = cms.string("min(projPfMet,projChargedMetSmurf)"), ##note: min of proj and proj of min are not the same
        imet = cms.string("min(projPfMet,projChargedMetSmurf)*cos(pfMetPhi-chargedMetSmurfPhi)"),
        dphill = cms.string("dPhill()"),
        drll = cms.string("dRll()"),
        dphilljet = cms.string("dPhillLeadingJet("+CJVmaxEta+",1,"+jetId_WP+")"),
        dphilljetjet = cms.string("dPhilljetjet("+CJVmaxEta+",1,"+jetId_WP+")"), #WHAT
        dphillmet = cms.string("dPhillMet('PFMET')"),
        dphilmet = cms.string("dPhilMet('PFMET')"),
        dphilmet1 = cms.string("dPhilMetByPt(0,'PFMET')"),
        dphilmet2 = cms.string("dPhilMetByPt(1,'PFMET')"),
        mtw1 = cms.string("mTByPt(0,'PFMET')"),
        mtw2 = cms.string("mTByPt(1,'PFMET')"),
        mth = cms.string("mTHiggs('PFMET')"),
        gammaMRStar = cms.string("gammaMRStar"),
        njet = cms.string("nCentralJets("+CJVminPt+","+CJVmaxEta+",1,"+jetId_WP+")"),
        njetid = cms.string("nCentralJets("+CJVminPt+","+CJVmaxEta+",1,0)"),
        # here we do apply a dz cut cause we are actually counting bjets
        nbjettche = cms.string("bTaggedJetsOver("+CJVminPt+",2.1,'trackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+")"),
        nbjet = cms.string("bTaggedJetsOver("+CJVminPt+",1.05,'jetBProbabilityBJetTags',"+jetId_WP+","+DzBVeto+")"),
        # here we don't apply the dz cut, cause we just use the b-tag value of highest pt jets

        puppijetpt1 = cms.string("leadingSecondJetPt(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
       
        jetpt1 = cms.string("leadingJetPt(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jeteta1 = cms.string("leadingJetEta(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetphi1 = cms.string("leadingJetPhi(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetmass1 = cms.string("leadingJetMass(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetid1 = cms.string("leadingJetId(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetmva1 = cms.string("leadingJetMva(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetbjpb1 = cms.string("leadingJetBtag(0,'jetBProbabilityBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")"),
        jettche1 = cms.string("leadingJetBtag(0,'trackCountingHighEffBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")"),
        jettchp1 = cms.string("leadingJetBtag(0,'trackCountingHighPurBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")"),
        jetptd1 = cms.string("leadingJetPtd(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetCHM1 = cms.string("leadingJetChargedHadronMultiplicity(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetNHM1 = cms.string("leadingJetNeutralHadronMultiplicity(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetPhM1 = cms.string("leadingJetPhotonMultiplicity(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetQCptD1 = cms.string("leadingJetPtD(0,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetQCaxis11 = cms.string("leadingJetQGaxis1(0,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetQCaxis21 = cms.string("leadingJetQGaxis2(0,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetQCRMScand1 = cms.string("leadingJetQGRMScand(0,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetQCRmax1 = cms.string("leadingJetQGRmax(0,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetptD1 = cms.string("leadingJetPtD(0,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetaxis11 = cms.string("leadingJetQGaxis1(0,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetaxis21 = cms.string("leadingJetQGaxis2(0,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetRMScand1 = cms.string("leadingJetQGRMScand(0,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetRmax1 = cms.string("leadingJetQGRmax(0,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetNChgQC1 = cms.string("leadingJetNChgQC(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetNChgptCut1 = cms.string("leadingJetNChgptCut(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetNNeutralptCut1 = cms.string("leadingJetNNeutralptCut(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),

        jetpt2 = cms.string("leadingJetPt(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jeteta2 = cms.string("leadingJetEta(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetphi2 = cms.string("leadingJetPhi(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetmass2 = cms.string("leadingJetMass(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetid2 = cms.string("leadingJetId(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetmva2 = cms.string("leadingJetMva(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jettche2 = cms.string("leadingJetBtag(1,'trackCountingHighEffBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")"),
        jetbjpb2 = cms.string("leadingJetBtag(1,'jetBProbabilityBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")"),
        jettchp2 = cms.string("leadingJetBtag(1,'trackCountingHighPurBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")"),
        jetptd2 = cms.string("leadingJetPtd(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetCHM2 = cms.string("leadingJetChargedHadronMultiplicity(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetNHM2 = cms.string("leadingJetNeutralHadronMultiplicity(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetPhM2 = cms.string("leadingJetPhotonMultiplicity(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetQCptD2 = cms.string("leadingJetPtD(1,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetQCaxis12 = cms.string("leadingJetQGaxis1(1,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetQCaxis22 = cms.string("leadingJetQGaxis2(1,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetQCRMScand2 = cms.string("leadingJetQGRMScand(1,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetQCRmax2 = cms.string("leadingJetQGRmax(1,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetptD2 = cms.string("leadingJetPtD(1,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetaxis12 = cms.string("leadingJetQGaxis1(1,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetaxis22 = cms.string("leadingJetQGaxis2(1,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetRMScand2 = cms.string("leadingJetQGRMScand(1,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetRmax2 = cms.string("leadingJetQGRmax(1,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetNChgQC2 = cms.string("leadingJetNChgQC(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetNChgptCut2 = cms.string("leadingJetNChgptCut(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetNNeutralptCut2 = cms.string("leadingJetNNeutralptCut(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),

        jetpt3 = cms.string("leadingJetPt(2,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jeteta3 = cms.string("leadingJetEta(2,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetphi3 = cms.string("leadingJetPhi(2,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetmass3 = cms.string("leadingJetMass(2,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetid3 = cms.string("leadingJetId(2,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetmva3 = cms.string("leadingJetMva(2,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jettche3 = cms.string("leadingJetBtag(2,'trackCountingHighEffBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")"),
        jetbjpb3 = cms.string("leadingJetBtag(2,'jetBProbabilityBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")"),
        jettchp3 = cms.string("leadingJetBtag(2,'trackCountingHighPurBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")"),
        jetptd3 = cms.string("leadingJetPtd(2,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetCHM3 = cms.string("leadingJetChargedHadronMultiplicity(2,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetNHM3 = cms.string("leadingJetNeutralHadronMultiplicity(2,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetPhM3 = cms.string("leadingJetPhotonMultiplicity(2,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetQCptD3 = cms.string("leadingJetPtD(2,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetQCaxis13 = cms.string("leadingJetQGaxis1(2,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetQCaxis23 = cms.string("leadingJetQGaxis2(2,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetQCRMScand3 = cms.string("leadingJetQGRMScand(2,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetQCRmax3 = cms.string("leadingJetQGRmax(2,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetptD3 = cms.string("leadingJetPtD(2,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetaxis13 = cms.string("leadingJetQGaxis1(2,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetaxis23 = cms.string("leadingJetQGaxis2(2,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetRMScand3 = cms.string("leadingJetQGRMScand(2,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetRmax3 = cms.string("leadingJetQGRmax(2,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetNChgQC3 = cms.string("leadingJetNChgQC(2,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetNChgptCut3 = cms.string("leadingJetNChgptCut(2,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetNNeutralptCut3 = cms.string("leadingJetNNeutralptCut(2,0,"+CJVmaxEta+",1,"+jetId_WP+")"),

        jetpt4 = cms.string("leadingJetPt(3,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jeteta4 = cms.string("leadingJetEta(3,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetphi4 = cms.string("leadingJetPhi(3,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetmass4 = cms.string("leadingJetMass(3,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetid4 = cms.string("leadingJetId(3,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetmva4 = cms.string("leadingJetMva(3,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jettche4 = cms.string("leadingJetBtag(3,'trackCountingHighEffBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")"),
        jetbjpb4 = cms.string("leadingJetBtag(3,'jetBProbabilityBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")"),
        jettchp4 = cms.string("leadingJetBtag(3,'trackCountingHighPurBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")"),
        jetptd4 = cms.string("leadingJetPtd(3,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetCHM4 = cms.string("leadingJetChargedHadronMultiplicity(3,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetNHM4 = cms.string("leadingJetNeutralHadronMultiplicity(3,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetPhM4 = cms.string("leadingJetPhotonMultiplicity(3,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetQCptD4 = cms.string("leadingJetPtD(3,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetQCaxis14 = cms.string("leadingJetQGaxis1(3,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetQCaxis24 = cms.string("leadingJetQGaxis2(3,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetQCRMScand4 = cms.string("leadingJetQGRMScand(3,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetQCRmax4 = cms.string("leadingJetQGRmax(3,0,"+CJVmaxEta+",1,"+jetId_WP+",1)"),
        jetptD4 = cms.string("leadingJetPtD(3,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetaxis14 = cms.string("leadingJetQGaxis1(3,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetaxis24 = cms.string("leadingJetQGaxis2(3,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetRMScand4 = cms.string("leadingJetQGRMScand(3,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetRmax4 = cms.string("leadingJetQGRmax(3,0,"+CJVmaxEta+",1,"+jetId_WP+",0)"),
        jetNChgQC4 = cms.string("leadingJetNChgQC(3,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetNChgptCut4 = cms.string("leadingJetNChgptCut(3,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        jetNNeutralptCut4 = cms.string("leadingJetNNeutralptCut(3,0,"+CJVmaxEta+",1,"+jetId_WP+")"),


        jetRho = cms.string("getJetRhoIso()"),
        iso1 = cms.string("allIsoByPt(0)/ptByPt(0)"),
        iso2 = cms.string("allIsoByPt(1)/ptByPt(1)"),
        iso3 = cms.string("allIsoByPt(2)/ptByPt(2)"),
        iso4 = cms.string("allIsoByPt(3)/ptByPt(3)"),
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
        lh1 = cms.string("leptLHByPt(0)"),
        lh2 = cms.string("leptLHByPt(1)"),
        lh3 = cms.string("leptLHByPt(2)"),
        lh4 = cms.string("leptLHByPt(3)"),

        pdgid1 = cms.string("pdgIdByPt(0)"),
        pdgid2 = cms.string("pdgIdByPt(1)"),
        pdgid3 = cms.string("pdgIdByPt(2)"),
        pdgid4 = cms.string("pdgIdByPt(3)"),

        nbrem1 = cms.string("nBremByPt(0)"),
        nbrem2 = cms.string("nBremByPt(1)"),
        nbrem3 = cms.string("nBremByPt(2)"),
        nbrem4 = cms.string("nBremByPt(3)"),
        isomva1 = cms.string("mvaIsoByPt(0)"),
        isomva2 = cms.string("mvaIsoByPt(1)"),
        isomva3 = cms.string("mvaIsoByPt(2)"),
        isomva4 = cms.string("mvaIsoByPt(3)"),
        # in the 2012 selection, 2 BJet algorithms are used: softtche and hardbjpb !
        # softbdisc = cms.string("highestBDiscRange(10.0,30.0,'trackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+")"),
        # hardbdisc = cms.string("highestBDiscRange(30.0,999999.,'jetBProbabilityBJetTags',"+jetId_WP+","+DzBVeto+",1)"),
        softtche = cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'trackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+")"),
        hardtche = cms.string("highestBDiscRange("+CJVminPt+",999999.,'trackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+",1)"),
        softbjpb = cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'jetBProbabilityBJetTags',"+jetId_WP+","+DzBVeto+")"),
        hardbjpb = cms.string("highestBDiscRange("+CJVminPt+",999999.,'jetBProbabilityBJetTags',"+jetId_WP+","+DzBVeto+",1)"),
        tightmu = cms.string("passesSmurfMuonID"),
        worstJetLepPt = cms.string("max(matchedJetPt(0, 0.5)/pt(0), matchedJetPt(1, 0.5)/pt(1))"),
        dataset = cms.string("REPLACE_ME"),
        #puAW = cms.InputTag("puWeightA"),  # miniAOD
        #puBW = cms.InputTag("puWeightB"),  # miniAOD
        #puW = cms.InputTag("puWeight"),  # miniAOD
        puW = cms.string("1"),
        kfW = cms.string("1"),
        #kfW = cms.InputTag("ptWeight"),
        baseW = cms.string("REPLACE_ME"),
        #fourW = cms.string("REPLACE_ME"),
        #fermiW = cms.string("REPLACE_ME"),

        trpu = cms.InputTag("nPU:tr"),
        itpu = cms.InputTag("nPU:it"),
        ootpup1 = cms.InputTag("nPU:p1"),
        ootpum1 = cms.InputTag("nPU:m1"),


        effAW = cms.string("1"),
        effBW = cms.string("1"),
        effW = cms.string("1"),
        triggAW = cms.string("1"),
        triggBW = cms.string("1"),
        triggW = cms.string("1"),
        fakeAW = cms.string("1"),
        fakeBW = cms.string("1"),
        fakeW = cms.string("1"),
        #vbf stuff:
        njetvbf = cms.string("nJetVBF(30,"+CJVmaxEta+",1,"+jetId_WP+")"),
        mjj = cms.string("mjj(0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        detajj = cms.string("dEtajj(30,"+CJVmaxEta+",1,"+jetId_WP+")"),
        cjetpt1 = cms.string("leadingVBFJetPt(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        cjeteta1 = cms.string("leadingVBFJetEta(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        cjetphi1 = cms.string("leadingVBFJetPhi(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        cjetid1 = cms.string("leadingVBFJetId(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        cjetmva1 = cms.string("leadingVBFJetMva(0,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        cjetpt2 = cms.string("leadingVBFJetPt(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        cjeteta2 = cms.string("leadingVBFJetEta(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        cjetphi2 = cms.string("leadingVBFJetPhi(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        cjetid2 = cms.string("leadingVBFJetId(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        cjetmva2 = cms.string("leadingVBFJetMva(1,0,"+CJVmaxEta+",1,"+jetId_WP+")"),
        # mc info
        mctruth = cms.string("-1"),

    ),
    flags = cms.PSet(
        #sameflav = cms.string("hypo == 3 || hypo == 6"),
        #zveto = cms.string("abs(mll-91.1876)>15. || hypo == 4 || hypo == 5"),
        # here we do apply a dz cut cause we are actually counting bjets
        bveto = cms.string("bTaggedJetsBetween(10,30,2.1,'trackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+") == 0 && nSoftMu(3) == 0"),
        bveto_ip = cms.string("bTaggedJetsBetween(10,30,2.1,'trackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+") == 0"),
        bveto_mu = cms.string("nSoftMu(3) == 0"),
        bveto_nj = cms.string("bTaggedJetsBetween(10,30,2.1,'trackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+") == 0 && nSoftMu(3,1) == 0"),
        bveto_munj = cms.string("nSoftMu(3,1) == 0"),
        bveto_nj30 = cms.string("bTaggedJetsBetween(10,30,2.1,'trackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+") == 0 && nSoftMu(3,30) == 0"),
        bveto_munj30 = cms.string("nSoftMu(3,30) == 0"),
        bveto_nj05 = cms.string("bTaggedJetsBetween(10,30,2.1,'trackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+") == 0 && nSoftMu(3,1,0.5) == 0"),
        bveto_munj05 = cms.string("nSoftMu(3,1,0.5) == 0"),
        bveto_nj3005 = cms.string("bTaggedJetsBetween(10,30,2.1,'trackCountingHighEffBJetTags',"+jetId_WP+","+DzBVeto+") == 0 && nSoftMu(3,30,0.5) == 0"),
        bveto_munj3005 = cms.string("nSoftMu(3,30,0.5) == 0"),
        dphiveto = cms.string("passesDPhillJet("+DphiJetVetominPt+","+DphiJetVetominEta+",1,"+jetId_WP+")"),
        #pass2012ICHEP1 = cms.string('passCustomByPt(0,"'+Scenario6_ICHEP2012+'","'+Scenario6_ICHEP+'")'),
        #pass2012ICHEP2 = cms.string('passCustomByPt(1,"'+Scenario6_ICHEP2012+'","'+Scenario6_ICHEP+'")'),
        #pass2012ICHEP3 = cms.string('passCustomByPt(2,"'+Scenario6_ICHEP2012+'","'+Scenario6_ICHEP+'")'),
        #pass2012ICHEP4 = cms.string('passCustomByPt(3,"'+Scenario6_ICHEP2012+'","'+Scenario6_ICHEP+'")'),

# passBDT1 = cms.string('passCustomByPt(0,"'+Scenario2_KINK_MUONS+'","'+Scenario4_BDT_ELECTRONS+'")'),
# passBDT2 = cms.string('passCustomByPt(1,"'+Scenario2_KINK_MUONS+'","'+Scenario4_BDT_ELECTRONS+'")'),
# passLH1 = cms.string('passCustomByPt(0,"'+Scenario2_KINK_MUONS+'","'+Scenario3_LH_ELECTRONS+ '")'),
# passLH2 = cms.string('passCustomByPt(1,"'+Scenario2_KINK_MUONS+'","'+Scenario3_LH_ELECTRONS+ '")'),
# passCB1 = cms.string('passCustomByPt(0,"'+Scenario2_KINK_MUONS+'","'+Scenario2_LP_ELECTRONS+ '")'),
# passCB2 = cms.string('passCustomByPt(1,"'+Scenario2_KINK_MUONS+'","'+Scenario2_LP_ELECTRONS+ '")'),
# passCBOld1 = cms.string('passCustomByPt(0,"'+Scenario1_LP_MUONS +'","'+Scenario1_LP_ELECTRONS+ '")'),
# passCBOld2 = cms.string('passCustomByPt(1,"'+Scenario1_LP_MUONS +'","'+Scenario1_LP_ELECTRONS+ '")'),
# WRONG FOR 2012 SELECTION
# passWW = cms.string("guillelmoTrigger('DATASET') && pfMet > 20 && mll()>20 && (abs(mll-91.1876)>15 || hypo == 4 || hypo == 5) && min(projPfMet,projChargedMetSmurf) && nCentralJets(30,4.7) && (passesDPhillJet||!sameflav) && bTaggedJetsBetween(10,30,2.1,'trackCountingHighEffBJetTags',2.0) == 0 && nSoftMu(3) == 0 && nExtraLep(10)"),

    ),
    addRunLumiInfo = cms.bool(True)
)



