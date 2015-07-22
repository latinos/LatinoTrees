
#include "LatinoTrees/AnalysisStep/interface/MiniAODTriggerAnalyzer.h"


MiniAODTriggerAnalyzer::MiniAODTriggerAnalyzer(const edm::ParameterSet& iConfig):
   triggerBits_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bits")))
//    triggerObjects_(consumes<pat::TriggerObjectStandAloneCollection>(iConfig.getParameter<edm::InputTag>("objects"))),
//    triggerPrescales_(consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<edm::InputTag>("prescales")))
{
 edm::Service<TFileService> fs;
 _totalEventsTriggers = fs -> make<TH1F>("totalEventsTriggers", "totalEventsTriggers", 1000,  0., 1000.);
}

void MiniAODTriggerAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
 edm::Handle<edm::TriggerResults> triggerBits;
//  edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;
//  edm::Handle<pat::PackedTriggerPrescales> triggerPrescales;
 
 iEvent.getByToken(triggerBits_, triggerBits);
//  iEvent.getByToken(triggerObjects_, triggerObjects);
//  iEvent.getByToken(triggerPrescales_, triggerPrescales);
 
 const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);
 for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i) {
  std::string nameTrigger = names.triggerName(i);
  //---- if not found, add to the list
  std::vector<std::string>::iterator name_position;
  name_position = std::find(_pathNames.begin(), _pathNames.end(), nameTrigger);
  int name_int_position = int(name_position - _pathNames.begin());
  
  if (name_int_position<1000) {
   if (name_position == _pathNames.end()) {
    _pathNames.push_back(nameTrigger);
    _totalEventsTriggers->GetXaxis()->SetBinLabel (name_int_position +1, nameTrigger.c_str());
   }
  }
  else {
   name_int_position = 1000;
  }
  
  if (triggerBits->accept(i)) {
   _totalEventsTriggers->Fill( name_int_position+1 , 1);
  }
  
 }
 
}

//define this as a plug-in
DEFINE_FWK_MODULE(MiniAODTriggerAnalyzer);


