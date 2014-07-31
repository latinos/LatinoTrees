import FWCore.ParameterSet.Config as cms

#from LatinoTrees.AnalysisStep.wwElectrons_cfi import *
#from LatinoTrees.AnalysisStep.wwMuons_cfi import *



skimEventProducer = cms.EDProducer('SkimEventProducer',
    mcLHEEventInfoTag = cms.InputTag(""),
    mcGenEventInfoTag = cms.InputTag(""),
    mcGenWeightTag = cms.InputTag(""),
    genParticlesTag = cms.InputTag(""),
    genMetTag = cms.InputTag(""),
    genJetTag = cms.InputTag(""),
    muTag = cms.InputTag("slimmedMuons"), # miniAOD
    elTag = cms.InputTag("slimmedElectrons"), # miniAOD
    softMuTag = cms.InputTag("slimmedMuons"), #miniAOD wwMuons4Veto
    # extraElTag = cms.InputTag("wwElectrons"),
    jetTag = cms.InputTag("slimmedJets"), # miniAOD slimPatJetsTriggerMatch
    tagJetTag = cms.InputTag("slimmedJets"), # miniAOD slimPatJetsTriggerMatch
    fatJetTag = cms.InputTag("slimmedJets"), # miniAOD  slimPatFatJetsTriggerMatch
    pfMetTag = cms.InputTag("slimmedMETs"), # miniAOD pfMet
    tcMetTag = cms.InputTag("slimmedMETs"), # miniAOD tcMet
    chargedMetTag = cms.InputTag("slimmedMETs"), # miniAOD trackMetProducer
    vtxTag = cms.InputTag("offlineSlimmedPrimaryVertices"), # miniAOD goodPrimaryVertices
### allCandsTag = cms.InputTag("allReducedPFCands"), ### Needed for MVAMet
    chCandsTag = cms.InputTag("packedPFCandidates"), # miniAOD reducedPFCands
    sptTag = cms.InputTag("vertexMapProd","sumPt"),
    spt2Tag = cms.InputTag("vertexMapProd","sumPt2"),
    rhoTag = cms.InputTag("fixedGridRhoFastjetAll"), # miniAOD
    # branchAlias = cms.string("wwelmu"),
    hypoType = cms.string("WWELMU"),
    
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

def addEventHypothesis(process,label,thisMuTag,thisEleTag,thisSoftMuTag='wwMuons4Veto',preSequence=cms.Sequence()):
    hypos = ['mumu','muel','elmu','elel']
    process.peakingFilter = cms.EDFilter("GenFilterDiBosons")

    tempSkimEventFilter = cms.EDFilter("SkimEventSelector",
       src = cms.InputTag(""),
       filter = cms.bool(True),
       cut = cms.string("1"),
       #cut = cms.string("nLep >=2 "),
    )

    for hypo in hypos:
        #create the four hypothesis:
        setattr(process,'ww'+hypo+label,process.skimEventProducer.clone(hypoType='WW'+hypo.upper(),muTag=thisMuTag,elTag=thisEleTag,softMuTag=thisSoftMuTag))
        #create SkimEventSelectors (asking for nLep >=2)
        setattr(process,'skim'+hypo+label,tempSkimEventFilter.clone(src='ww'+hypo+label))
        # create sequence
# p = cms.Path(preSequence)
# if peakingType == 'peaking': p = cms.Path( process.peakingFilter)
# if peakingType == 'non-peaking': p = cms.Path(~process.peakingFilter)
        p = cms.Path(
            #getattr(process,thisMuTag) +
            #getattr(process,thisEleTag) +
            #getattr(process,thisSoftMuTag) +
            getattr(process,'ww'+hypo+label) +
            getattr(process,'skim'+hypo+label)
        )
        setattr(process,'sel'+hypo+label,p)
        # add to scheduler
        if getattr(process,'schedule') != None: process.schedule.append( getattr(process,'sel'+hypo+label) )
        # add to pooloutput module
        if hasattr(process,'out'): process.out.outputCommands.append( 'keep *_{0}_*_*'.format( 'ww'+hypo+label ) )
        if hasattr(process,'out'): process.out.SelectEvents.SelectEvents.append( 'sel'+hypo+label )

    #bestll = cms.EDProducer("SkimEventBest2L2vProducer",
                                    #mumuEvents=cms.InputTag('wwmumu%s' % label),
                                    #elelEvents=cms.InputTag('wwelel%s' % label),
                                    #elmuEvents=cms.InputTag('wwelmu%s' % label),
                                    #muelEvents=cms.InputTag('wwmuel%s' % label),
                                   #)
    #setattr( process,'wwellell%s' % label,bestll )
    ## add path for best
    #setattr( process, 'selellell%s' % label, cms.Path(
        #getattr(process, 'wwellell%s' % label)
    #))









# process.ttLeps = cms.EDProducer("CandViewMerger",
# src = cms.VInputTag(
# cms.InputTag("wwMuonsMergeIP"),
# cms.InputTag("wwEleIPMerge"),
# )
# )
#
# process.ttDiLeps = cms.EDProducer("CandViewShallowCloneCombiner",
# decay = cms.string('ttLeps@+ ttLeps@-'),
# cut = cms.string(
# 'deltaR(daughter(0).eta,daughter(0).phi,daughter(1).eta,daughter(1).phi) > 0.05 && ' +
# 'min(daughter(0).pt,daughter(1).pt) > 10 && ' +
# 'max(daughter(0).pt,daughter(1).pt) > 20'
# ),
# checkCharge = cms.bool(True)
# )
#
# process.ttCount = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("ttDiLeps"), minNumber = cms.uint32(1))
#
#
# process.ttFilter = cms.Sequence( process.wwEleIPMerge + process.wwMuonsMergeIP + process.ttLeps + process.ttDiLeps + process.ttCount)
# process.ttPath = cms.Path( process.ttFilter )
# process.schedule.append( process.ttPath )
# process.out.SelectEvents.SelectEvents.append( 'ttPath' )