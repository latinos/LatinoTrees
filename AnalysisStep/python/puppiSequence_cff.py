#import FWCore.ParameterSet.Config as cms

#from Dummy.Puppi.Puppi_cff import puppiCentral, puppiForward, puppi 



######## make clustering in AK4 puppi jets
#from RecoJets.JetProducers.ak5PFJets_cfi  import ak5PFJets

#AK4PFJetsPuppi = ak5PFJets.clone(
    #src      = cms.InputTag('puppi','Puppi'),
    #rParam   = cms.double(0.4),
    #jetPtMin = cms.double(10)
#)

#AK5PFJetsPuppi = ak5PFJets.clone(
    #src      = cms.InputTag('puppi','Puppi'),
    #rParam   = cms.double(0.5),
    #jetPtMin = cms.double(10)
#)

#### puppi met
#from RecoMET.METProducers.PFMET_cfi import pfMet
#pfMetPuppi = pfMet.clone();
#pfMetPuppi.src = cms.InputTag('puppi','Puppi')
#pfMetPuppi.calculateSignificance = False # this can't be easily implemented on packed PF candidates at the moment                                                                  

#### final sequence ###
#puppiSequence = cms.Sequence(puppi*
                             #AK5PFJetsPuppi*
                             #pfMetPuppi)





import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.ak5PFJets_cfi  import ak5PFJets



from CommonTools.PileupAlgos.Puppi_cff import puppi


################################
################################       
################################       

def makePuppiAlgo(process):

  puppi.candName = cms.InputTag('packedPFCandidates')
  puppi.vertexName = cms.InputTag('offlineSlimmedPrimaryVertices')
 
  process.puppi = puppi.clone()

  process.puppi_onMiniAOD = cms.Sequence(process.puppi)


 #### puppi particles
 #process.load('Dummy.Puppi.Puppi_cff')
 #process.puppi.candName = cms.untracked.string('particleFlow')
 #process.puppi.vertexName = cms.untracked.string('offlinePrimaryVertices')

 ### puppi met
 #process.pfMetPuppi = pfMet.clone( src = cms.InputTag('puppi','Puppi'),
                                   #calculateSignificance = False)          

 #process.patMetPuppi = process.patMETs.clone( metSource = cms.InputTag("pfMetPuppi"))
 
 ### final sequence
 #process.puppiSequence = cms.Sequence(process.puppi*
                                      #process.pfMetPuppi*
                                      #process.patMetPuppi)                                                                 

#######################
### make puppi jets ###
#######################


from RecoJets.JetAssociationProducers.ic5PFJetTracksAssociatorAtVertex_cfi import *
from PhysicsTools.PatAlgos.mcMatchLayer0.jetMatch_cfi import patJetPartonMatch
from PhysicsTools.PatAlgos.mcMatchLayer0.jetMatch_cfi import patJetGenJetMatch
from PhysicsTools.PatAlgos.mcMatchLayer0.jetFlavourId_cff import patJetPartonAssociationLegacy
from PhysicsTools.PatAlgos.mcMatchLayer0.jetFlavourId_cff import patJetFlavourAssociation
from PhysicsTools.PatAlgos.recoLayer0.jetTracksCharge_cff import patJetCharge
from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import selectedPatJets
from PhysicsTools.PatAlgos.producersLayer1.jetProducer_cfi import patJets
#from PhysicsTools.PatAlgos.slimming.pileupJetId_cfi import *
from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection

