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
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
// #include "PhysicsTools/TagAndProbe/interface/BaseTreeFiller.h"
#include "LatinoTrees/AnalysisStep/interface/BaseGenericTreeFiller.h"
#include <set>
#include "FWCore/ParameterSet/interface/Registry.h"

class GenericTreeProducer : public edm::EDFilter {
  public:
    explicit GenericTreeProducer(const edm::ParameterSet&);
    ~GenericTreeProducer();

  private:
    virtual bool filter(edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

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
  probeFiller_(new tnp::BaseGenericTreeFiller("probe_tree", iConfig, consumesCollector(), (int32_t) (iConfig.existsAs<int32_t>("maxVectorsLength") ? iConfig.getParameter<int32_t>("maxVectorsLength") : 10)  ))
{
}

GenericTreeProducer::~GenericTreeProducer(){
}

bool GenericTreeProducer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup){
  bool result = !filter_;
  edm::Handle<reco::CandidateView> probes;
  iEvent.getByToken(probesToken_, probes);
  if(!probes.isValid()) return result;
  probeFiller_->init(iEvent);

//   std::cout << " GenericTreeProducer::filter --> init" << std::endl;
  std::vector<reco::CandidateBaseRef> selectedProbes;
  for (size_t i = 0; i < probes->size(); ++i){
//    std::cout << " GenericTreeProducer::filter --> i = " << i << " :: " << probes->size() -1  << std::endl;
   const reco::CandidateBaseRef &probe = probes->refAt(i);
   if(cut_(*probe)){
    selectedProbes.push_back(probe);
   }
  }
  
  
  // select probes and calculate the sorting value
  //   typedef std::pair<reco::CandidateBaseRef, double> Pair;
  //   std::vector<Pair> selectedProbes;
  //   for (size_t i = 0; i < probes->size(); ++i){
  //     const reco::CandidateBaseRef &probe = probes->refAt(i);
  //     if(cut_(*probe)){
  //       selectedProbes.push_back(Pair(probe, sortFunction_(*probe)));
  //     }
  //   }
  // sort only if a function was provided
  //   if(sortDescendingBy_.size()>0) sort(selectedProbes.begin(), selectedProbes.end(), boost::bind(&Pair::second, _1) > boost::bind(&Pair::second, _2));
  // fill the first maxProbes_ into the tree
  //   for (size_t i = 0; i < (maxProbes_<0 ? selectedProbes.size() : std::min((size_t)maxProbes_, selectedProbes.size())); ++i){
  //    probeFiller_->fill(selectedProbes[i].first);
  //    result = true;
  //   }
  
//   std::cout << " GenericTreeProducer::filter :: maxProbes_ = " << maxProbes_ << std::endl;
//   std::cout << " GenericTreeProducer::filter :: selectedProbes.size() [after cuts] = " << selectedProbes.size() << std::endl;
  for (size_t i = 0; i < (maxProbes_<0 ? selectedProbes.size() : std::min((size_t)maxProbes_, selectedProbes.size())); ++i){
//    std::cout << " GenericTreeProducer::filter --> i = " << i << " :: " << probes->size() -1 << " filling ... " << std::endl;
   probeFiller_->fill(selectedProbes[i]);
//    std::cout << " GenericTreeProducer::filter --> i = " << i << " :: " << probes->size() -1 << " filled " << std::endl;
   result = true;
  }
  return result;
 
}

void GenericTreeProducer::endJob(){
    // ask to write the current PSet info into the TTree header
    probeFiller_->writeProvenance(edm::getProcessParameterSet());
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenericTreeProducer);


