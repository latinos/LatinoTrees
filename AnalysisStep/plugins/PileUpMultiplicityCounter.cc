#include <memory>
#include <vector>
#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

class PileUpMultiplicityCounter : public edm::EDProducer {
    public:
        explicit PileUpMultiplicityCounter(const edm::ParameterSet&);
        virtual void produce(edm::Event&, const edm::EventSetup&);
        ~PileUpMultiplicityCounter();

    private:
        edm::InputTag puTag_;
        edm::InputTag src_;
        edm::EDGetTokenT<std::vector<PileupSummaryInfo> > puSummaryT_ ;
        edm::EDGetTokenT<edm::View<reco::Candidate> > recoCandidateT_ ;
};

PileUpMultiplicityCounter::PileUpMultiplicityCounter(const edm::ParameterSet& iConfig){
  
  if(iConfig.exists("puLabel")) puTag_ = iConfig.getParameter<edm::InputTag>("puLabel");
  else throw cms::Exception("puLabel missing tag ");
  if(iConfig.exists("src"))     src_ = iConfig.getParameter<edm::InputTag>("src");
  else throw cms::Exception("src missing tag ");

  puSummaryT_     = consumes<std::vector<PileupSummaryInfo> >(puTag_);
  recoCandidateT_ = consumes<edm::View<reco::Candidate> >(src_);  
  
  produces<edm::ValueMap<float> > ("tr");
  produces<edm::ValueMap<float> > ("it");
  produces<edm::ValueMap<float> > ("m1");
  produces<edm::ValueMap<float> > ("p1");
}


void PileUpMultiplicityCounter::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

 edm::Handle<std::vector<PileupSummaryInfo> > puInfoH;
 iEvent.getByToken(puSummaryT_,puInfoH);
 if(!puInfoH.isValid()) throw cms::Exception("bad puInfoH");

 edm::Handle<edm::View<reco::Candidate> > srcH;
 iEvent.getByToken(recoCandidateT_,srcH);
 if(!puInfoH.isValid()) throw cms::Exception("bad recoCandidate inputs");

 std::auto_ptr<float> tr(new float(0));
 std::auto_ptr<float> it(new float(0));
 std::auto_ptr<float> m1(new float(0));
 std::auto_ptr<float> p1(new float(0));

 for(size_t i=0;i < puInfoH->size();++i) {
  if( puInfoH->at(i).getBunchCrossing()==0 ) *tr = puInfoH->at(i).getTrueNumInteractions();
  if( puInfoH->at(i).getBunchCrossing()==0 ) *it = puInfoH->at(i).getPU_NumInteractions();
  if( puInfoH->at(i).getBunchCrossing()==-1) *m1 = puInfoH->at(i).getPU_NumInteractions();
  if( puInfoH->at(i).getBunchCrossing()==1 ) *p1 = puInfoH->at(i).getPU_NumInteractions();
 }

 std::auto_ptr<edm::ValueMap<float> > trMap(new edm::ValueMap<float>);
 std::auto_ptr<edm::ValueMap<float> > itMap(new edm::ValueMap<float>);
 std::auto_ptr<edm::ValueMap<float> > m1Map(new edm::ValueMap<float>);
 std::auto_ptr<edm::ValueMap<float> > p1Map(new edm::ValueMap<float>);

 edm::ValueMap<float>::Filler trFill(*trMap);
 edm::ValueMap<float>::Filler itFill(*itMap);
 edm::ValueMap<float>::Filler m1Fill(*m1Map);
 edm::ValueMap<float>::Filler p1Fill(*p1Map);

 std::vector<float> trVec(srcH->size(),*tr);
 std::vector<float> itVec(srcH->size(),*it);
 std::vector<float> m1Vec(srcH->size(),*m1);
 std::vector<float> p1Vec(srcH->size(),*p1);

 trFill.insert(srcH, trVec.begin(), trVec.end());
 itFill.insert(srcH, itVec.begin(), itVec.end());
 m1Fill.insert(srcH, m1Vec.begin(), m1Vec.end());
 p1Fill.insert(srcH, p1Vec.begin(), p1Vec.end());

 trFill.fill();
 itFill.fill();
 m1Fill.fill();
 p1Fill.fill();

 iEvent.put(trMap,"tr");
 iEvent.put(itMap,"it");
 iEvent.put(p1Map,"p1");
 iEvent.put(m1Map,"m1");

}

PileUpMultiplicityCounter::~PileUpMultiplicityCounter() { }
DEFINE_FWK_MODULE(PileUpMultiplicityCounter);
