import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.ak5PFJets_cfi  import ak5PFJets

from RecoMET.METProducers.PFMET_cfi import pfMet

################################
################################       
################################       

def makePuppiAlgo(process):

 ### puppi particles
 process.load('Dummy.Puppi.Puppi_cff')
 process.puppi.candName = cms.untracked.string('particleFlow')
 process.puppi.vertexName = cms.untracked.string('offlinePrimaryVertices')

 process.particleFlowPuppiPtrs = process.particleFlowPtrs.clone(src = cms.InputTag("puppi","Puppi"))

 process.pfPuppiPileUp         = process.pfPileUp.clone( PFCandidates = cms.InputTag("particleFlowPuppiPtrs"))

 process.pfPuppiPileUpJME      = process.pfPileUpJME.clone( PFCandidates = cms.InputTag("particleFlowPuppiPtrs"))

 process.pfPuppiNoPileUp       = process.pfNoPileUp.clone( bottomCollection = cms.InputTag("particleFlowPuppiPtrs"),
                                                           topCollection = cms.InputTag("pfPuppiPileUp"))

 process.pfPuppiNoPileUpJME    = process.pfNoPileUpJME.clone( bottomCollection = cms.InputTag("particleFlowPuppiPtrs"),
                                                              topCollection = cms.InputTag("pfPuppiPileUp"))


 ## packed puppi candidates 
 process.packedPFCandidatesPuppi = process.packedPFCandidates.clone(inputCollection = cms.InputTag("puppi","Puppi"),
                                                                    inputCollectionFromPVTight = cms.InputTag("pfPuppiNoPileUp"),
                                                                    inputCollectionFromPVLoose = cms.InputTag("pfPuppiNoPileUpJME"))

 ## puppi met
 process.pfMetPuppi = pfMet.clone( src = cms.InputTag('puppi','Puppi'),
                                   calculateSignificance = False)          

 process.patMetPuppi = process.patMETs.clone( metSource = cms.InputTag("pfMetPuppi"))

 
 ## final sequence
 process.puppiSequence = cms.Sequence(process.puppi*
                                      process.particleFlowPuppiPtrs*
                                      process.pfPuppiPileUp*
                                      process.pfPuppiNoPileUp*
                                      process.pfPuppiPileUpJME*
                                      process.pfPuppiNoPileUpJME*
                                      process.packedPFCandidatesPuppi*
                                      process.pfMetPuppi*
                                      process.patMetPuppi)                                                                 

#######################
### make puppi jets ###
#######################

