

# to add new samples:
#
#    human readable name                               sample name from DBS                                                   options: label, id, scale, ...
samples['DYll']     = ['/DYJetsToLL_M-50_13TeV-madgraph-pythia8-tauola_v2/Phys14DR-AVE30BX50_tsg_PHYS14_ST_V1-v1/MINIAODSIM', ['label=DYll',     'id=12345', 'scale=1.23']]
samples['DYll50ns'] = ['/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Phys14DR-PU4bx50_PHYS14_25_V1-v1/MINIAODSIM',                 ['label=DYll50ns', 'id=12345', 'scale=2.13']]


# if some global configurations needs to be changed, un-comment below:

# local folder name
#config.General.requestName = 'MCtest_13May2015'
#config.Data.unitsPerJob = 10   # since files based, 10 files per job
#config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/'
#config.Data.splitting = 'FileBased'    #'LumiBased'
#config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshww/amassiro/RunII/test/'
#config.section_('Site')
#config.Site.storageSite = 'T2_CH_CERN'


