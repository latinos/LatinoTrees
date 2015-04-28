import os
from WMCore.Configuration import Configuration
config = Configuration()

pyCfgParams = ['outputFile=stepB_MC.root', 'doNoFilter=True', 'doMuonIsoId=True', 'doGen=True', 'doLHE=False', 'runPUPPISequence=False']

config.section_('General')
config.General.transferLogs = True

config.section_('JobType')
config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = '../stepB.py'
config.JobType.outputFiles = ['stepB_MC.root']

config.section_('Data')    
config.Data.inputDBS        = 'global'
config.Data.splitting       = 'FileBased'
config.Data.unitsPerJob     = 1
config.Data.publishDataName = 'NoFilter'
config.Data.ignoreLocality  = True

config.section_('Site')
config.Site.storageSite = 'T2_ES_IFCA'


if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand

    # Make sure you set this parameter (here or above in the config, it does not matter)
    config.General.workArea = 'crab_projects_28Apr'

    def submit(config):
        res = crabCommand('submit', config = config)

    ####################### Public samples to be analysed ######################
                   
    config.General.requestName = 'QCD'
    config.Data.inputDataset   = '/QCD_Pt-20toInf_MuEnrichedPt15_PionKaonDecay_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v3/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=QCD', 'id=12345', 'scale=79.80534'])
    submit(config)

    config.General.requestName = 'DYJetsToLL'
    config.Data.inputDataset   = '/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=DYJetsToLL', 'id=12345', 'scale=2.12968'])
    submit(config)

    config.General.requestName = 'WJetsToLNu'
    config.Data.inputDataset   = '/WJetsToLNu_13TeV-madgraph-pythia8-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=WJetsToLNu', 'id=12345', 'scale=2.04731'])
    submit(config)

    config.General.requestName = 'WWTo2L2Nu'
    config.Data.inputDataset   = '/WWTo2L2Nu_CT10_13TeV-powheg-pythia8-tauola/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=WWTo2L2Nu', 'id=12345', 'scale=0.01385'])
    submit(config)

    config.General.requestName = 'TTJets'
    config.Data.inputDataset   = '/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=TTJets', 'id=12345', 'scale=0.03269'])
    submit(config)

    config.General.requestName = 'TBarToLeptons_s'
    config.Data.inputDataset   = '/TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=TBarToLeptons_s', 'id=12345', 'scale=0.01664'])
    submit(config)

    config.General.requestName = 'TToLeptons_s'
    config.Data.inputDataset   = '/TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=TToLeptons_s', 'id=12345', 'scale=0.01440'])
    submit(config)

    config.General.requestName = 'TBarToLeptons_t'
    config.Data.inputDataset   = '/TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=TBarToLeptons_t', 'id=12345', 'scale=0.32380'])
    submit(config)

    config.General.requestName = 'TToLeptons_t'
    config.Data.inputDataset   = '/TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=TToLeptons_t', 'id=12345', 'scale=0.27204'])
    submit(config)

    config.General.requestName = 'gg_HWW'
    config.Data.inputDataset   = '/GluGluToHToWWTo2LAndTau2Nu_M-125_13TeV-powheg-pythia6/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=gg_HWW', 'id=12345', 'scale=0.00996'])
    submit(config)

    config.General.requestName = 'VBF_HWW'
    config.Data.inputDataset   = '/VBF_HToWWToLAndTauNuQQ_M-125_13TeV-powheg-pythia6/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=VBF_HWW', 'id=12345', 'scale=0.00089'])
    submit(config)

    config.General.requestName = 'VH_HWW'
    config.Data.inputDataset   = '/WH_ZH_HToWW_2Or3WToLNuAndTau_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=VH_HWW', 'id=12345', 'scale=0.00030'])
    submit(config)

    ####################### Private samples to be analysed #####################

    config.Data.inputDBS       = 'phys03'
    config.Data.ignoreLocality = False

    config.General.requestName = 'Higgs_hzpzp_ww_1GeV'
    config.Data.inputDataset   = '/CRAB_UserFiles/dburns-Higgs_hzpzp_ww_1GeV_13TeV_RECO_v1-7d492cb64f2cdaff326f939f96e45c96/USER'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=Higgs_hzpzp_ww_1GeV', 'id=12345', 'scale=0.00031'])
    submit(config)

    config.General.requestName = 'Higgs_hzpzp_ww_100GeV'
    config.Data.inputDataset   = '/CRAB_UserFiles/dburns-Higgs_hzpzp_ww_100GeV_13TeV_RECO_v1-7d492cb64f2cdaff326f939f96e45c96/USER'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=Higgs_hzpzp_ww_100GeV', 'id=12345', 'scale=0.00031'])
    submit(config)

    config.General.requestName = 'ppTOzh_zTO2v_hTOwwTO2l2v'
    config.Data.inputDataset   = '/CRAB_UserFiles/dburns-ppTOzh_zTO2v_hTOwwTO2l2v_13TeV_AODSIM_v1-7d492cb64f2cdaff326f939f96e45c96/USER'
    config.JobType.pyCfgParams = list(pyCfgParams)
    config.JobType.pyCfgParams.extend(['label=ppTOzh_zTO2v_hTOwwTO2l2v', 'id=12345', 'scale=0.00393'])
    submit(config)
