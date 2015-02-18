import FWCore.ParameterSet.Config as cms

from LatinoTrees.AnalysisStep.wwElectrons_cfi import *
from LatinoTrees.AnalysisStep.wwMuons_cfi import *



skimEventProducer = cms.EDProducer('SkimEventProducer',
    mcLHEEventInfoTag = cms.InputTag(""),
    mcGenEventInfoTag = cms.InputTag(""),
    mcGenWeightTag    = cms.InputTag(""),
    genParticlesTag   = cms.InputTag(""),
    genMetTag         = cms.InputTag(""),
    genJetTag         = cms.InputTag(""),
    phoTag = cms.InputTag("slimmedPhotons"), #miniAOD Photon
    muTag     = cms.InputTag("slimmedMuons"), # miniAOD
    elTag     = cms.InputTag("slimmedElectrons"), # miniAOD
    softMuTag = cms.InputTag("slimmedMuons"), #miniAOD wwMuons4Veto
    # extraElTag = cms.InputTag("wwElectrons"),
    jetTag = cms.InputTag("slimmedJets"), # miniAOD slimPatJetsTriggerMatch
    tagJetTag = cms.InputTag("slimmedJets"), # miniAOD slimPatJetsTriggerMatch
    secondJetTag = cms.InputTag("slimmedJets"), # miniAOD  slimPatFatJetsTriggerMatch -> alternative jet collection
    fatJetTag = cms.InputTag("slimmedJets"), # miniAOD  slimPatFatJetsTriggerMatch
    pfMetTag = cms.InputTag("slimmedMETs"), # miniAOD pfMet
    pupMetTag = cms.InputTag(""), #  puppiMet from puppi objects
    tcMetTag = cms.InputTag("slimmedMETs"), # miniAOD tcMet
    chargedMetTag = cms.InputTag("slimmedMETs"), # miniAOD trackMetProducer
    vtxTag = cms.InputTag("offlineSlimmedPrimaryVertices"), # miniAOD goodPrimaryVertices
### allCandsTag = cms.InputTag("allReducedPFCands"), ### Needed for MVAMet
    chCandsTag = cms.InputTag("packedPFCandidates"), # miniAOD reducedPFCands
    pfCandsTag = cms.InputTag("packedPFCandidates"), # miniAOD reducedPFCands
    sptTag = cms.InputTag("vertexMapProd","sumPt"),
    spt2Tag = cms.InputTag("vertexMapProd","sumPt2"),
    rhoTag = cms.InputTag("fixedGridRhoFastjetAll"), # miniAOD
    # branchAlias = cms.string("wwelmu"),
    #hypoType = cms.string("WWELMU"),
    
    triggerTag = cms.InputTag("TriggerResults","","HLT"),
    singleMuDataPaths = cms.vstring(
        "1-163261:HLT_Mu15_v*",
        "163262-165099:HLT_Mu24_v*",
        "165102-173235:HLT_Mu30_v*",
        "173236-175972:HLT_Mu40_v*",
        "175973-180252:HLT_Mu40_eta2p1_v*",
        "163262-170901:HLT_IsoMu17_v*",
        "171050-175910:HLT_IsoMu20_v*",
        "175911-175921:HLT_IsoMu24_v*",
        "175922-180252:HLT_IsoMu24_eta2p1_v*",
# end of 2011 Data
"190456-999999:HLT_IsoMu24_eta2p1_v*",
    ),
    doubleMuDataPaths = cms.vstring(
        "1-165208:HLT_DoubleMu7_v*",
        "165364-178419:HLT_Mu13_Mu8_v*",
        "178420-180252:HLT_Mu17_Mu8_v*",
        "178420-180252:HLT_Mu17_TkMu8_v*",
# end of 2011 Data
"190456-999999:HLT_Mu17_Mu8_v*",
"190456-999999:HLT_Mu17_TkMu8_v*",
    ),
    doubleElDataPaths = cms.vstring(
        "1-170901:HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v*",
        "171050-180252:HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
# end of 2011 Data
"190456-999999:HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
    ),
    muEGDataPaths = cms.vstring(
        "1-175972:HLT_Mu17_Ele8_CaloIdL_v*",
        "175973-180252:HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v*",
        "1-167913:HLT_Mu8_Ele17_CaloIdL_v*",
        "167914-180252:HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v*",
# end of 2011 Data
        "190456-999999:HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
"190456-999999:HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
    ),
    singleElDataPaths = cms.vstring(
        "1-164237:HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v*",
        "165085-166967:HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v*",
        "166968-170901:HLT_Ele52_CaloIdVT_TrkIdT_v*",
        "170902-178419:HLT_Ele65_CaloIdVT_TrkIdT_v*",
        "178420-180252:HLT_Ele80_CaloIdVT_TrkIdT_v*",
# end of 2011 Data
"190456-999999:HLT_Ele27_WP80_v*",
    ),
    AllEmbedPaths = cms.vstring(
        "1-999999:HLT_*",
    ),

    singleMuMCPaths = cms.vstring("*"),
    singleElMCPaths = cms.vstring("*"),
    doubleMuMCPaths = cms.vstring("*"),
    doubleElMCPaths = cms.vstring("*"),
    muEGMCPaths = cms.vstring("*"),
    # singleMuMCPaths = cms.vstring("HLT_Mu21_v*"),
    # singleElMCPaths = cms.vstring("FILLME"),
    # doubleMuMCPaths = cms.vstring("HLT_DoubleMu5_v*"),
    # doubleElMCPaths = cms.vstring("HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v*"),
    # muEGMCPaths = cms.vstring("HLT_Mu5_Ele17_v*","HLT_Mu11_Ele8_v*"),

    #looseMuSelection = cms.string(PRESEL_MU +"&&"+ MUON_ID_LOOSE),
    #tightMuSelection = cms.string(PRESEL_MU +"&&"+ MUON_ID_CUT +"&&"+ MUON_MERGE_ISO+"&&"+MUON_MERGE_IP),
    #looseEleSelection = cms.string(ELE_BASE + " && " + ELE_ID_LOOSE_2011),
    #tightEleSelection = cms.string(ELE_BASE + " && " + ELE_MERGE_ID + " && " + ELE_MERGE_ISO + " && " + ELE_MERGE_CONV + " && " + ELE_MERGE_IP),
)





