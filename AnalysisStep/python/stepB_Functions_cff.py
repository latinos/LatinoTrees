import FWCore.ParameterSet.Config as cms
import os
#from WWAnalysis.AnalysisStep.wwMuons_cfi import Scenario6_ICHEP2012,Scenario2_KINK_MUONS,Scenario1_LP_MUONS
#from WWAnalysis.AnalysisStep.wwElectrons_cfi import Scenario6_ICHEP,Scenario4_BDT_ELECTRONS,Scenario3_LH_ELECTRONS,Scenario2_LP_ELECTRONS,Scenario1_LP_ELECTRONS

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

####################################
# variables defined in stepB_cff.py
####################################

#jetId_WP="4" ----> FIXME: to be used!
#jetId_WP="0"

## JetCuts

#CJVminPt="30."
#CJVmaxEta="4.7"
#DphiJetVetominPt="15."
#DphiJetVetominEta="4.7"

#DzBVeto="999999.9"
#minPtBVeto="10.0"


def addMuonIdIsoVariables(process,pt):
    if hasattr(pt,"variables"):      
        setattr(pt.variables, "std_vector_lepton_NValidHitsInTrk",      cms.string("muNValidHitsInTrkByPt")),
        setattr(pt.variables, "std_vector_lepton_NValidFractInTrk",     cms.string("muNValidFractInTrkByPt")),
        setattr(pt.variables, "std_vector_lepton_NormChi2GTrk",         cms.string("muNormChi2GTrkByPt")),
        setattr(pt.variables, "std_vector_lepton_NValidHitsSATrk",      cms.string("muNValidHitsSATrkByPt")),
        setattr(pt.variables, "std_vector_lepton_NumOfMatchedStations", cms.string("muNumOfMatchedStationsByPt")),
        setattr(pt.variables, "std_vector_lepton_BestTrackdz",          cms.string("muBestTrackdzByPt")),
        setattr(pt.variables, "std_vector_lepton_BestTrackdxy",         cms.string("muBestTrackdxyByPt")),
        setattr(pt.variables, "std_vector_lepton_NValidPixelHitsInTrk", cms.string("muNValidPixelHitsInTrkByPt")),
        setattr(pt.variables, "std_vector_lepton_NTkLayers",            cms.string("muNTkLayersByPt")),
        setattr(pt.variables, "std_vector_lepton_TrkKink",              cms.string("muTrkKinkByPt")),
        setattr(pt.variables, "std_vector_lepton_Chi2LocalPos",         cms.string("muChi2LocalPosByPt")), 
        setattr(pt.variables, "std_vector_lepton_SegCompatibilty",      cms.string("muSegCompatibiltyByPt")),
        setattr(pt.variables, "std_vector_lepton_chargedHadronIso",     cms.string("chargedHadronIsoByPt")),
        setattr(pt.variables, "std_vector_lepton_chargedParticleIso",   cms.string("chargedParticleIsoByPt")),
        setattr(pt.variables, "std_vector_lepton_neutralHadronIso",     cms.string("neutralHadronIsoByPt")),
        setattr(pt.variables, "std_vector_lepton_photonIso",            cms.string("photonIsoByPt")),
        setattr(pt.variables, "std_vector_lepton_sumPUPt",              cms.string("sumPUPtByPt")),
 
    else:
        raise RuntimeError, "In addMuonIdIsoVariables, %s doesn't look like a ProbeTreeProducer object, it has no 'variables' attribute." % pt


def addEleIdIsoVariables(process,pt):
    if hasattr(pt,"variables"):      
        setattr(pt.variables, "std_vector_deltaEtaIn" ,   cms.string("deltaEtaSuperClusterTrackAtVtxByPt")),
        setattr(pt.variables, "std_vector_deltaPhiIn" ,   cms.string("deltaPhiSuperClusterTrackAtVtxByPt")),
        setattr(pt.variables, "std_vector_sigmaIetaIeta", cms.string("sigmaIetaIetaByPt")),
        setattr(pt.variables, "std_vector_HoE" ,          cms.string("hadronicOverEmByPt")),
        setattr(pt.variables, "std_vector_numHits",       cms.string("numberOfHitsByPt")),
    else:
        raise RuntimeError, "In addEleIdIsoVariables, %s doesn't look like a ProbeTreeProducer object, it has no 'variables' attribute." % pt


