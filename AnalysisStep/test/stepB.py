import FWCore.ParameterSet.Config as cms
import LatinoTrees.Misc.VarParsing as opts
import re
import sys

options = opts.VarParsing('analysis')

#-------------------------------------------------------------------------------
# some basic cleanup
del options._register['filePrepend']
del options._register['totalSections']
del options._register['section']
del options._register['secondaryOutputFile']
del options._singletons['filePrepend']
del options._singletons['totalSections']
del options._singletons['section']
del options._singletons['secondaryOutputFile']
del options._register['secondaryInputFiles']

del options._lists['secondaryInputFiles']
#-------------------------------------------------------------------------------
options.register ('summary',
                  True,
                  opts.VarParsing.multiplicity.singleton,
                  opts.VarParsing.varType.bool,
                  'Print run summary')

options.register ('skipEvents',
                  0, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.int, # string, int, or float
                  'Number of events to skip')

options.register ('reportEvery',
                  1000, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.int, # string, int, or float
                  'Write Begin processing the ... only every N events')

options.register ('label',
                  'XXX',
                  opts.VarParsing.multiplicity.singleton,
                  opts.VarParsing.varType.string,
                  'Label')

options.register ('json',
                  'YYY',
                  opts.VarParsing.multiplicity.singleton,
                  opts.VarParsing.varType.string,
                  'Json file for data')

options.register ('id',
                  0, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.string, # string, int, or float
                  'Dataset id')

options.register ('scale',
                  0, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.float, # string, int, or float
                  'Scale factor')