def makePatPuppiJetSequence( process, rParameter = 0.4):
 
  #process.load('RecoJets.JetProducers.ak4PFJetsPuppi_cfi')
  #process.ak4PFJetsPuppi.src = cms.InputTag('puppi')
  
  ####### make clustering in AKxxx puppi jets
  jetRPrefix = str(int(rParameter*10));
  ###########################################################setattr(process,"AK"+jetRPrefix+"PFJetsPuppi", ak5PFJets.clone( 
                                           ###########################################################src      = cms.InputTag('puppi'),
                                           ############################################################src      = cms.InputTag('puppi','Puppi'),
                                           ###########################################################rParam   = cms.double(rParameter),
                                           ###########################################################jetPtMin = cms.double(10))
  ###########################################################)

  ###########################################################addJetCollection(
    ###########################################################process,
    ###########################################################postfix   = "", # Puppi removed
    ###########################################################labelName = "AK"+jetRPrefix+"selectedPatJetsPuppi",  # need puppi otherwise I cannot use jettoolbox later
    ###########################################################jetSource = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi"),
    ###########################################################trackSource = cms.InputTag('unpackedTracksAndVertices'), 
    ###########################################################pfCandidates = cms.InputTag('packedPFCandidates'), 
    ###########################################################pvSource = cms.InputTag('unpackedTracksAndVertices'), 
    ############################################################jetCorrections = ("AK4PF", cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'Type-2'),
    ############################################################jetCorrections = ("AK"+jetRPrefix+"selectedPatJetsPF", cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'Type-2'),
    ############################################################btagDiscriminators = [      'pfCombinedSecondaryVertexBJetTags'     ],
    ###########################################################genJetCollection=cms.InputTag('ak4GenJets')
    ###########################################################)

  ############################################################setattr(process,"patJetGenJetMatchAK"+jetRPrefix+"selectedPatJetsPuppi.matched","slimmedGenJets")
  ############################################################setattr(process,"patJetPartonMatchAK"+jetRPrefix+"selectedPatJetsPuppi.matched","prunedGenParticles")
  ###########################################################process.patJetGenJetMatchAK4selectedPatJetsPuppi.matched = "slimmedGenJets" # FIXME R=0.4
  ###########################################################process.patJetPartonMatchAK4selectedPatJetsPuppi.matched = "prunedGenParticles" # FIXME R=0.4

  ###########################################################process.load("PhysicsTools.PatAlgos.mcMatchLayer0.jetFlavourId_cff")
  ###########################################################process.patJetPartonsPuppi.particles = "prunedGenParticles"
  ###########################################################process.patJetPartonsLegacyPuppi.src = "prunedGenParticles" # if using legacy jet flavour (not used by default)
  ###########################################################process.patJetPartonsLegacyPuppi.skipFirstN = cms.uint32(0) # do not skip first 6 particles, we already pruned some!
  ###########################################################process.patJetPartonsLegacyPuppi.acceptNoDaughters = cms.bool(True) # as we drop intermediate stuff, we need to accept quarks with no siblings
    
  #adjust PV
  #process.patJetCorrFactorsAK4selectedPatJetsPuppi.primaryVertices = "offlineSlimmedPrimaryVertices" # FIXME R=0.4
  # FIXME corrections not supported for PUPPI???
  
  # FIXME not working
  #recreate tracks and pv for btagging
  #process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
  #process.combinedSecondaryVertex.trackMultiplicityMin = 1 #silly sv, uses un filtered tracks.. i.e. any pt



  ######## btagging sequence for puppi jets
  ######## jet track association
  ##setattr(process, "AK"+jetRPrefix+"jetTracksAssociatorAtVertexPuppi", ic5PFJetTracksAssociatorAtVertex.clone( jets = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
  
  ### impact parameter and track counting
  ##setattr(process,"AK"+jetRPrefix+"impactParameterTagInfosPuppi",      impactParameterTagInfos.clone( jetTracks = cms.InputTag("AK"+jetRPrefix+"jetTracksAssociatorAtVertexPuppi")))  
  ##setattr(process,"AK"+jetRPrefix+"trackCountingHighEffbPuppiJetTags", trackCountingHighEffBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi")))) 
  ##setattr(process,"AK"+jetRPrefix+"trackCountingHighPurBPuppiJetTags", trackCountingHighPurBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"))))
  ##setattr(process,"AK"+jetRPrefix+"trackCountingHighPurBPuppiJetTags", jetProbabilityBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"))))
  ##setattr(process,"AK"+jetRPrefix+"jetBProbabilityBPuppiJetTags",      jetBProbabilityBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"))))
  ##setattr(process,"AK"+jetRPrefix+"jetProbabilityBPuppiJetTags",       jetProbabilityBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"))))
  #### secondary vertex tags   
  ##setattr(process,"AK"+jetRPrefix+"secondaryVertexTagInfoPuppi",       secondaryVertexTagInfos.clone( trackIPTagInfos = cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi")))
  ##setattr(process,"AK"+jetRPrefix+"inclusiveSecondaryVertexFinderTagInfosPuppi", inclusiveSecondaryVertexFinderTagInfos.clone( trackIPTagInfos = cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi")))

  ##setattr(process,"AK"+jetRPrefix+"simpleSecondaryVertexHighEffBPuppiJetTags",  simpleSecondaryVertexHighEffBJetTags.clone(tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"secondaryVertexTagInfoPuppi"))))
  ##setattr(process,"AK"+jetRPrefix+"simpleSecondaryVertexHighPurBPuppiJetTags",  simpleSecondaryVertexHighPurBJetTags.clone(tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"secondaryVertexTagInfoPuppi"))))
  ##setattr(process,"AK"+jetRPrefix+"combinedSecondaryVertexBPuppiJetTags",       combinedSecondaryVertexBJetTags.clone(tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"), cms.InputTag("AK"+jetRPrefix+"secondaryVertexTagInfoPuppi"))))
  ##setattr(process,"AK"+jetRPrefix+"combinedSecondaryVertexMVABPuppiJetTags",    combinedSecondaryVertexMVABJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"), cms.InputTag("AK"+jetRPrefix+"secondaryVertexTagInfoPuppi"))))
  
  #### soft leptons
  ##setattr(process,"AK"+jetRPrefix+"ghostTrackVertexTagInfosPuppi",     ghostTrackVertexTagInfos.clone( trackIPTagInfos = cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"))) 
  ##setattr(process,"AK"+jetRPrefix+"ghostTrackBJetTagsPuppi",           ghostTrackBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"), cms.InputTag("AK"+jetRPrefix+"ghostTrackVertexTagInfosPuppi"))))
  
  ##setattr(process,"AK"+jetRPrefix+"softPFMuonsTagInfosPuppi",          softPFMuonsTagInfos.clone( jets = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
  ##setattr(process,"AK"+jetRPrefix+"softPFMuonBJetTagsPuppi",           softPFMuonBJetTags.clone(tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"softPFMuonsTagInfosPuppi"))))
  ##setattr(process,"AK"+jetRPrefix+"softPFElectronsTagInfosPuppi",      softPFElectronsTagInfos.clone(jets = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
  ##setattr(process,"AK"+jetRPrefix+"softPFElectronBJetTagsPuppi",       softPFElectronBJetTags.clone(tagInfos= cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"softPFElectronsTagInfosPuppi"))))
  
  ### final sequence
  ##setattr(process,"AK"+jetRPrefix+"btaggingPuppi", cms.Sequence( getattr(process,"AK"+jetRPrefix+"jetTracksAssociatorAtVertexPuppi")*
                                                                 ##getattr(process,"AK"+jetRPrefix+"impactParameterTagInfosPuppi")*
                                                                 ##getattr(process,"AK"+jetRPrefix+"trackCountingHighEffbPuppiJetTags")*
                                                                 ##getattr(process,"AK"+jetRPrefix+"trackCountingHighPurBPuppiJetTags")*                                   
                                                                 ##getattr(process,"AK"+jetRPrefix+"jetProbabilityBPuppiJetTags")*
                                                                 ##getattr(process,"AK"+jetRPrefix+"jetBProbabilityBPuppiJetTags")*
                                                                 ##getattr(process,"AK"+jetRPrefix+"secondaryVertexTagInfoPuppi")*
                                                                 ##getattr(process,"AK"+jetRPrefix+"simpleSecondaryVertexHighEffBPuppiJetTags")*
                                                                 ##getattr(process,"AK"+jetRPrefix+"simpleSecondaryVertexHighPurBPuppiJetTags")*
                                                                 ##getattr(process,"AK"+jetRPrefix+"combinedSecondaryVertexBPuppiJetTags")*
                                                                 ##getattr(process,"AK"+jetRPrefix+"combinedSecondaryVertexMVABPuppiJetTags")*
                                                                 ##getattr(process,"AK"+jetRPrefix+"ghostTrackVertexTagInfosPuppi")*
                                                                 ##getattr(process,"AK"+jetRPrefix+"ghostTrackBJetTagsPuppi")*
                                                                 ##getattr(process,"AK"+jetRPrefix+"softPFMuonsTagInfosPuppi")*
                                                                 ##getattr(process,"AK"+jetRPrefix+"softPFMuonBJetTagsPuppi")*
                                                                 ##getattr(process,"AK"+jetRPrefix+"softPFElectronsTagInfosPuppi")*
                                                                 ##getattr(process,"AK"+jetRPrefix+"softPFElectronBJetTagsPuppi")
                                                                 ##)
  ##)

  
  ### jet flavout association
  ##setattr(process,"AK"+jetRPrefix+"patJetPuppiPartonMatch",     patJetPartonMatch.clone( src = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
  ##setattr(process,"AK"+jetRPrefix+"patJetPuppiGenJetMatch",     patJetGenJetMatch.clone( src = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
  ##setattr(process,"AK"+jetRPrefix+"patJetPuppiPartonAssociationLegacy",      patJetPartonAssociationLegacy.clone( jets = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
  ##setattr(process,"AK"+jetRPrefix+"patJetPuppiFlavourAssociation",           patJetFlavourAssociation.clone(  jets = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
  ##setattr(process,"AK"+jetRPrefix+"patPuppiJetCharge",                       patJetCharge.clone(src = cms.InputTag("AK"+jetRPrefix+"jetTracksAssociatorAtVertexPuppi")))

  ##setattr(process,"AK"+jetRPrefix+"patPuppiFlavour", cms.Sequence(getattr(process,"AK"+jetRPrefix+"patJetPuppiPartonMatch")*
                                ##getattr(process,"AK"+jetRPrefix+"patJetPuppiGenJetMatch")*
                                ##getattr(process,"AK"+jetRPrefix+"patJetPuppiPartonAssociationLegacy")
                                ##getattr(process,"AK"+jetRPrefix+"patJetPuppiFlavourAssociation")
                                ##getattr(process,"AK"+jetRPrefix+"patPuppiJetCharge")
                                ##)
   ##)
 
  ### pileup jet id
  #setattr(process,"AK"+jetRPrefix+"pileupJetIdPuppi", pileupJetId.clone(
        #jets = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi"),
        #vertexes = cms.InputTag("offlineSlimmedPrimaryVertices"),
        #applyJec = cms.bool(False)
        #)
  #)
  
  
  #### make puppi pat jets 
  #setattr(process,"AK"+jetRPrefix+"patJetsPuppi", patJets.clone( 
                                   #jetSource  = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi"),
                                   #addGenJetMatch = cms.bool(False),
                                   ##genJetMatch        =  cms.InputTag("AK"+jetRPrefix+"patJetPuppiGenJetMatch"),
                                   #addGenPartonMatch = cms.bool(False),
                                   ##genPartonMatch     =  cms.InputTag("AK"+jetRPrefix+"patJetPuppiPartonMatch"),
                                   ##JetPartonMapSource =  cms.InputTag("AK"+jetRPrefix+"patJetPuppiPartonAssociationLegacy"),
                                   #jetCorrFactorsSource   = cms.VInputTag(""),
                                   #addJetCharge = cms.bool(False),
                                   ##jetChargeSource        = cms.InputTag("AK"+jetRPrefix+"patPuppiJetCharge"),
                                   #getJetMCFlavour = cms.bool(False),
                                   ##JetFlavourInfoSource   = cms.InputTag("AK"+jetRPrefix+"patJetPuppiFlavourAssociation"),
                                   #addAssociatedTracks  = cms.bool(False),
                                   ##trackAssociationSource = cms.InputTag("AK"+jetRPrefix+"jetTracksAssociatorAtVertexPuppi"),       
                                   #addTagInfos = cms.bool(False), 
                                   ##tagInfoSources         = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"secondaryVertexTagInfoPuppi")),
                                   #addJetCorrFactors      = cms.bool(False),
                                   #addBTagInfo            = cms.bool(False),
                                   #addDiscriminators      = cms.bool(False),
                                   ##discriminatorSources  = cms.VInputTag(
                                      ##cms.InputTag("AK"+jetRPrefix+"jetBProbabilityBPuppiJetTags"), cms.InputTag("AK"+jetRPrefix+"jetProbabilityBPuppiJetTags"), 
                                      ##cms.InputTag("AK"+jetRPrefix+"trackCountingHighEffbPuppiJetTags"), cms.InputTag("AK"+jetRPrefix+"trackCountingHighPurBPuppiJetTags"), 
                                      ##cms.InputTag("AK"+jetRPrefix+"simpleSecondaryVertexHighEffBPuppiJetTags"),
                                      ##cms.InputTag("AK"+jetRPrefix+"simpleSecondaryVertexHighPurBPuppiJetTags"), 
                                      ##cms.InputTag("AK"+jetRPrefix+"combinedSecondaryVertexBPuppiJetTags"), 
                                      ##cms.InputTag("AK"+jetRPrefix+"combinedSecondaryVertexMVABPuppiJetTags")
                                      ##),
                                   #userData = cms.PSet(
                                        #userCands  = cms.PSet( src = cms.VInputTag("")),
                                        ##userInts = cms.PSet( src = cms.VInputTag("")), # FIXME
                                        ##userFloats = cms.PSet( src = cms.VInputTag("")), # FIXME
                                        #userInts   = cms.PSet( src = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"pileupJetIdPuppi","fullId"))),
                                        #userFloats = cms.PSet( src = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"pileupJetIdPuppi","fullDiscriminant"))),
                                        #userClasses = cms.PSet( src = cms.VInputTag("")),
                                        #userFunctionLabels = cms.vstring(),
                                        #userFunctions = cms.vstring()
                                        #),
                                   #)
  #)

  
  ### make selected puppi jets
  #setattr(process,"AK"+jetRPrefix+"selectedPatJetsPuppi", selectedPatJets.clone(src = cms.InputTag("AK"+jetRPrefix+"patJetsPuppi")))
   
  ##setattr(process,"AK"+jetRPrefix+"makePatJetsPuppi", cms.Sequence( getattr(process,"AK"+jetRPrefix+"PFJetsPuppi")*
  #process.makePatPuppi = cms.Sequence()
  #setattr(process,"makePatPuppi", cms.Sequence( getattr(process,"AK"+jetRPrefix+"PFJetsPuppi")*
                                                                    ##getattr(process,"AK"+jetRPrefix+"btaggingPuppi")*
                                                                    ##getattr(process,"AK"+jetRPrefix+"patPuppiFlavour")*
                                                                    #getattr(process,"AK"+jetRPrefix+"pileupJetIdPuppi")*
                                                                    #getattr(process,"AK"+jetRPrefix+"patJetsPuppi")*
                                                                    #getattr(process,"AK"+jetRPrefix+"selectedPatJetsPuppi"))
  #)
  
  process.makePatPuppi = cms.Sequence()
  setattr(process,"makePatPuppi", cms.Sequence( getattr(process,"AK"+jetRPrefix+"PFJetsPuppi")*
                                                #getattr(process,"AK"+jetRPrefix+"patJetsPuppi")*
                                                process.patJetGenJetMatchAK4selectedPatJetsPuppi*
                                                process.patJetPartonMatchAK4selectedPatJetsPuppi*
                                                process.patJetPartonsPuppi*
                                                #getattr(process,"patJetCorrFactorsAK"+jetRPrefix+"selectedPatJetsPuppi")* # FIXME no JEC
                                                getattr(process,"patJetFlavourAssociationAK"+jetRPrefix+"selectedPatJetsPuppi")*
                                                getattr(process,"patJetsAK"+jetRPrefix+"selectedPatJetsPuppi")
                                                )
  )
  #process.makePatPuppi = cms.Sequence()
  #setattr(process,"makePatPuppi"+"","AK"+jetRPrefix+"makePatJetsPuppi.clone()") 
  
from RecoMET.METProducers.PFMET_cfi import pfMet
from PhysicsTools.PatAlgos.producersLayer1.metProducer_cfi import patMETs
 
def makePatPuppiMetSequence( process ):
 # puppi met
  setattr(process,"pfMetPuppi", pfMet.clone( src = cms.InputTag('puppi'),
                                   calculateSignificance = False))          

  setattr(process,"patMetPuppi",patMETs.clone( 
                                         metSource = cms.InputTag("pfMetPuppi"),
					 addGenMET = cms.bool(False)
					 #getMETSource = cms.InputTag("slimmedGenMetTrue")
					   )
					 )
  #process.patMetPuppi = process.patMETs.clone( metSource = cms.InputTag("pfMetPuppi"))

  process.makePatMetPuppi = cms.Sequence()
  setattr(process, "makePatMetPuppi", cms.Sequence( getattr(process,"pfMetPuppi")*
                                                    getattr(process,"patMetPuppi")
						    )
						    )