def addBTaggingVariables(pt,dzCut=99999):
    if hasattr(pt,"variables"):

        pt.variables.softcsvv2ivf = cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'combinedInclusiveSecondaryVertexV2BJetTags',"+jetId_WP+",%f)"%dzCut)
        pt.variables.hardcsvv2ivf = cms.string("highestBDiscRange("+CJVminPt+",999999.,'combinedInclusiveSecondaryVertexV2BJetTags',"+jetId_WP+",%f,1)"%dzCut)
        pt.variables.softssvhe = cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'simpleSecondaryVertexHighEffBJetTags',"+jetId_WP+",%f)"%dzCut)
        pt.variables.hardssvhe = cms.string("highestBDiscRange("+CJVminPt+",999999.,'simpleSecondaryVertexHighEffBJetTags',"+jetId_WP+",%f,1)"%dzCut)
        pt.variables.softssvhb = cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'simpleSecondaryVertexHighPurBJetTags',"+jetId_WP+",%f)"%dzCut)
        pt.variables.hardssvhb = cms.string("highestBDiscRange("+CJVminPt+",999999.,'simpleSecondaryVertexHighPurBJetTags',"+jetId_WP+",%f,1)"%dzCut)
        pt.variables.softpfcsv = cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'pfCombinedSecondaryVertexBJetTags',"+jetId_WP+",%f)"%dzCut)
        pt.variables.hardpfcsv = cms.string("highestBDiscRange("+CJVminPt+",999999.,'simpleSecondaryVertexHighEffBJetTags',"+jetId_WP+",%f,1)"%dzCut)
        pt.variables.softcmva = cms.string("highestBDiscRange("+minPtBVeto+","+CJVminPt+" ,'combinedMVABJetTags',"+jetId_WP+",%f)"%dzCut)
        pt.variables.hardcmva = cms.string("highestBDiscRange("+CJVminPt+",999999.,'combinedMVABJetTags',"+jetId_WP+",%f,1)"%dzCut)

        pt.variables.std_vector_jet_csvv2ivf = cms.string("jetcsvv2ivfByPt")
        pt.variables.std_vector_jet_ssvhe = cms.string("jetssvheByPt")
        pt.variables.std_vector_jet_ssvhb = cms.string("jetssvhbByPt")
        pt.variables.std_vector_jet_pfcsv = cms.string("jetpfcsvByPt")
        pt.variables.std_vector_jet_cmva = cms.string("jetcmvaByPt")
        pt.variables.std_vector_jet_tche = cms.string("jettcheByPt")
        pt.variables.std_vector_jet_tchp = cms.string("jettchpByPt")
        pt.variables.std_vector_jet_bjpb = cms.string("jetbjpbByPt")

        pt.variables.jetcsvv2ivf1 = cms.string("leadingJetBtag(0,'combinedInclusiveSecondaryVertexV2BJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)
        pt.variables.jetcsvv2ivf2 = cms.string("leadingJetBtag(1,'combinedInclusiveSecondaryVertexV2BJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)
        pt.variables.jetssvhe1 = cms.string("leadingJetBtag(0,'simpleSecondaryVertexHighEffBJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)
        pt.variables.jetssvhe2 = cms.string("leadingJetBtag(1,'simpleSecondaryVertexHighEffBJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)
        pt.variables.jetssvhb1 = cms.string("leadingJetBtag(0,'simpleSecondaryVertexHighPurBJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)
        pt.variables.jetssvhb2 = cms.string("leadingJetBtag(1,'simpleSecondaryVertexHighPurBJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)
        pt.variables.jetpfcsv1 = cms.string("leadingJetBtag(0,'pfCombinedSecondaryVertexBJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)
        pt.variables.jetpfcsv2 = cms.string("leadingJetBtag(1,'pfCombinedSecondaryVertexBJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)
        pt.variables.jetcmva1 = cms.string("leadingJetBtag(0,'combinedMVABJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)
        pt.variables.jetcmva2 = cms.string("leadingJetBtag(1,'combinedMVABJetTags',0,"+CJVmaxEta+",1,1,%f)"%dzCut)

    else:
        raise RuntimeError, "In addBTaggingVariables, %s doesn't look like a ProbeTreeProducer object, it has no 'variables' attribute." % pt

# LHE information
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
        setattr(pt.variables, "higgsLHEeta" , cms.string("higgsLHEEta()")),
        setattr(pt.variables, "higgsLHEphi" , cms.string("higgsLHEPhi()")),
        setattr(pt.variables, "higgsLHEm" , cms.string("higgsLHEmass()")),

    else:
        raise addLHEVariables, "In addLHEVariables, %s doesn't look like a ProbeTreeProducer object, it has no 'variables' attribute." % pt

# gen information
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

        setattr(pt.variables, "std_vector_jetGen_pt"  , cms.string("leadingGenJetPt")),
        setattr(pt.variables, "std_vector_jetGen_phi" , cms.string("leadingGenJetPhi")),
        setattr(pt.variables, "std_vector_jetGen_eta" , cms.string("leadingGenJetEta")),

        setattr(pt.variables, "std_vector_jet_HadronFlavour" , cms.string("leadingJetHadronFlavour")),
        setattr(pt.variables, "std_vector_jet_PartonFlavour" , cms.string("leadingJetPartonFlavour")),


        #setattr(pt.variables, "jetGenpt1" , cms.string("leadingGenJetPt(0)")),
        #setattr(pt.variables, "jetGenphi1" , cms.string("leadingGenJetPhi(0)")),
        #setattr(pt.variables, "jetGeneta1" , cms.string("leadingGenJetEta(0)")),
        #setattr(pt.variables, "jetGenpt2" , cms.string("leadingGenJetPt(1)")),
        #setattr(pt.variables, "jetGenphi2" , cms.string("leadingGenJetPhi(1)")),
        #setattr(pt.variables, "jetGeneta2" , cms.string("leadingGenJetEta(1)")),
        #setattr(pt.variables, "jetGenpt3" , cms.string("leadingGenJetPt(2)")),
        #setattr(pt.variables, "jetGenphi3" , cms.string("leadingGenJetPhi(2)")),
        #setattr(pt.variables, "jetGeneta3" , cms.string("leadingGenJetEta(2)")),


    else:
        raise addGenVariables, "In addGenVariables, %s doesn't look like a ProbeTreeProducer object, it has no 'variables' attribute." % pt



def addTau(process,pt):

        if hasattr(pt,"variables"):
          pt.variables.std_vector_tau_pt  = cms.string("leadingTauPt")
          pt.variables.std_vector_tau_eta = cms.string("leadingTauEta")
          pt.variables.std_vector_tau_phi = cms.string("leadingTauPhi")


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





def addAdditionalJets(process,pt):

        if hasattr(pt,"variables"):

            setattr(pt.variables, "jetpt5", cms.string("leadingJetPt(4,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jeteta5", cms.string("leadingJetEta(4,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetphi5", cms.string("leadingJetPhi(4,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetmass5", cms.string("leadingJetMass(4,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetid5", cms.string("leadingJetId(4,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetmva5", cms.string("leadingJetMva(4,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jettche5", cms.string("leadingJetBtag(4,'trackCountingHighEffBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")")),
            setattr(pt.variables, "jetbjpb5", cms.string("leadingJetBtag(4,'jetBProbabilityBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")")),
            setattr(pt.variables, "jettchp5", cms.string("leadingJetBtag(4,'trackCountingHighPurBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")")),
            setattr(pt.variables, "jetptd5", cms.string("leadingJetPtd(4,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetCHM5", cms.string("leadingJetChargedHadronMultiplicity(4,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetNHM5", cms.string("leadingJetNeutralHadronMultiplicity(4,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetPhM5", cms.string("leadingJetPhotonMultiplicity(4,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetQCptD5", cms.string("leadingJetPtD(4,0,"+CJVmaxEta+",1,"+jetId_WP+",1)")),
            setattr(pt.variables, "jetQCaxis15", cms.string("leadingJetQGaxis1(4,0,"+CJVmaxEta+",1,"+jetId_WP+",1)")),
            setattr(pt.variables, "jetQCaxis25", cms.string("leadingJetQGaxis2(4,0,"+CJVmaxEta+",1,"+jetId_WP+",1)")),
            setattr(pt.variables, "jetQCRMScand5", cms.string("leadingJetQGRMScand(4,0,"+CJVmaxEta+",1,"+jetId_WP+",1)")),
            setattr(pt.variables, "jetQCRmax5", cms.string("leadingJetQGRmax(4,0,"+CJVmaxEta+",1,"+jetId_WP+",1)")),
            setattr(pt.variables, "jetptD5", cms.string("leadingJetPtD(4,0,"+CJVmaxEta+",1,"+jetId_WP+",0)")),
            setattr(pt.variables, "jetaxis15", cms.string("leadingJetQGaxis1(4,0,"+CJVmaxEta+",1,"+jetId_WP+",0)")),
            setattr(pt.variables, "jetaxis25", cms.string("leadingJetQGaxis2(4,0,"+CJVmaxEta+",1,"+jetId_WP+",0)")),
            setattr(pt.variables, "jetRMScand5", cms.string("leadingJetQGRMScand(4,0,"+CJVmaxEta+",1,"+jetId_WP+",0)")),
            setattr(pt.variables, "jetRmax5", cms.string("leadingJetQGRmax(4,0,"+CJVmaxEta+",1,"+jetId_WP+",0)")),
            setattr(pt.variables, "jetNChgQC5", cms.string("leadingJetNChgQC(4,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetNChgptCut5", cms.string("leadingJetNChgptCut(4,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetNNeutralptCut5", cms.string("leadingJetNNeutralptCut(4,0,"+CJVmaxEta+",1,"+jetId_WP+")")),

            setattr(pt.variables, "jetpt6", cms.string("leadingJetPt(5,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jeteta6", cms.string("leadingJetEta(5,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetphi6", cms.string("leadingJetPhi(5,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetmass6", cms.string("leadingJetMass(5,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetid6", cms.string("leadingJetId(5,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetmva6", cms.string("leadingJetMva(5,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jettche6", cms.string("leadingJetBtag(5,'trackCountingHighEffBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")")),
            setattr(pt.variables, "jetbjpb6", cms.string("leadingJetBtag(5,'jetBProbabilityBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")")),
            setattr(pt.variables, "jettchp6", cms.string("leadingJetBtag(5,'trackCountingHighPurBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")")),
            setattr(pt.variables, "jetptd6", cms.string("leadingJetPtd(5,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetCHM6", cms.string("leadingJetChargedHadronMultiplicity(5,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetNHM6", cms.string("leadingJetNeutralHadronMultiplicity(5,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetPhM6", cms.string("leadingJetPhotonMultiplicity(5,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetQCptD6", cms.string("leadingJetPtD(5,0,"+CJVmaxEta+",1,"+jetId_WP+",1)")),
            setattr(pt.variables, "jetQCaxis16", cms.string("leadingJetQGaxis1(5,0,"+CJVmaxEta+",1,"+jetId_WP+",1)")),
            setattr(pt.variables, "jetQCaxis26", cms.string("leadingJetQGaxis2(5,0,"+CJVmaxEta+",1,"+jetId_WP+",1)")),
            setattr(pt.variables, "jetQCRMScand6", cms.string("leadingJetQGRMScand(5,0,"+CJVmaxEta+",1,"+jetId_WP+",1)")),
            setattr(pt.variables, "jetQCRmax6", cms.string("leadingJetQGRmax(5,0,"+CJVmaxEta+",1,"+jetId_WP+",1)")),
            setattr(pt.variables, "jetptD6", cms.string("leadingJetPtD(5,0,"+CJVmaxEta+",1,"+jetId_WP+",0)")),
            setattr(pt.variables, "jetaxis16", cms.string("leadingJetQGaxis1(5,0,"+CJVmaxEta+",1,"+jetId_WP+",0)")),
            setattr(pt.variables, "jetaxis26", cms.string("leadingJetQGaxis2(5,0,"+CJVmaxEta+",1,"+jetId_WP+",0)")),
            setattr(pt.variables, "jetRMScand6", cms.string("leadingJetQGRMScand(5,0,"+CJVmaxEta+",1,"+jetId_WP+",0)")),
            setattr(pt.variables, "jetRmax6", cms.string("leadingJetQGRmax(5,0,"+CJVmaxEta+",1,"+jetId_WP+",0)")),
            setattr(pt.variables, "jetNChgQC6", cms.string("leadingJetNChgQC(5,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetNChgptCut6", cms.string("leadingJetNChgptCut(5,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetNNeutralptCut6", cms.string("leadingJetNNeutralptCut(5,0,"+CJVmaxEta+",1,"+jetId_WP+")")),

            setattr(pt.variables, "jetpt7", cms.string("leadingJetPt(6,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jeteta7", cms.string("leadingJetEta(6,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetphi7", cms.string("leadingJetPhi(6,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetmass7", cms.string("leadingJetMass(6,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetid7", cms.string("leadingJetId(6,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetmva7", cms.string("leadingJetMva(6,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jettche7", cms.string("leadingJetBtag(6,'trackCountingHighEffBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")")),
            setattr(pt.variables, "jetbjpb7", cms.string("leadingJetBtag(6,'jetBProbabilityBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")")),
            setattr(pt.variables, "jettchp7", cms.string("leadingJetBtag(6,'trackCountingHighPurBJetTags',0,"+CJVmaxEta+",1,"+jetId_WP+","+DzBVeto+")")),
            setattr(pt.variables, "jetptd7", cms.string("leadingJetPtd(6,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetCHM7", cms.string("leadingJetChargedHadronMultiplicity(6,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetNHM7", cms.string("leadingJetNeutralHadronMultiplicity(6,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetPhM7", cms.string("leadingJetPhotonMultiplicity(6,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetQCptD7", cms.string("leadingJetPtD(6,0,"+CJVmaxEta+",1,"+jetId_WP+",1)")),
            setattr(pt.variables, "jetQCaxis17", cms.string("leadingJetQGaxis1(6,0,"+CJVmaxEta+",1,"+jetId_WP+",1)")),
            setattr(pt.variables, "jetQCaxis27", cms.string("leadingJetQGaxis2(6,0,"+CJVmaxEta+",1,"+jetId_WP+",1)")),
            setattr(pt.variables, "jetQCRMScand7", cms.string("leadingJetQGRMScand(6,0,"+CJVmaxEta+",1,"+jetId_WP+",1)")),
            setattr(pt.variables, "jetQCRmax7", cms.string("leadingJetQGRmax(6,0,"+CJVmaxEta+",1,"+jetId_WP+",1)")),
            setattr(pt.variables, "jetptD7", cms.string("leadingJetPtD(6,0,"+CJVmaxEta+",1,"+jetId_WP+",0)")),
            setattr(pt.variables, "jetaxis17", cms.string("leadingJetQGaxis1(6,0,"+CJVmaxEta+",1,"+jetId_WP+",0)")),
            setattr(pt.variables, "jetaxis27", cms.string("leadingJetQGaxis2(6,0,"+CJVmaxEta+",1,"+jetId_WP+",0)")),
            setattr(pt.variables, "jetRMScand7", cms.string("leadingJetQGRMScand(6,0,"+CJVmaxEta+",1,"+jetId_WP+",0)")),
            setattr(pt.variables, "jetRmax7", cms.string("leadingJetQGRmax(6,0,"+CJVmaxEta+",1,"+jetId_WP+",0)")),
            setattr(pt.variables, "jetNChgQC7", cms.string("leadingJetNChgQC(6,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetNChgptCut7", cms.string("leadingJetNChgptCut(6,0,"+CJVmaxEta+",1,"+jetId_WP+")")),
            setattr(pt.variables, "jetNNeutralptCut7", cms.string("leadingJetNNeutralptCut(6,0,"+CJVmaxEta+",1,"+jetId_WP+")")),




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



########## PHOTON VARIABLES

def addPhotonVariables(process,pt):
    if hasattr(pt,"variables"):
        setattr(pt.variables, "v_photon1" ,    cms.string("photon(0)")),
        setattr(pt.variables, "v_photon2" ,    cms.string("photon(1)")),
        setattr(pt.variables, "v_photon3" ,    cms.string("photon(2)")),
        setattr(pt.variables, "v_photon1id" ,  cms.string("photon_id(0)")),
        setattr(pt.variables, "nPhos" ,        cms.string("nPhos()")),
        setattr(pt.variables, "pho_n_id" ,     cms.string("Pho_n_ID()")),
        setattr(pt.variables, "mllg" ,         cms.string("mllg()")),
        setattr(pt.variables, "mllgid" ,       cms.string("mllg()")),

def addPhotonIDVariables(process,pt):
        setattr(pt.variables, "pho_sietaieta" ,    cms.string("Pho_sigmaIetaIeta(0)")),
        setattr(pt.variables, "pho_HoE" ,          cms.string("Pho_hadronicOverEm(0)")),
        setattr(pt.variables, "pho_chIso" ,        cms.string("Pho_rhoChargedHadronIso(0)")),
        setattr(pt.variables, "pho_nhIso" ,        cms.string("Pho_rhoNeutralHadronIso(0)")),
        setattr(pt.variables, "pho_phIso" ,        cms.string("Pho_rhoPhotonIso(0)")),
        setattr(pt.variables, "pho_passElecVeto" , cms.string("Pho_PassElectronVeto(0)")),
        setattr(pt.variables, "pho_hasPixelSeed",  cms.string("Pho_HasPixelSeed(0)")),



