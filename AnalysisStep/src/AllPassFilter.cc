#include "LatinoTrees/AnalysisStep/interface/AllPassFilter.h"

//! ctor
AllPassFilter::AllPassFilter(const edm::ParameterSet& iConfig) {
 edm::Service<TFileService> fs;
 _totalEvents = fs -> make<TH1F>("totalEvents", "totalEvents", 1,  0., 1.);
}

// ----------------------------------------------------------------

//! dtor
AllPassFilter::~AllPassFilter()
{}

// ----------------------------------------------------------------


//! loop over the reco particles and count leptons
bool AllPassFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {
 _totalEvents -> Fill(0.5);
 return true;
}

DEFINE_FWK_MODULE(AllPassFilter);