options.register ('doTauEmbed',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on DY embedding mode (can be \'True\' or \'False\')')

options.register ('selection',
                  'Tight',
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.string, # string, int, or float
                  'Selection level [Tight,Loose]')

options.register ('doSameSign',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on Same Sign mode (can be \'True\' or \'False\')')

options.register ('doType01met',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on Type01 met correction Sign mode (can be \'True\' or \'False\')')

options.register ('doSusy',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on Susy MC dumper (can be \'True\' or \'False\')')

options.register ('doHiggs',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on Higgs MC mass dumper (can be \'True\' or \'False\')')

options.register ('doLHE',
                  True, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on LHE dumper (can be \'True\' or \'False\')')

options.register ('typeLHEcomment',
                  0, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.int, # string, int, or float
                  'type of comment in LHE, 0 [default] powheg scale variation, 1 MG for anomalous couplings')

options.register ('doGen',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on gen Variables dumper (can be \'True\' or \'False\')')

options.register ('doNoFilter',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on no filter requirement, not even requiring 2 leptons! Needed for unfolding at GEN (can be \'True\' or \'False\')')

options.register ('doCut',
                  '1',
                  opts.VarParsing.multiplicity.singleton,
                  opts.VarParsing.varType.string,
                  'Apply cut. String. Default = \'1\', meaning accept all events. NB: flag doNoFilter overrides this cut!')

options.register ('doMuonIsoId',
                  True, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on muon id/iso dumper (can be \'True\' or \'False\')')

options.register ('doEleIsoId',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on electron id/iso dumper (can be \'True\' or \'False\')')

options.register ('acceptDuplicates',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'accept duplicates. Suggested true for private production (can be \'True\' or \'False\')')

options.register ('doFatJet',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on Fat jet production and dumper (can be \'True\' or \'False\')')

options.register ('puInformation',
                  'slimmedAddPileupInfo', # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.string, # string, int, or float
                  'name of pile-up information collection: it may change for premixing samples in MC, slimmedAddPileupInfo [default], mixData for premixinf')

options.register ('doPhotonID',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on photon ID variables (can be \'True\' or \'False\')')

options.register ('doIsoStudy',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on isolation studies, additional variables, ... (can be \'True\' or \'False\')')

options.register ('runPUPPISequence',
                  True,
                  opts.VarParsing.multiplicity.singleton,
                  opts.VarParsing.varType.bool,
                  'Turn on PUPPI jets (can be \'True\' or \'False\')')

options.register ('globalTag',
                  '74X_mcRun2_asymptotic_v4',
                   opts.VarParsing.multiplicity.singleton,
                   opts.VarParsing.varType.string,
                  'GlobalTag')

options.register ('doBTag',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on bTagging variables dumper (can be \'True\' or \'False\')')

options.register ('doMCweights',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on MC weights dumper (can be \'True\' or \'False\')')

options.register ('LHEweightSource',
                  'externalLHEProducer', # default value
                   opts.VarParsing.multiplicity.singleton,
                   opts.VarParsing.varType.string,
                  'LHE weight source. It depends on sample and on how they were produced: pLHE, wMLHE, ... (suggested options  \'externalLHEProducer\' or \'source\')')

options.register ('LHERunInfo',
                  'externalLHEProducer', # default value
                   opts.VarParsing.multiplicity.singleton,
                   opts.VarParsing.varType.string,
                  'LHE Info used at beginRun to dump weights explanations')

options.register ('doSoftActivity',
                  True, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Turn on soft activity variables (can be \'True\' or \'False\')')

options.register ('is50ns',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Switch between 50ns and 25ns (can be \'True\' or \'False\')')

options.register ('isPromptRecoData',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton, # singleton or list
                  opts.VarParsing.varType.bool,
                  'Switch between PAT and RECO process names for the MET filters (can be \'True\' or \'False\')')

options.register ('metNoHF',
                  'slimmedMETsNoHF', # default value
                   opts.VarParsing.multiplicity.singleton,
                   opts.VarParsing.varType.string,
                  'metNoHF. Only in miniAOD v2 (set as empty if collection to be skipped)')

options.register ('doCorrectMet',
                  False, # default value
                  opts.VarParsing.multiplicity.singleton,
                  opts.VarParsing.varType.string,
                  'Turn on MET Type-1 correction')
 



#-------------------------------------------------------------------------------
# defaults
options.outputFile = 'stepB.root'
options.maxEvents = -1 # all events
#-------------------------------------------------------------------------------

options.parseArguments()

process = cms.Process("stepB")

# logger
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
#process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.MessageLogger.cerr.FwkReport.reportEvery = options.reportEvery


# accepting te duplicate is needed in case we have generated
# privately some samples and the event number always starts from 0
if options.acceptDuplicates :
    process.source = cms.Source('PoolSource',
          fileNames = cms.untracked.vstring( options.inputFiles ),
          skipEvents = cms.untracked.uint32( options.skipEvents ),
          duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
          )
else :
    process.source = cms.Source('PoolSource',
          fileNames = cms.untracked.vstring( options.inputFiles ),
          skipEvents = cms.untracked.uint32( options.skipEvents ),
          )
###

process.source.inputCommands = cms.untracked.vstring( "keep *", "drop *_conditionsInEdm_*_*", "drop *_MEtoEDMConverter_*_*")


process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(options.summary))
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

## multithread (begin)
#import os
#CPUS = os.popen('grep -c ^processor /proc/cpuinfo').read()
#process.options.numberOfThreads = cms.untracked.uint32(int(CPUS)-2)
## multithread (end)

#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

#globalTag = options.globalTag + "::All"
globalTag = options.globalTag
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

#
# load ES producers
#
#process.load('Configuration.StandardSequences.Geometry_cff')
#process.load("Geometry.TrackerGeometryBuilder.trackerGeometryDB_cfi")
 
#process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')

#process.load('Configuration.StandardSequences.MagneticField_38T_cff')
#process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')

#process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi") #---> not working in 74???
 
#process.load("Configuration.StandardSequences.GeometryDB_cff")
#process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")  
process.load("Geometry.TrackerGeometryBuilder.trackerGeometryDB_cfi")
 
#process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.Services_cff')

process.GlobalTag.globaltag = globalTag



# load configurations
doMuonIsoId = options.doMuonIsoId
doEleIsoId = options.doEleIsoId
doLHE = options.doLHE
doGen = options.doGen
doHiggs = options.doHiggs
doSusy = options.doSusy
doTauEmbed = options.doTauEmbed
doSameSign = options.doSameSign
doNoFilter = options.doNoFilter
doIsoStudy = options.doIsoStudy
typeLHEcomment = options.typeLHEcomment
label = options.label
doBTag = options.doBTag
doCut = options.doCut
doMCweights = options.doMCweights


print "      __          __  _                ______              "
print "     / /   ____ _/ /_(_)___  ____     /_  __/_______  ___  "
print "    / /   / __ `/ __/ / __ \/ __ \     / / / ___/ _ \/ _ \ "
print "   / /___/ /_/ / /_/ / / / / /_/ /    / / / /  /  __/  __/ "
print "  /_____/\__,_/\__/_/_/ /_/\____/    /_/ /_/   \___/\___/  "
print "                                                           "


id = 0
json = None
isMC = True

wztth = False
dy = False


import LatinoTrees.AnalysisStep.globalVariables as globalVariables


# SkimEventProducer is where the objects are defined
#  and all configurations and parameters are defined too
process.load("LatinoTrees.AnalysisStep.skimEventProducer_cfi")


if options.is50ns :
    process.skimEventProducer.apply50nsValues = cms.bool(True)
else :
    process.skimEventProducer.apply50nsValues = cms.bool(False) 


# Default parameters for jets
process.skimEventProducer.maxEtaForJets = cms.double(4.7)
process.skimEventProducer.minPtForJets = cms.double(0)
process.skimEventProducer.applyCorrectionForJets = cms.bool(True) 
process.skimEventProducer.applyIDForJets = cms.int32(int(globalVariables.jetId_WP))
# 7 Run II jetID LOOSE
# 8 Run II jetID TIGHT

process.skimEventProducer.dzCutForBtagJets = cms.double(999999.9)


# load configuration file with the variables list
from LatinoTrees.AnalysisStep.stepB_cff import *





print " >> label:: ", label

if '2011' in label: label = label[:label.find('2011')]
if '2012' in label: label = label[:label.find('2012')]
if '2015' in label: label = label[:label.find('2015')]


# data
if label in [ 'SingleElectron', 'DoubleEG', 'SingleMuon', 'DoubleMuon', 'MuEG', 'MET', 'SinglePhoton' ]:
    print " >> DATA:: ", label
    dataset = [label]
    id = options.id
    if options.json != "YYY" :
      json = options.json
    scalef = 1
    doPDFvar = False
    doGen = False
    doLHE = False
    isMC = False
    doMCweights = False
    process.skimEventProducer.isMC = cms.untracked.int32(0)

# dytt embedded sample
elif doTauEmbed == True:
    dataset = ["AllEmbed"]
    id = options.id
    if options.json != "YYY" :
      json = options.json
    scalef = 1
    doPDFvar = False
    doGen = False
    doLHE = False

# mc
else:
    dataset = ['MC', label];
    id = options.id;
    scalef = options.scale
    doPDFvar = True


stepBTree.variables.trigger         = stepBTree.variables.trigger.value().replace("DATASET",dataset[0])
stepBTree.variables.triggerFakeRate = stepBTree.variables.triggerFakeRate.value().replace("DATASET",dataset[0])

idn = re.sub('[^0-9]','',str(id))
print " >> idn = ", idn
stepBTree.variables.dataset = str(idn)


# Change TriggerResults process name (for the MET filters) according to MC/PromptRecoData/17Jul2015Data
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2015#ETmiss_filters
if isMC :
  #process.skimEventProducer.triggerSpecialTag = cms.InputTag("TriggerResults","","PAT")
  process.skimEventProducer.triggerSpecialTag = cms.InputTag("TriggerResults","","HLT")
elif options.isPromptRecoData :
  process.skimEventProducer.triggerSpecialTag = cms.InputTag("TriggerResults","","RECO")
else : 
  process.skimEventProducer.triggerSpecialTag = cms.InputTag("TriggerResults","","PAT")


# Set metNoHF tag (fix for miniAOD older version)
process.skimEventProducer.pfMetNoHfTag = cms.InputTag(options.metNoHF)


#
# The lines below work in CMSSW_8_0_5
#
if options.doCorrectMet :
    from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
    runMetCorAndUncFromMiniAOD(process, isData=(not isMC))


# save triggers only in DATA
if not isMC :
    setattr(stepBTree.variables, "std_vector_trigger",           cms.string("selectedRateTrigger/60") )
    setattr(stepBTree.variables, "std_vector_trigger_prescale",  cms.string("selectedRateTriggerPrescale/60") )
    setattr(stepBTree.variables, "std_vector_trigger_L1min_prescale",  cms.string("selectedRateTriggerL1minPrescale/60") )
    setattr(stepBTree.variables, "std_vector_trigger_L1max_prescale",  cms.string("selectedRateTriggerL1maxPrescale/60") )
    # special paths, e.g. metFilters. See skimEventProducer_cfi for the list
    setattr(stepBTree.variables, "std_vector_trigger_special",   cms.string("specialRateTrigger/8") )
if isMC :
    process.skimEventProducer.SelectedPaths = cms.vstring ("")
    # special paths always saved
    setattr(stepBTree.variables, "std_vector_trigger_special",   cms.string("specialRateTrigger/8") )



# mc
if dataset[0] == "MC":
    stepBTree.variables.baseW = "%.12f" % scalef
# data
else:
    from FWCore.PythonUtilities.LumiList import LumiList
    import os
    if json != None :
      lumis = LumiList(filename = os.getenv('CMSSW_BASE')+'/src/LatinoTrees/Misc/Jsons/%s.json'%json)
      process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()
      process.source.lumisToProcess = lumis.getCMSSWString().split(',')
    stepBTree.variables.baseW = "1"
    stepBTree.variables.trpu = cms.string("1")
    stepBTree.variables.itpu = cms.string("1")
    stepBTree.variables.ootpup1 = cms.string("1")
    stepBTree.variables.ootpum1 = cms.string("1")
    stepBTree.variables.puW = cms.string("1")
    stepBTree.variables.puAW = cms.string("1")
    stepBTree.variables.puBW = cms.string("1")


####################
# run electron id ##
# see twiki:
#    https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipesRun2
# 
#
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
dataFormat = DataFormat.MiniAOD
switchOnVIDElectronIdProducer(process, dataFormat)

#process.skimEventProducer.electronIds = cms.vstring(
 #)

# define which IDs we want to produce
if options.is50ns :
    my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Spring15_50ns_V1_cff',
                     'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV60_cff']

    process.skimEventProducer.electronIds = cms.vstring(
        "egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V1-standalone-veto",
        "egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V1-standalone-loose",
        "egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V1-standalone-medium",
        "egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V1-standalone-tight",
        )

    stepBTree.variables.std_vector_lepton_eleIdVeto   = stepBTree.variables.std_vector_lepton_eleIdVeto  .value().replace("FREQUENCY","50ns")
    stepBTree.variables.std_vector_lepton_eleIdLoose  = stepBTree.variables.std_vector_lepton_eleIdLoose .value().replace("FREQUENCY","50ns")
    stepBTree.variables.std_vector_lepton_eleIdMedium = stepBTree.variables.std_vector_lepton_eleIdMedium.value().replace("FREQUENCY","50ns")
    stepBTree.variables.std_vector_lepton_eleIdTight  = stepBTree.variables.std_vector_lepton_eleIdTight .value().replace("FREQUENCY","50ns")
else :
    my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Spring15_25ns_V1_cff',
                     'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV60_cff']

    process.skimEventProducer.electronIds = cms.vstring(
        "egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-veto",
        "egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-loose",
        "egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-medium",
        "egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-tight",
        )

    stepBTree.variables.std_vector_lepton_eleIdVeto   = stepBTree.variables.std_vector_lepton_eleIdVeto  .value().replace("FREQUENCY","25ns")
    stepBTree.variables.std_vector_lepton_eleIdLoose  = stepBTree.variables.std_vector_lepton_eleIdLoose .value().replace("FREQUENCY","25ns")
    stepBTree.variables.std_vector_lepton_eleIdMedium = stepBTree.variables.std_vector_lepton_eleIdMedium.value().replace("FREQUENCY","25ns")
    stepBTree.variables.std_vector_lepton_eleIdTight  = stepBTree.variables.std_vector_lepton_eleIdTight .value().replace("FREQUENCY","25ns")

# add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

####################

#
# These selections are used to define the collection
# of leptons and are defined in 
# python/skimEventProducer_cfi.py
#

if options.selection == 'Tight':
    labelSetup = "TreeProducer"; muon = "slimmedMuons"; ele = "slimmedElectrons"; softmu = "slimmedMuons"; pho = "slimmedPhotons"; preSeq = cms.Sequence();
    #labelSetup = "Scenario1"; muon = "slimmedMuons"; ele = "slimmedElectrons"; softmu = "slimmedMuons"; pho = "slimmedPhotons"; preSeq = cms.Sequence();
    #labelSetup = "Scenario1"; muon = "wwMuoTight"; ele = "wwEleTight"; softmu = "slimmedMuons"; pho = "slimmedPhotons"; preSeq = cms.Sequence();  # --> fix ele/mu tag and un-comment this line and comment the previous one
elif options.selection == 'Loose':
    labelSetup = "TreeProducer"; muon = "wwMuScenario7"; ele = "wwEleScenario5"; softmu = "wwMu4VetoScenario6"; pho = "wwPhoScenario1"; preSeq = cms.Sequence();
    #labelSetup = "Scenario7"; muon = "wwMuScenario7"; ele = "wwEleScenario5"; softmu = "wwMu4VetoScenario6"; pho = "wwPhoScenario1"; preSeq = cms.Sequence();
elif options.selection == 'LooseNoIso':
    process.skimEventProducer.applyJetCleaning = cms.int32(0)  # don't apply jet cleaning, since leptons are not-isolated here
    labelSetup = "TreeProducer"; muon = "wwMuoTightNoIso"; ele = "wwEleLooseNoIso"; softmu = "wwMuoForVeto"; pho = "slimmedPhotons"; preSeq = cms.Sequence(); 
else:
    raise ValueError('selection must be either Tight or Loose')



# add L1+L2+L3 jet energy corrections in MC
# first create the "raw" jets
#process.load("LatinoTrees.JetUncorrector.JetUncorrector_cff")
#preSeq += process.rawJets
# then apply the new corrections
process.load("JetMETCorrections.Configuration.JetCorrectionServices_cff")
process.load("JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff")

from JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox
#from RecoJets.JetProducers.jetToolbox_cff import jetToolbox
process.myJetSequence = cms.Sequence()

JEC = ['L1FastJet','L2Relative','L3Absolute']
if not isMC:
    JEC += ['L2L3Residual']

jetToolbox( process, 'ak4', 'myJetSequence', 'outTemp',    
             JETCorrPayload='AK4PFchs', JETCorrLevels = JEC, 
             miniAOD=True,
             runOnMC=isMC,
             addNsub=True,  # was True      
             #addPUJetID=False,
             #bTagDiscriminators = ['pfTrackCountingHighEffBJetTags','pfTrackCountingHighPurBJetTags','pfJetProbabilityBJetTags','pfJetBProbabilityBJetTags','pfSimpleSecondaryVertexHighEffBJetTags','pfSimpleSecondaryVertexHighPurBJetTags','pfCombinedSecondaryVertexV2BJetTags','pfCombinedInclusiveSecondaryVertexV2BJetTags','pfCombinedMVAV2BJetTags'], 
             #bTagDiscriminators = ['pfTrackCountingHighEffBJetTags','pfTrackCountingHighPurBJetTags','pfJetProbabilityBJetTags','pfJetBProbabilityBJetTags','pfSimpleSecondaryVertexHighEffBJetTags','pfSimpleSecondaryVertexHighPurBJetTags','pfCombinedSecondaryVertexV2BJetTags','pfCombinedInclusiveSecondaryVertexV2BJetTags'], 
             #bTagDiscriminators = ['pfTrackCountingHighEffBJetTags','pfTrackCountingHighPurBJetTags','pfJetProbabilityBJetTags','pfJetBProbabilityBJetTags','pfSimpleSecondaryVertexHighEffBJetTags','pfSimpleSecondaryVertexHighPurBJetTags','pfCombinedSecondaryVertexV2BJetTags','pfCombinedInclusiveSecondaryVertexV2BJetTags'],              
             addPUJetID=True,
             addPruning=False,
             addTrimming=False,
             addCMSTopTagger=True,
             addHEPTopTagger=True,
             addMassDrop=True,
             addSoftDrop=False, 
             addQGTagger=True,  # addSoftDrop=True
             ) #, addPrunedSubjets=True )


preSeq += process.myJetSequence

# no need to recorrect the jets, since they are reclustered on the fly
# the name patJetsAK4PFCHS found looking at the "processDump.py" and looking for patjetproducer
#   edmConfigDump stepB.py &> processDump.py
#
process.skimEventProducer.jetTag    = cms.InputTag("selectedPatJetsAK4PFCHS")
process.skimEventProducer.tagJetTag = cms.InputTag("selectedPatJetsAK4PFCHS")
if options.doFatJet :
    process.skimEventProducer.fatJetTag = cms.InputTag("slimmedJetsAK8")



# QG tagger
# it will be soon included in jettoolbox
#process.load('RecoJets.JetProducers.QGTagger_cfi')
#process.QGTagger.srcJets          = cms.InputTag('corJets')        # Could be reco::PFJetCollection or pat::JetCollection (both AOD and miniAOD)
#process.QGTagger.jetsLabel        = cms.string('QGL_AK4PFchs')     # Other options (might need to add an ESSource for it): see https://twiki.cern.ch/twiki/bin/viewauth/CMS/QGDataBaseVersion
#process.QGTagger.jec              = cms.string(<jet corrector>)       # Provide the jet correction service if your jets are uncorrected, otherwise keep empty
#process.QGTagger.systematicsLabel = cms.string('')     # Produce systematic smearings (not yet available, keep empty)

#process.patJets.userData.userFloats.src += ['QGTagger:qgLikelihood']

#add dressed leptons sequence
if isMC:
  process.load("LatinoTrees.AnalysisStep.dressedLeptons_cff")
  preSeq += process.dressedLeptonsSequence

# add puppi calculated from miniAOD
#   since puppi must be run as first
#   we include puppi in the "preSequence"
#   it's not possible to add it afterwards, 
#   unless we transform everything into a sequence
#   and let cmssw do the rest ...
if options.runPUPPISequence:

    from LatinoTrees.AnalysisStep.puppiSequence_cff import makePuppiAlgo, makePatPuppiJetSequence, makePatPuppiMetSequence
    #from LatinoTrees.AnalysisStep.puppiSequence_cff import makePuppiAlgo, makePatPuppiMetSequence

    jetPuppiR = 0.4
    #makePuppiAlgo(process) ## call puppi producer and puppi met
    
    jetToolbox( process, 'ak4', 'myPuppiJetSequence', 'outTemp',    
             JETCorrPayload='AK4PFPuppi', JETCorrLevels = JEC, 
             PUMethod='Puppi',
             miniAOD=True,
             runOnMC=isMC, 
             addNsub=True,  # was True      
             addPUJetID=False,
             #addPUJetID=True, ----> can't be puppi AND pujetid
             addPruning=False,
             addTrimming=False,
             addCMSTopTagger=True,
             addHEPTopTagger=True, 
             addMassDrop=True,
             addSoftDrop=False, 
             addQGTagger=True,  
             #addSoftDrop=True
             ) #, addPrunedSubjets=True )


    #makePatPuppiJetSequence(process,jetPuppiR) ## call pat puppi jets
    makePatPuppiMetSequence(process) ## call pat puppi met

    # now add to the preSequence
    preSeq += process.makePatMetPuppi
    preSeq += process.myPuppiJetSequence

    process.skimEventProducer.pupMetTag = cms.InputTag("patMetPuppi")
    # the name selectedPatJetsAK4PFPuppi found looking at the "processDump.py" and looking for patjetproducer
    process.skimEventProducer.secondJetTag = cms.InputTag("selectedPatJetsAK4PFPuppi")
    #process.skimEventProducer.secondJetTag = cms.InputTag("patJetsAK4selectedPatJetsPuppi")

if options.doSoftActivity:   
    # Add trackjets
    process.chargedPackedPFCandidates = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("charge != 0 && abs(eta) < 2.5 && pt > 0.3 && fromPV >= 2"))

    from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets 
    process.ak4TrackJets = ak4PFJets.clone(src = cms.InputTag('chargedPackedPFCandidates'), jetPtMin = cms.double(1.0))
    
    process.trackJetSequence = cms.Sequence(process.chargedPackedPFCandidates * process.ak4TrackJets)
    preSeq += process.trackJetSequence

    # Make pat track-jets
    from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection

    addJetCollection(   
        process,
        labelName = 'AK4PFTrack',
        jetSource = cms.InputTag('ak4TrackJets'),
        algo = 'AK',
        rParam = 0.4,
        jetCorrections = None, 
        pfCandidates = cms.InputTag( 'packedPFCandidates' ),  
        svSource = cms.InputTag( 'slimmedSecondaryVertices' ),  
        genJetCollection = cms.InputTag( 'slimmedGenJets'),
        pvSource = cms.InputTag( 'offlineSlimmedPrimaryVertices' ), 
        btagDiscriminators = [
                #'pfTrackCountingHighEffBJetTags',
                #'pfTrackCountingHighPurBJetTags',
                'pfJetProbabilityBJetTags'
                #'pfJetBProbabilityBJetTags'
        ],
        getJetMCFlavour = isMC,
        genParticles = cms.InputTag('prunedGenParticles'),
        outputModules = ['outputFile']
    )
 
    getattr(process,'selectedPatJetsAK4PFTrack').cut = cms.string('pt > 1.0')
    preSeq += getattr(process,'selectedPatJetsAK4PFTrack')
    process.skimEventProducer.trackJetTag = cms.InputTag("selectedPatJetsAK4PFTrack")
    
# create the EventHypothesis
# and tweaks for special MC/data samples

from LatinoTrees.AnalysisStep.skimEventProducer_cfi import addEventHypothesis
process.skimEventProducer.triggerTag = cms.InputTag("TriggerResults","","HLT")
#process.skimEventProducer.triggerTag = cms.InputTag("TriggerResults","","PAT")

if doTauEmbed == True:
  process.skimEventProducer.triggerTag = cms.InputTag("TriggerResults","","EmbeddedRECO")
  process.skimEventProducer.mcGenWeightTag = cms.InputTag("generator:minVisPtFilter")

# this is where the "event" is built
#                                                                sequence and no path
#addEventHypothesis(process,labelSetup,muon,ele,softmu,pho,preSeq,False)
#addEventHypothesis(process,labelSetup,muon,ele,softmu,pho,preSeq,True)
#addEventHypothesis(process,labelSetup,muon,ele,softmu,pho,preSeq,True,"1")
addEventHypothesis(process,labelSetup,muon,ele,softmu,pho,preSeq,True,doCut)

#process.options = cms.untracked.PSet( allowUnscheduled = cms.untracked.bool(True) )
process.options.allowUnscheduled = cms.untracked.bool(True)

if (wztth == True) or (doPDFvar == True):
    getattr(process,"ww%s"% (labelSetup)).mcGenEventInfoTag = "generator"

#if doLHE == True :
    #getattr(process,"ww%s"% (labelSetup)).mcLHEEventInfoTag = "source"
    #getattr(process,"ww%s"% (labelSetup)).whichLHE = cms.untracked.int32(typeLHEcomment)


if doGen == True :
    # Difference between "prunedGenParticles" and "packedGenParticles": https://twiki.cern.ch/twiki/bin/viewauth/CMS/MiniAOD#PackedGenParticles
    getattr(process,"ww%s"% (labelSetup)).genParticlesTag = "prunedGenParticles"
    #getattr(process,"ww%s"% (labelSetup)).genMetTag = "slimmedMETs" # "genMetTrue"
    getattr(process,"ww%s"% (labelSetup)).genJetTag = "slimmedGenJets"
    #cms.InputTag("ak5GenJetsNoElNoMuNoNu","","Yield")
    getattr(process,"ww%s"% (labelSetup)).dressedMuonTag     = "dressedMuons01" 
    getattr(process,"ww%s"% (labelSetup)).dressedElectronTag = "dressedElectrons01" 

#if id in ["036", "037", "037c0", "037c1", "037c2", "037c3", "037c4", "037c5", "037c6", "037c7", "037c8", "037c9", "042", "043", "045", "046" ]: # DY-Madgraph sample
    #getattr(process,"ww%s"% (labelSetup)).genParticlesTag = "prunedGen"



# if puppi, change the input jet collection
#if options.runPUPPISequence:
    #getattr(process,"ww%s"% (labelSetup)).secondJetTag    = "patJetsAK4selectedPatJetsPuppi"

# latino trees construction

#tree = stepBTree.clone(src = cms.InputTag("ww%s"% (labelSetup) ));
tree = stepBTree.clone(src = cms.InputTag("skim%s"% (labelSetup) )); # --> source is "skimmed" events!


seq = cms.Sequence()
setattr(process, 'TreeSequence', seq)
#setattr(process, "Nvtx", nverticesModule.clone(probes = cms.InputTag("ww%s"% (labelSetup))))
setattr(process, "Nvtx", nverticesModule.clone(probes = cms.InputTag("skim%s"% (labelSetup))))
seq += getattr(process, "Nvtx")
tree.variables.nvtx = cms.InputTag("Nvtx")
if doIsoStudy: 
    addIsoStudyVariables(process,tree)


# pile-up
nPU = cms.EDProducer("PileUpMultiplicityCounter",
    puLabel = cms.InputTag(options.puInformation),  # v1 miniaod addPileupInfo 
    src = cms.InputTag("")
)

# pile-up information
nPU.puLabel = cms.InputTag(options.puInformation)


if dataset[0] == 'MC':
    #setattr(process, "NPU", nPU.clone(src = cms.InputTag("ww%s"% (labelSetup))))
    setattr(process, "NPU", nPU.clone(src = cms.InputTag("skim%s"% (labelSetup))))
    seq += getattr(process, "NPU")
    tree.variables.trpu = cms.InputTag("NPU:tr")
    tree.variables.itpu = cms.InputTag("NPU:it")
    tree.variables.ootpup1 = cms.InputTag("NPU:p1")
    tree.variables.ootpum1 = cms.InputTag("NPU:m1")
    tree.variables.ootpup2 = cms.InputTag("NPU:p2")
    tree.variables.ootpum2 = cms.InputTag("NPU:m2")
    tree.variables.ootpup3 = cms.InputTag("NPU:p3")
    tree.variables.ootpum3 = cms.InputTag("NPU:m3")
    tree.variables.ootpum4 = cms.InputTag("NPU:m4")
    tree.variables.ootpum5 = cms.InputTag("NPU:m5")


# muon id and iso variables
if doMuonIsoId:
  addMuonIdIsoVariables(process,tree)

# electron id and iso variables
if doEleIsoId:
  addEleIdIsoVariables(process,tree)

# PHOTON VARIABLES
if options.doPhotonID:
  addPhotonIDVariables(process,tree)
 
addPhotonVariables(process,tree)

# LHE information dumper
if doLHE:
  addLHEVariables(process,tree)

  #tree.variables.numbLHE = cms.string("numberOfbQuarks()")
  #tree.variables.numtLHE = cms.string("numberOftQuarks()")

  #tree.variables.HEPMCweightScale0 = cms.string("HEPMCweightScale(0)")
  #tree.variables.HEPMCweightScale1 = cms.string("HEPMCweightScale(1)")
  #tree.variables.HEPMCweightScale2 = cms.string("HEPMCweightScale(2)")
  #tree.variables.HEPMCweightScale3 = cms.string("HEPMCweightScale(3)")
  #tree.variables.HEPMCweightScale4 = cms.string("HEPMCweightScale(4)")
  #tree.variables.HEPMCweightScale5 = cms.string("HEPMCweightScale(5)")
  #tree.variables.HEPMCweightScale6 = cms.string("HEPMCweightScale(6)")

  #if typeLHEcomment == 1 :
     ##import ROOT
     #for i in range (70) :
         ##ROOT.gROOT.ProcessLine("tree.variables.HEPMCweightScale" + str(i+7) + " = cms.string(\"HEPMCweightScale(" + str(i+7) + ")\")")
         #exec("tree.variables.HEPMCweightScale" + str(i+7) + " = cms.string(\"HEPMCweightScale(" + str(i+7) + ")\")")

  #tree.variables.HEPMCweightRen0 = cms.string("HEPMCweightRen(0)")
  #tree.variables.HEPMCweightRen1 = cms.string("HEPMCweightRen(1)")
  #tree.variables.HEPMCweightRen2 = cms.string("HEPMCweightRen(2)")
  #tree.variables.HEPMCweightRen3 = cms.string("HEPMCweightRen(3)")
  #tree.variables.HEPMCweightRen4 = cms.string("HEPMCweightRen(4)")
  #tree.variables.HEPMCweightRen5 = cms.string("HEPMCweightRen(5)")
  #tree.variables.HEPMCweightRen6 = cms.string("HEPMCweightRen(6)")

  #tree.variables.HEPMCweightFac0 = cms.string("HEPMCweightFac(0)")
  #tree.variables.HEPMCweightFac1 = cms.string("HEPMCweightFac(1)")
  #tree.variables.HEPMCweightFac2 = cms.string("HEPMCweightFac(2)")
  #tree.variables.HEPMCweightFac3 = cms.string("HEPMCweightFac(3)")
  #tree.variables.HEPMCweightFac4 = cms.string("HEPMCweightFac(4)")
  #tree.variables.HEPMCweightFac5 = cms.string("HEPMCweightFac(5)")
  #tree.variables.HEPMCweightFac6 = cms.string("HEPMCweightFac(6)")

if doGen: addGenVariables(process,tree)

# add QG likelihood
addQGJets(process,tree)   

# add fatjets
if options.doFatJet :
    addFatJets(process,tree)


addTau(process,tree)

if doMCweights:
    addMCweights(process,tree)
    # if you add weights you need LHE and GEN
    # getattr(process,"ww%s"% (labelSetup)).mcLHEEventInfoTag = "source"  #----> the name of this collection is sample dependent!!! What!?!? BE CAREFUL!
    # getattr(process,"ww%s"% (labelSetup)).mcLHEEventInfoTag = "externalLHEProducer"
    getattr(process,"ww%s"% (labelSetup)).mcLHEEventInfoTag = options.LHEweightSource
    getattr(process,"ww%s"% (labelSetup)).mcGenEventInfoTag = "generator"


if id in ["036", "037", "037c0", "037c1", "037c2", "037c3", "037c4", "037c5", "037c6", "037c7", "037c8", "037c9", "042", "043", "045", "046" ]: # DY-Madgraph sample
    tree.variables.mctruth = cms.string("getFinalStateMC()")

if id in ["077", "078", "074" ]:
    tree.variables.PtZ = cms.string("getZPt()")
    tree.variables.MZ = cms.string("getZMass()")
    tree.variables.WZchan = cms.string("getWZdecayMC()")

if doTauEmbed == True:
    tree.variables.mctruth = cms.string("mcGenWeight()")

if wztth == True:
    tree.variables.mctruth = cms.string("mcHiggsProd()")
    tree.variables.mcHWWdecay = cms.string("getWWdecayMC()")

if doSusy == True :
    tree.variables.susyMstop = cms.string("getSusyStopMass()")
    tree.variables.susyMLSP = cms.string("getSusyLSPMass()")

if doHiggs == True :
    tree.variables.MHiggs = cms.string("getHiggsMass()")
    tree.variables.PtHiggs = cms.string("getHiggsPt()")
    tree.variables.HEPMCweight = cms.string("HEPMCweight()")

if doPDFvar == True :
    tree.variables.pdfscalePDF = cms.string("getPDFscalePDF()")
    tree.variables.pdfx1 = cms.string("getPDFx1()")
    tree.variables.pdfx2 = cms.string("getPDFx2()")
    tree.variables.pdfid1 = cms.string("getPDFid1()")
    tree.variables.pdfid2 = cms.string("getPDFid2()")
    tree.variables.pdfx1PDF = cms.string("getPDFx1PDF()")
    tree.variables.pdfx2PDF = cms.string("getPDFx2PDF()")

# bTagging variables
if doBTag: addBTaggingVariables(tree, 99999)

# jet additional variables: e.g. pu-jetid
addJetsVariables(tree, 99999)


# lepton variables needed for fake rate studies, measurement and application: e.g. jet variables for jet closest to lepton
addFakeRateVariables(tree)



setattr(process,"Tree", tree)
seq += tree


# path already set up
p = getattr(process,'sel'+labelSetup)
p += seq
setattr(process,'sel'+labelSetup,p)

# define output
process.TFileService = cms.Service("TFileService",fileName = cms.string(options.outputFile))


#####################################
## counter of all events you run on
##   for MC in case you some crab jobs fail,
##   it is necessary to keep track of how many
##   events we run on
process.AllEvents = cms.EDFilter("AllPassFilter")
process.counterPath = cms.Path(process.AllEvents)
##
##

#####################################
## Trigger dumper
##   dump trigger rates
process.TriggerAnalyzer = cms.EDAnalyzer("MiniAODTriggerAnalyzer",
      bits = cms.InputTag("TriggerResults","","HLT")
      )
process.TriggerAnalyzerPath = cms.Path(process.TriggerAnalyzer)
##
##

#####################################
## Weights dumper
##   dump weights with NO selections applied
process.WeightDumperAnalyzer = cms.EDAnalyzer('WeightDumper',
     mcLHEEventInfoTag  = cms.InputTag(options.LHEweightSource),
     mcLHERunInfoTag    = cms.InputTag(options.LHERunInfo),
     #mcLHEEventInfoTag  = cms.InputTag("externalLHEProducer"),
     #mcLHEEventInfoTag = cms.InputTag("source"),
     genEvtInfoTag      = cms.InputTag("generator"), 
     debug              = cms.untracked.bool(False)
  )

if isMC and doLHE :
  #print "             ------------------------> running WeightDumperAnalyzer"
  process.WeightDumperAnalyzerPath = cms.Path(process.WeightDumperAnalyzer)
##
##


#####################################
## pileup dumper
##   dump pileup with NO selections applied
process.PileUpDumperAnalyzer = cms.EDAnalyzer('PileUpDumper',
    puLabel = cms.InputTag(options.puInformation),  # v1 miniaod addPileupInfo 
    debug   = cms.untracked.bool(False)
  )

if isMC :
  process.PileUpDumperAnalyzerPath = cms.Path(process.PileUpDumperAnalyzer)


#if IsoStudy:
  ##getattr(process,"ww%s%s"% (X,labelSetup)).elTag = "wwEleIDMerge"
  ##getattr(process,"ww%s%s"% (X,labelSetup)).muTag = "wwMuonsMergeID"
  #getattr(process,"Tree").cut = cms.string("!isSTA(0) && !isSTA(1) && leptEtaCut(2.4,2.5) && ptMax > 20 && ptMin > 10 && passesIP && nExtraLep(10) == 0")
  ##prepend = process.isoStudySequence + process.wwEleIDMerge + process.wwMuonsMergeID
  ##getattr(process,"sel%s%s"% (X,labelSetup))._seq = prepend + getattr(process,"sel%s%s"% (X,labelSetup))._seq


if doSameSign:
    getattr(process,"Tree").cut = cms.string("q(0)*q(1) > 0 && !isSTA(0) && !isSTA(1) && leptEtaCut(2.4,2.5) && ptMax > 20 && ptMin > 10")

if options.doSoftActivity: 
    addSoftActivityVariables(process,tree)

# Save events
if doNoFilter:
    print ">> Accept all events"
    getattr(process,"skim%s"% (labelSetup)).cut = cms.string("nLep >= 0")
else :
    if doCut == "1" :
        print ">> Accept events with nLep >= 1"
        getattr(process,"skim%s"% (labelSetup)).cut = cms.string("nLep >= 1")
    else :
        print ">> Accept events with ", doCut
     

# TTree producer ends up in the endPath, then it's NOT a filter anymore
# then applying a cut here has no effect
# then I leave "1" by default, not to create confusion
getattr(process,"Tree").cut = cms.string("1")
#################
# very important!
# needed otherwise the "unschedule" approach does not work
#process.myoutputstep = cms.EndPath(process.outTemp+process.Tree)
process.myoutputstep = cms.EndPath(process.Tree)
#################

####
# remove complain about "filter in endpath" ...
# this command is not required, but removes annoying message
# cms.ignore(process.Tree)
####

#
# dump cfg file: 
# to do it do: python stepB.py
# decomment *if* needed
#
#processDumpFile = open('processDump.py', 'w')
#print >> processDumpFile, process.dumpPython()