def addEventHypothesis(process,label,thisMuTag,thisEleTag,thisSoftMuTag='wwMuons4Veto',thisPhotonTag='slimmedPhotons',preSequence=cms.Sequence(),isMiniAODProduction=False):

    # isMiniAODProduction is a flag that
    # when it is true takes into account that latino tree are run in the same python of miniAOD production, so no cms.Path have to be defined, but only sequences to preserve order of execution
    # when it is false it assumes the miniAOD have already been produced

    # prepare objects:
    # - electrons
    # - muons

    preSequence += wwElectronSequence
    preSequence += wwMuonSequence


    tempSkimEventFilter = cms.EDFilter("SkimEventSelector",
       src = cms.InputTag(""),
       filter = cms.bool(True),
       cut = cms.string("1"),
       #cut = cms.string("nLep >=2 "),
    )

    #create the only hypothesis (>= 1 lepton):
    setattr(process,'ww'+label,process.skimEventProducer.clone( muTag=thisMuTag ,elTag=thisEleTag, softMuTag=thisSoftMuTag, phoTag=thisPhotonTag ))

    #create SkimEventSelectors (asking for nLep >=2)
    setattr(process,'skim'+label,tempSkimEventFilter.clone(src='ww'+label))
    # create sequence
    if not isMiniAODProduction :
     p = cms.Path(
        preSequence+
        getattr(process,'ww'+label) +
        getattr(process,'skim'+label)
     )
    else :
     p = cms.Sequence(
        preSequence+
        getattr(process,'ww'+label) +
        getattr(process,'skim'+label)
     )

    setattr(process,'sel'+label,p)  # --> process."'sel'+label" = p
    # add to scheduler
    if getattr(process,'schedule') != None: process.schedule.append( getattr(process,'sel'+label) )
    # add to pooloutput module
    if hasattr(process,'out'): process.out.outputCommands.append( 'keep *_{0}_*_*'.format( 'ww'+label ) )
    if hasattr(process,'out'): process.out.SelectEvents.SelectEvents.append( 'sel'+label )






