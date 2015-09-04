import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing ('analysis')

# add a list of strings for events to process

#options.register ('doMCweights',
                  #False, # default value
                  #VarParsing.multiplicity.singleton, # singleton or list
                  #VarParsing.varType.bool,
                  #'Turn on MC weights dumper (can be \'True\' or \'False\'')


options.parseArguments()

process = cms.Process("SimpleGenDumper")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring (options.inputFiles),
)

process.TFileService = cms.Service("TFileService",
      fileName = cms.string (options.outputFile),
      closeFileFast = cms.untracked.bool(True)
)

process.Analyzer = cms.EDAnalyzer('PileUpDumper',
    puLabel  = cms.InputTag("addPileupInfo"),
    debug    = cms.untracked.bool(False)
  )

process.p = cms.Path(process.Analyzer)


