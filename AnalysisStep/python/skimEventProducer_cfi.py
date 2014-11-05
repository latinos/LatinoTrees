import FWCore.ParameterSet.Config as cms

#############################################
## Default call targetting MINIAOD objects ##
#############################################

skimEventProducer = cms.EDProducer('SkimEventProducer',

    mcLHEEventInfoTag = cms.InputTag(""),
    mcGenEventInfoTag = cms.InputTag("generator"),
    mcGenWeightTag    = cms.InputTag(""),
    genParticlesTag   = cms.InputTag("genParticles"),
    genMetTag         = cms.InputTag("genMetTrue"),
    genJetTag         = cms.InputTag("ak5GenJets"),

    muTag         = cms.InputTag("selectedPatMuons"),  
    softMuTag     = cms.InputTag("selectedPatMuons"),  ## required to be softMuons

    elTag         = cms.InputTag("selectedPatElectrons"),  ## electrons from miniAOD

    jetTag         = cms.InputTag("selectedPatJets"), 
    jetPuppiTag    = cms.InputTag(""), ## fixed by addEventHypothesis
    tagJetTag      = cms.InputTag("selectedPatJets"), 
    tagJetPuppiTag = cms.InputTag(""), ## fixed by addEventHypothesis

    fatJetTag     = cms.InputTag(""), 
    pfMetTag      = cms.InputTag("patMETs"), 
    pfPuppiMetTag = cms.InputTag(""), ## fixed by addEventHypothesis

    vtxTag     = cms.InputTag("offlinePrimaryVertices"), 
    pfCandsTag = cms.InputTag("particleFlow"), 

    sptTag  = cms.InputTag("vertexMapProd","sumPt"),
    spt2Tag = cms.InputTag("vertexMapProd","sumPt2"),
    rhoTag  = cms.InputTag("fixedGridRhoFastjetAll"), # miniAOD

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
        "190456-999999:HLT_IsoMu24_eta2p1_v*",
    ),
    doubleMuDataPaths = cms.vstring(
        "1-165208:HLT_DoubleMu7_v*",
        "165364-178419:HLT_Mu13_Mu8_v*",
        "178420-180252:HLT_Mu17_Mu8_v*",
        "178420-180252:HLT_Mu17_TkMu8_v*",
        "190456-999999:HLT_Mu17_Mu8_v*",
        "190456-999999:HLT_Mu17_TkMu8_v*",
    ),
    doubleElDataPaths = cms.vstring(
        "1-170901:HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v*",
        "171050-180252:HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
        "190456-999999:HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
    ),
    muEGDataPaths = cms.vstring(
        "1-175972:HLT_Mu17_Ele8_CaloIdL_v*",
        "175973-180252:HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v*",
        "1-167913:HLT_Mu8_Ele17_CaloIdL_v*",
        "167914-180252:HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v*",
        "190456-999999:HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
        "190456-999999:HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
    ),
    singleElDataPaths = cms.vstring(
        "1-164237:HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v*",
        "165085-166967:HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v*",
        "166968-170901:HLT_Ele52_CaloIdVT_TrkIdT_v*",
        "170902-178419:HLT_Ele65_CaloIdVT_TrkIdT_v*",
        "178420-180252:HLT_Ele80_CaloIdVT_TrkIdT_v*",
        "190456-999999:HLT_Ele27_WP80_v*",
    ),
    AllEmbedPaths = cms.vstring(
        "1-999999:HLT_*",
    ),

    singleMuMCPaths = cms.vstring("*"),
    singleElMCPaths = cms.vstring("*"),
    doubleMuMCPaths = cms.vstring("*"),
    doubleElMCPaths = cms.vstring("*"),
    muEGMCPaths     = cms.vstring("*"),
)

### clone the SkimEventProducer changing input collections

def addEventHypothesis(process,skimEventProducer,label,thisMuTag,thisEleTag,thisSoftMuTag,
                       puppiJets,puppiMET,preSequence=cms.Sequence(),isMiniAODProduction=False):

    # isMiniAODProduction is a flag that when is true takes into account that latino tree are run in the same python of miniAOD production, so no cms.Path have
    # top be defined

    if isMiniAODProduction == False:
       process.peakingFilter = cms.EDFilter("GenFilterDiBosons")

    tempSkimEventFilter = cms.EDFilter("SkimEventSelector",
       src = cms.InputTag(""),
       filter = cms.bool(True),
       cut = cms.string("1"),
       #cut = cms.string("nLep >=2 "),
    )

    #create the only hypothesis (>= 1 lepton):
    setattr(process,'ww'+label,skimEventProducer.clone(muTag          = thisMuTag, 
                                                       elTag          = thisEleTag, 
                                                       softMuTag      = thisSoftMuTag,
                                                       jetPuppiTag    = puppiJets, 
                                                       tagJetPuppiTag = puppiJets, 
                                                       pfPuppiMetTag  = puppiMET))

    #create SkimEventSelectors (asking for nLep >=2)
    setattr(process,'skim'+label,tempSkimEventFilter.clone(src='ww'+label))

    p = cms.Sequence(
        preSequence+
        getattr(process,'ww'+label) +
        getattr(process,'skim'+label)
     )

    setattr(process,'sel'+label,p)
    # add to scheduler
    if getattr(process,'schedule') != None: process.schedule.append( getattr(process,'sel'+label) )
    # add to pooloutput module
    if hasattr(process,'out'): process.out.outputCommands.append( 'keep *_{0}_*_*'.format( 'ww'+label ) )
    if hasattr(process,'out'): process.out.SelectEvents.SelectEvents.append( 'sel'+label )






