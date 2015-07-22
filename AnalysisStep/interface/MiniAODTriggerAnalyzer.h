// system include files
#include <memory>
#include <cmath>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"

//---- credits to
//----   https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2015#Trigger
//----   then modified for our needs!

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"



class MiniAODTriggerAnalyzer : public edm::EDAnalyzer {
 
public:
 explicit MiniAODTriggerAnalyzer(const edm::ParameterSet&);
 ~MiniAODTriggerAnalyzer() {}
 
private:
 virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
 
 edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
 edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjects_;
 edm::EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescales_;
 
 TH1F* _totalEventsTriggers; 
 std::vector <std::string> _pathNames;
 
};

