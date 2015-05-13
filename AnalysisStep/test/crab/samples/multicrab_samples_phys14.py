

config.General.requestName = 'DYll'
config.Data.inputDataset = '/DYJetsToLL_M-50_13TeV-madgraph-pythia8-tauola_v2/Phys14DR-AVE30BX50_tsg_PHYS14_ST_V1-v1/MINIAODSIM'
config.JobType.pyCfgParams = list(pyCfgParams)
config.JobType.pyCfgParams.extend(['label=DYll', 'id=12345', 'scale=1.23'])
submit(config)

config.General.requestName = 'DYll50ns'
config.Data.inputDataset = '/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Phys14DR-PU4bx50_PHYS14_25_V1-v1/MINIAODSIM'
config.JobType.pyCfgParams = list(pyCfgParams)
config.JobType.pyCfgParams.extend(['label=DYll50ns', 'id=12345', 'scale=2.13'])
submit(config)