def makePatPuppiJetSequence( process, rParameter = 0.5):

 ####### make clustering in AK5 puppi jets
 jetRPrefix = str(int(rParameter*10));
 setattr(process,"AK"+jetRPrefix+"PFJetsPuppi", ak5PFJets.clone( src      = cms.InputTag('puppi','Puppi'),
                                                                 rParam   = cms.double(rParameter),
                                                                 jetPtMin = cms.double(10))
 )

 
 ####### btagging sequence for puppi jets
 ####### jet track association
 setattr(process, "AK"+jetRPrefix+"jetTracksAssociatorAtVertexPuppi",
         process.jetTracksAssociatorAtVertex.clone( jets = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
 
 ## impact parameter and track counting
 setattr(process,"AK"+jetRPrefix+"impactParameterTagInfosPuppi",
         process.impactParameterTagInfos.clone( jetTracks = cms.InputTag("AK"+jetRPrefix+"jetTracksAssociatorAtVertexPuppi")))  
 setattr(process,"AK"+jetRPrefix+"trackCountingHighEffbPuppiJetTags",
         process.trackCountingHighEffBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi")))) 
 setattr(process,"AK"+jetRPrefix+"trackCountingHighPurBPuppiJetTags", 
         process.trackCountingHighPurBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"))))
 setattr(process,"AK"+jetRPrefix+"trackCountingHighPurBPuppiJetTags", 
         process.jetProbabilityBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"))))
 setattr(process,"AK"+jetRPrefix+"jetBProbabilityBPuppiJetTags",
         process.jetBProbabilityBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"))))
 setattr(process,"AK"+jetRPrefix+"jetProbabilityBPuppiJetTags",
         process.jetProbabilityBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"))))
 ## secondary vertex tags   
 setattr(process,"AK"+jetRPrefix+"secondaryVertexTagInfoPuppi", 
         process.secondaryVertexTagInfos.clone( trackIPTagInfos = cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi")))
 setattr(process,"AK"+jetRPrefix+"inclusiveSecondaryVertexFinderTagInfosPuppi", 
         process.inclusiveSecondaryVertexFinderTagInfos.clone( trackIPTagInfos = cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi")))

 setattr(process,"AK"+jetRPrefix+"simpleSecondaryVertexHighEffBPuppiJetTags",
         process.simpleSecondaryVertexHighEffBJetTags.clone(tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"secondaryVertexTagInfoPuppi"))))
 setattr(process,"AK"+jetRPrefix+"simpleSecondaryVertexHighPurBPuppiJetTags",
         process.simpleSecondaryVertexHighPurBJetTags.clone(tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"secondaryVertexTagInfoPuppi"))))
 setattr(process,"AK"+jetRPrefix+"combinedSecondaryVertexBPuppiJetTags",
         process.combinedSecondaryVertexBJetTags.clone(tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"), cms.InputTag("AK"+jetRPrefix+"secondaryVertexTagInfoPuppi"))))
 setattr(process,"AK"+jetRPrefix+"combinedSecondaryVertexMVABPuppiJetTags",
         process.combinedSecondaryVertexMVABJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"), cms.InputTag("AK"+jetRPrefix+"secondaryVertexTagInfoPuppi"))))
 setattr(process,"AK"+jetRPrefix+"combinedInclusiveSecondaryVertexBPuppiJetTags",
         process.combinedInclusiveSecondaryVertexBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"), cms.InputTag("AK"+jetRPrefix+"inclusiveSecondaryVertexFinderTagInfosPuppi"))))
 
 ## soft leptons
 setattr(process,"AK"+jetRPrefix+"ghostTrackVertexTagInfosPuppi",
         process.ghostTrackVertexTagInfos.clone( trackIPTagInfos = cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"))) 
 setattr(process,"AK"+jetRPrefix+"ghostTrackBJetTagsPuppi",
         process.ghostTrackBJetTags.clone( tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"impactParameterTagInfosPuppi"), cms.InputTag("AK"+jetRPrefix+"ghostTrackVertexTagInfosPuppi"))))
 
 setattr(process,"AK"+jetRPrefix+"softPFMuonsTagInfosPuppi", 
         process.softPFMuonsTagInfos.clone( jets = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
 setattr(process,"AK"+jetRPrefix+"softPFMuonBJetTagsPuppi",
         process.softPFMuonBJetTags.clone(tagInfos = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"softPFMuonsTagInfosPuppi"))))
 setattr(process,"AK"+jetRPrefix+"softPFElectronsTagInfosPuppi",
         process.softPFElectronsTagInfos.clone(jets = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
 setattr(process,"AK"+jetRPrefix+"softPFElectronBJetTagsPuppi",
         process.softPFElectronBJetTags.clone(tagInfos= cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"softPFElectronsTagInfosPuppi"))))
 
 ## final sequence
 setattr(process,"AK"+jetRPrefix+"btaggingPuppi", cms.Sequence( getattr(process,"AK"+jetRPrefix+"jetTracksAssociatorAtVertexPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"impactParameterTagInfosPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"trackCountingHighEffbPuppiJetTags")*
                                                                getattr(process,"AK"+jetRPrefix+"trackCountingHighPurBPuppiJetTags")*                                   
                                                                getattr(process,"AK"+jetRPrefix+"jetProbabilityBPuppiJetTags")*
                                                                getattr(process,"AK"+jetRPrefix+"jetBProbabilityBPuppiJetTags")*
                                                                getattr(process,"AK"+jetRPrefix+"secondaryVertexTagInfoPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"simpleSecondaryVertexHighEffBPuppiJetTags")*
                                                                getattr(process,"AK"+jetRPrefix+"simpleSecondaryVertexHighPurBPuppiJetTags")*
                                                                getattr(process,"AK"+jetRPrefix+"combinedSecondaryVertexBPuppiJetTags")*
                                                                getattr(process,"AK"+jetRPrefix+"combinedSecondaryVertexMVABPuppiJetTags")*
                                                                getattr(process,"AK"+jetRPrefix+"combinedInclusiveSecondaryVertexBPuppiJetTags")*
                                                                getattr(process,"AK"+jetRPrefix+"ghostTrackVertexTagInfosPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"ghostTrackBJetTagsPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"softPFMuonsTagInfosPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"softPFMuonBJetTagsPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"softPFElectronsTagInfosPuppi")*
                                                                getattr(process,"AK"+jetRPrefix+"softPFElectronBJetTagsPuppi")
                                                                )
 )

 
 ## jet flavout association
 setattr(process,"AK"+jetRPrefix+"patJetPuppiPartonMatch",
         process.patJetPartonMatch.clone( src = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
 setattr(process,"AK"+jetRPrefix+"patJetPuppiGenJetMatch",
         process.patJetGenJetMatch.clone( src = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
 setattr(process,"AK"+jetRPrefix+"patJetPuppiPartonAssociationLegacy",
         process.patJetPartonAssociationLegacy.clone( jets = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
 setattr(process,"AK"+jetRPrefix+"patJetPuppiFlavourAssociation",
         process.patJetFlavourAssociation.clone(  jets = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi")))
 setattr(process,"AK"+jetRPrefix+"patPuppiJetCharge",
         process.patJetCharge.clone(src = cms.InputTag("AK"+jetRPrefix+"jetTracksAssociatorAtVertexPuppi")))

 setattr(process,"AK"+jetRPrefix+"patPuppiFlavour", cms.Sequence(getattr(process,"AK"+jetRPrefix+"patJetPuppiPartonMatch")*
                               getattr(process,"AK"+jetRPrefix+"patJetPuppiGenJetMatch")*
                               getattr(process,"AK"+jetRPrefix+"patJetPuppiPartonAssociationLegacy")*
                               getattr(process,"AK"+jetRPrefix+"patJetPuppiFlavourAssociation")*
                               getattr(process,"AK"+jetRPrefix+"patPuppiJetCharge"))
  )

 
 ## pileup jet id
 setattr(process,"AK"+jetRPrefix+"pileupJetIdPuppi",
         process.pileupJetId.clone(jets = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi"),
                                   applyJEC = cms.bool(False)))
 
 
 ### make puppi pat jets 
 setattr(process,"AK"+jetRPrefix+"patJetsPuppi", process.patJets.clone( 
                                  jetSource  = cms.InputTag("AK"+jetRPrefix+"PFJetsPuppi"),
                                  genJetMatch        =  cms.InputTag("AK"+jetRPrefix+"patJetPuppiGenJetMatch"),
                                  genPartonMatch     =  cms.InputTag("AK"+jetRPrefix+"patJetPuppiPartonMatch"),
                                  JetPartonMapSource =  cms.InputTag("AK"+jetRPrefix+"patJetPuppiPartonAssociationLegacy"),
                                  jetCorrFactorsSource   = cms.VInputTag(""),
                                  jetChargeSource        = cms.InputTag("AK"+jetRPrefix+"patPuppiJetCharge"),
                                  JetFlavourInfoSource   = cms.InputTag("AK"+jetRPrefix+"patJetPuppiFlavourAssociation"),
                                  trackAssociationSource = cms.InputTag("AK"+jetRPrefix+"jetTracksAssociatorAtVertexPuppi"),       
                                  tagInfoSources         = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"secondaryVertexTagInfoPuppi")),
                                  addJetCorrFactors      = cms.bool(False),
                                  discriminatorSources  = cms.VInputTag(
                                     cms.InputTag("AK"+jetRPrefix+"jetBProbabilityBPuppiJetTags"), cms.InputTag("AK"+jetRPrefix+"jetProbabilityBPuppiJetTags"), 
                                     cms.InputTag("AK"+jetRPrefix+"trackCountingHighEffbPuppiJetTags"), cms.InputTag("AK"+jetRPrefix+"trackCountingHighPurBPuppiJetTags"), 
                                     cms.InputTag("AK"+jetRPrefix+"simpleSecondaryVertexHighEffBPuppiJetTags"),
                                     cms.InputTag("AK"+jetRPrefix+"simpleSecondaryVertexHighPurBPuppiJetTags"), 
                                     cms.InputTag("AK"+jetRPrefix+"combinedSecondaryVertexBPuppiJetTags"), 
                                     cms.InputTag("AK"+jetRPrefix+"combinedSecondaryVertexMVABPuppiJetTags"),
                                     cms.InputTag("AK"+jetRPrefix+"combinedInclusiveSecondaryVertexBPuppiJetTags")),
                                  userData = cms.PSet(
                                       userCands  = cms.PSet( src = cms.VInputTag("")),
                                       userInts   = cms.PSet( src = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"pileupJetIdPuppi","fullId"))),
                                       userFloats = cms.PSet( src = cms.VInputTag(cms.InputTag("AK"+jetRPrefix+"pileupJetIdPuppi","fullDiscriminant"))),
                                       userClasses = cms.PSet( src = cms.VInputTag("")),
                                       userFunctionLabels = cms.vstring('vtxMass','vtxNtracks','vtx3DVal','vtx3DSig'),
                                       userFunctions = cms.vstring('?(tagInfoSecondaryVertex().nVertices()>0)?(tagInfoSecondaryVertex().secondaryVertex(0).p4.M):(0)',
                                                                   '?(tagInfoSecondaryVertex().nVertices()>0)?(tagInfoSecondaryVertex().secondaryVertex(0).nTracks):(0)',
                                                                   '?(tagInfoSecondaryVertex().nVertices()>0)?(tagInfoSecondaryVertex().flightDistance(0).value):(0)',
                                                                   '?(tagInfoSecondaryVertex().nVertices()>0)?(tagInfoSecondaryVertex().flightDistance(0).significance):(0)')
                                       ),)
 )

 
 ## make selected puppi jets
 setattr(process,"AK"+jetRPrefix+"selectedPatJetsPuppi", process.selectedPatJets.clone(src = cms.InputTag("AK"+jetRPrefix+"patJetsPuppi")))
 
 ## make slimmed puppi jets
 setattr(process, "AK"+jetRPrefix+"slimmedJetsPuppi", process.slimmedJets.clone( src = cms.InputTag("AK"+jetRPrefix+"selectedPatJetsPuppi"),
                                                                                 packedPFCandidates = cms.InputTag("packedPFCandidatesPuppi")))
 
 setattr(process,"AK"+jetRPrefix+"makePatJetsPuppi", cms.Sequence( getattr(process,"AK"+jetRPrefix+"PFJetsPuppi")*
                                                                   getattr(process,"AK"+jetRPrefix+"btaggingPuppi")*
                                                                   getattr(process,"AK"+jetRPrefix+"patPuppiFlavour")*
                                                                   getattr(process,"AK"+jetRPrefix+"pileupJetIdPuppi")*
                                                                   getattr(process,"AK"+jetRPrefix+"patJetsPuppi")*
                                                                   getattr(process,"AK"+jetRPrefix+"selectedPatJetsPuppi")*
                                                                   getattr(process,"AK"+jetRPrefix+"slimmedJetsPuppi"))
 )
 
                               
