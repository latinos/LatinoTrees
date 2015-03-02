import FWCore.ParameterSet.Config as cms

ak4PFCHSL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFCHSL1Fastjet',
                             'ak4PFCHSL2Relative',
                             'ak4PFCHSL3Absolute'))

ak4PFCHSL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFCHSL1Fastjet',
                             'ak4PFCHSL2Relative',
                             'ak4PFCHSL3Absolute',
                             'ak4PFCHSResidual'))


ak4PFCHSL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PFCHS'),
    level = cms.string('L1FastJet'))

ak4PFCHSL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFCHSL1Offset',
        'ak4PFCHSL2Relative',
        'ak4PFCHSL3Absolute'))

ak4PFCHSL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFCHSL1Offset',
        'ak4PFCHSL2Relative',
        'ak4PFCHSL3Absolute',
        'ak4PFCHSResidual'))

ak4PFCHSL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PFCHS'),
    level = cms.string('L1Offset'))

ak4PFCHSL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFCHSL2Relative',
        'ak4PFCHSL3Absolute'))

ak4PFCHSL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFCHSL2Relative',
        'ak4PFCHSL3Absolute',
        'ak4PFCHSResidual'))

ak4PFCHSL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFCHS'),
    level = cms.string('L2Relative'))


ak4PFCHSL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFCHS'),
    level = cms.string('L3Absolute'))

ak4PFCHSResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFCHS'),
    level = cms.string('L2L3Residual'))

ak4PFL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL1Fastjet',
                             'ak4PFL2Relative',
        'ak4PFL3Absolute'))


ak4PFL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL1Fastjet',
        'ak4PFL2Relative',
        'ak4PFL3Absolute',
        'ak4PFL6SLB'))

ak4PFL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL1Fastjet',
        'ak4PFL2Relative',
        'ak4PFL3Absolute',
        'ak4PFResidual'))

ak4PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1FastJet'))

ak4PFL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL1Offset',
        'ak4PFL2Relative',
        'ak4PFL3Absolute'))

ak4PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL1Offset',
        'ak4PFL2Relative',
        'ak4PFL3Absolute',
        'ak4PFResidual'))

ak4PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1Offset'))

ak4PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL2Relative',
        'ak4PFL3Absolute'))

ak4PFL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL2Relative',
        'ak4PFL3Absolute',
        'ak4PFL6SLB'))

ak4PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL2Relative',
        'ak4PFL3Absolute',
        'ak4PFResidual'))

ak4PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PF'),
    level = cms.string('L2Relative'))

ak4PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PF'),
    level = cms.string('L3Absolute'))

ak4PFL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("ak4PFJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("ak4PFJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(False),
    algorithm = cms.string(''),
    level = cms.string('L6SLB'))

ak4PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PF'),
    level = cms.string('L2L3Residual'))




ak5PFCHSL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFCHSL1Fastjet',
                             'ak5PFCHSL2Relative',
                             'ak5PFCHSL3Absolute'))

ak5PFCHSL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFCHSL1Fastjet',
                             'ak5PFCHSL2Relative',
                             'ak5PFCHSL3Absolute',
                             'ak5PFCHSResidual'))


ak5PFCHSL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK5PFCHS'),
    level = cms.string('L1FastJet'))

ak5PFCHSL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFCHSL1Offset',
        'ak5PFCHSL2Relative',
        'ak5PFCHSL3Absolute'))

ak5PFCHSL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFCHSL1Offset',
        'ak5PFCHSL2Relative',
        'ak5PFCHSL3Absolute',
        'ak5PFCHSResidual'))

ak5PFCHSL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5PFCHS'),
    level = cms.string('L1Offset'))

ak5PFCHSL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFCHSL2Relative',
        'ak5PFCHSL3Absolute'))

ak5PFCHSL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFCHSL2Relative',
        'ak5PFCHSL3Absolute',
        'ak5PFCHSResidual'))

ak5PFCHSL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PFCHS'),
    level = cms.string('L2Relative'))


ak5PFCHSL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PFCHS'),
    level = cms.string('L3Absolute'))

ak5PFCHSResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PFCHS'),
    level = cms.string('L2L3Residual'))

ak5PFL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Fastjet',
                             'ak5PFL2Relative',
        'ak5PFL3Absolute'))


ak5PFL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Fastjet',
        'ak5PFL2Relative',
        'ak5PFL3Absolute',
        'ak5PFL6SLB'))

ak5PFL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Fastjet',
        'ak5PFL2Relative',
        'ak5PFL3Absolute',
        'ak5PFResidual'))

ak5PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK5PF'),
    level = cms.string('L1FastJet'))

ak5PFL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Offset',
        'ak5PFL2Relative',
        'ak5PFL3Absolute'))

ak5PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Offset',
        'ak5PFL2Relative',
        'ak5PFL3Absolute',
        'ak5PFResidual'))

ak5PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5PF'),
    level = cms.string('L1Offset'))

ak5PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL2Relative',
        'ak5PFL3Absolute'))

ak5PFL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL2Relative',
        'ak5PFL3Absolute',
        'ak5PFL6SLB'))

ak5PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL2Relative',
        'ak5PFL3Absolute',
        'ak5PFResidual'))

ak5PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L2Relative'))

ak5PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L3Absolute'))

ak5PFL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("ak5PFJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("ak5PFJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(False),
    algorithm = cms.string(''),
    level = cms.string('L6SLB'))

ak5PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L2L3Residual'))
