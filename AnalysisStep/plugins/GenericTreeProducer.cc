// -*- C++ -*-
//
// Package:    GenericTreeProducer
// Class:      GenericTreeProducer
//
/**\class GenericTreeProducer GenericTreeProducer.cc

 Description: TTree producer based on input probe parameters

 Implementation:
     <Notes on implementation>
*/

#include <memory>
#include <ctype.h>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "LatinoTrees/AnalysisStep/interface/BaseGenericTreeFiller.h"
#include <set>
#include "FWCore/ParameterSet/interface/Registry.h"

class GenericTreeProducer : public edm::EDAnalyzer {
  public:
    explicit GenericTreeProducer(const edm::ParameterSet&);
    ~GenericTreeProducer();
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void beginJob();
    virtual void endJob();

  private:

    /// InputTag to the collection of all probes
    edm::EDGetTokenT<reco::CandidateView> probesToken_;

    /// The selector object
    StringCutObjectSelector<reco::Candidate, true> cut_;

    /// Specifies whether this module should filter
    bool filter_;

    /// Name of the reco::Candidate function used for sorting
    std::string sortDescendingBy_;

    /// The StringObjectFunction itself
    StringObjectFunction<reco::Candidate, true> sortFunction_;

    /// The number of first probes used to fill the tree
    int32_t maxProbes_;

    /// The object that actually computes variables and fills the tree for the probe
    std::auto_ptr<tnp::BaseGenericTreeFiller> probeFiller_;
};

GenericTreeProducer::GenericTreeProducer(const edm::ParameterSet& iConfig) :
  probesToken_(consumes<reco::CandidateView>(iConfig.getParameter<edm::InputTag>("src"))),
  cut_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : ""),
  filter_(iConfig.existsAs<bool>("filter") ? iConfig.getParameter<bool>("filter") : false),
  sortDescendingBy_(iConfig.existsAs<std::string>("sortDescendingBy") ? iConfig.getParameter<std::string>("sortDescendingBy") : ""),
  sortFunction_(sortDescendingBy_.size()>0 ? sortDescendingBy_ : "pt"), //need to pass a valid default
  maxProbes_(iConfig.existsAs<int32_t>("maxProbes") ? iConfig.getParameter<int32_t>("maxProbes") : -1),
  probeFiller_(new tnp::BaseGenericTreeFiller("probe_tree", iConfig, consumesCollector()))
{
}

GenericTreeProducer::~GenericTreeProducer(){
}

void GenericTreeProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  edm::Handle<reco::CandidateView> probes;
  iEvent.getByToken(probesToken_, probes);
  if(!probes.isValid()) return ;
  probeFiller_->init(iEvent);

  std::vector<reco::CandidateBaseRef> selectedProbes;
  for (size_t i = 0; i < probes->size(); ++i){
   const reco::CandidateBaseRef &probe = probes->refAt(i);
   if(cut_(*probe)){
    selectedProbes.push_back(probe);
   }
  }
  
  
  for (size_t i = 0; i < (maxProbes_<0 ? selectedProbes.size() : std::min((size_t)maxProbes_, selectedProbes.size())); ++i){
   probeFiller_->fill(selectedProbes[i]);
  }
  return ;
 
}

void GenericTreeProducer::endJob(){
    // ask to write the current PSet info into the TTree header
    probeFiller_->writeProvenance(edm::getProcessParameterSet());
}

void GenericTreeProducer::beginJob(){}

//define this as a plug-in
DEFINE_FWK_MODULE(GenericTreeProducer);


