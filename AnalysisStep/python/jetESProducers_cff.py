import FWCore.ParameterSet.Config as cms

from CondCore.DBCommon.CondDBCommon_cfi import *
from CondCore.DBCommon.CondDBSetup_cfi import *

jec = cms.ESSource("PoolDBESSource",
                   CondDBSetup,
                   connect = cms.string('sqlite_file:PhaseII_Shashlik140PU.db'),
                   toGet = cms.VPSet(
                     cms.PSet(record = cms.string('JetCorrectionsRecord'),
                              tag    = cms.string('JetCorrectorParametersCollection_PhaseII_Shashlik140PU_AK4PFchs'),
                              label  = cms.untracked.string('AK4PFchs')
                              )), 
)

## add an es_prefer statement to resolve a possible conflict from simultaneous connection to a global tag
es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')

