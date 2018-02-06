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
        ~PileUpMultiplicityCounter();

    private:
        virtual void produce(edm::Event&, const edm::EventSetup&);
        edm::InputTag puTag_;
        edm::InputTag src_;
        edm::EDGetTokenT<std::vector<PileupSummaryInfo> > puSummaryT_ ;
        edm::EDGetTokenT<edm::View<reco::Candidate> > recoCandidateT_ ;
};

PileUpMultiplicityCounter::PileUpMultiplicityCounter(const edm::ParameterSet& iConfig) :
    puTag_(iConfig.getParameter<edm::InputTag>("puLabel")),
    src_  (iConfig.getParameter<edm::InputTag>("src"))
{

  puSummaryT_     = consumes<std::vector<PileupSummaryInfo> >(puTag_);
  recoCandidateT_ = consumes<edm::View<reco::Candidate> >(src_);  

  produces<edm::ValueMap<float> > ("tr");
  produces<edm::ValueMap<float> > ("it");
  produces<edm::ValueMap<float> > ("m1"); //-- -1 BX
  produces<edm::ValueMap<float> > ("p1"); //-- +1 BX
  produces<edm::ValueMap<float> > ("m2"); //-- -2 BX
  produces<edm::ValueMap<float> > ("p2"); //-- +2 BX
  produces<edm::ValueMap<float> > ("m3"); //-- -3 BX
  produces<edm::ValueMap<float> > ("p3"); //-- +3 BX
  produces<edm::ValueMap<float> > ("m4"); //-- -4 BX
  produces<edm::ValueMap<float> > ("m5"); //-- -5 BX
}


void PileUpMultiplicityCounter::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

 edm::Handle<std::vector<PileupSummaryInfo> > puInfoH;
 iEvent.getByToken(puSummaryT_,puInfoH);

 edm::Handle<edm::View<reco::Candidate> > srcH;
 iEvent.getByToken(recoCandidateT_,srcH);

 std::auto_ptr<float> tr(new float(0));
 std::auto_ptr<float> it(new float(0));
 std::auto_ptr<float> m1(new float(0));
 std::auto_ptr<float> p1(new float(0));
 std::auto_ptr<float> m2(new float(0));
 std::auto_ptr<float> p2(new float(0));
 std::auto_ptr<float> m3(new float(0));
 std::auto_ptr<float> p3(new float(0));
 std::auto_ptr<float> m4(new float(0));
 std::auto_ptr<float> m5(new float(0));
 
 for(size_t i=0;i<puInfoH->size();++i) {
  if( puInfoH->at(i).getBunchCrossing()==0 ) *tr = puInfoH->at(i).getTrueNumInteractions();
  if( puInfoH->at(i).getBunchCrossing()==0 ) *it = puInfoH->at(i).getPU_NumInteractions();
  if( puInfoH->at(i).getBunchCrossing()==-1) *m1 = puInfoH->at(i).getPU_NumInteractions();
  if( puInfoH->at(i).getBunchCrossing()==1 ) *p1 = puInfoH->at(i).getPU_NumInteractions();
  if( puInfoH->at(i).getBunchCrossing()==-2) *m2 = puInfoH->at(i).getPU_NumInteractions();
  if( puInfoH->at(i).getBunchCrossing()==2 ) *p2 = puInfoH->at(i).getPU_NumInteractions();
  if( puInfoH->at(i).getBunchCrossing()==-3) *m3 = puInfoH->at(i).getPU_NumInteractions();
  if( puInfoH->at(i).getBunchCrossing()==3 ) *p3 = puInfoH->at(i).getPU_NumInteractions();
  if( puInfoH->at(i).getBunchCrossing()==-4) *m4 = puInfoH->at(i).getPU_NumInteractions();
  if( puInfoH->at(i).getBunchCrossing()==-5) *m5 = puInfoH->at(i).getPU_NumInteractions();
 }

 std::unique_ptr<edm::ValueMap<float> > trMap(new edm::ValueMap<float>);
 std::unique_ptr<edm::ValueMap<float> > itMap(new edm::ValueMap<float>);
 std::unique_ptr<edm::ValueMap<float> > m1Map(new edm::ValueMap<float>);
 std::unique_ptr<edm::ValueMap<float> > p1Map(new edm::ValueMap<float>);
 std::unique_ptr<edm::ValueMap<float> > m2Map(new edm::ValueMap<float>);
 std::unique_ptr<edm::ValueMap<float> > p2Map(new edm::ValueMap<float>);
 std::unique_ptr<edm::ValueMap<float> > m3Map(new edm::ValueMap<float>);
 std::unique_ptr<edm::ValueMap<float> > p3Map(new edm::ValueMap<float>);
 std::unique_ptr<edm::ValueMap<float> > m4Map(new edm::ValueMap<float>);
 std::unique_ptr<edm::ValueMap<float> > m5Map(new edm::ValueMap<float>);
 
 edm::ValueMap<float>::Filler trFill(*trMap);
 edm::ValueMap<float>::Filler itFill(*itMap);
 edm::ValueMap<float>::Filler m1Fill(*m1Map);
 edm::ValueMap<float>::Filler p1Fill(*p1Map);
 edm::ValueMap<float>::Filler m2Fill(*m2Map);
 edm::ValueMap<float>::Filler p2Fill(*p2Map);
 edm::ValueMap<float>::Filler m3Fill(*m3Map);
 edm::ValueMap<float>::Filler p3Fill(*p3Map);
 edm::ValueMap<float>::Filler m4Fill(*m4Map);
 edm::ValueMap<float>::Filler m5Fill(*m5Map);
 
 std::vector<float> trVec(srcH->size(),*tr);
 std::vector<float> itVec(srcH->size(),*it);
 std::vector<float> m1Vec(srcH->size(),*m1);
 std::vector<float> p1Vec(srcH->size(),*p1);
 std::vector<float> m2Vec(srcH->size(),*m2);
 std::vector<float> p2Vec(srcH->size(),*p2);
 std::vector<float> m3Vec(srcH->size(),*m3);
 std::vector<float> p3Vec(srcH->size(),*p3);
 std::vector<float> m4Vec(srcH->size(),*m4);
 std::vector<float> m5Vec(srcH->size(),*m5);
 
 trFill.insert(srcH, trVec.begin(), trVec.end());
 itFill.insert(srcH, itVec.begin(), itVec.end());
 m1Fill.insert(srcH, m1Vec.begin(), m1Vec.end());
 p1Fill.insert(srcH, p1Vec.begin(), p1Vec.end());
 m2Fill.insert(srcH, m2Vec.begin(), m2Vec.end());
 p2Fill.insert(srcH, p2Vec.begin(), p2Vec.end());
 m3Fill.insert(srcH, m3Vec.begin(), m3Vec.end());
 p3Fill.insert(srcH, p3Vec.begin(), p3Vec.end());
 m4Fill.insert(srcH, m4Vec.begin(), m4Vec.end());
 m5Fill.insert(srcH, m5Vec.begin(), m5Vec.end());
 
 trFill.fill();
 itFill.fill();
 m1Fill.fill();
 p1Fill.fill();
 m2Fill.fill();
 p2Fill.fill();
 m3Fill.fill();
 p3Fill.fill();
 m4Fill.fill();
 m5Fill.fill();
 
 iEvent.put(std::move(trMap),"tr");
 iEvent.put(std::move(itMap),"it");
 iEvent.put(std::move(p1Map),"p1");
 iEvent.put(std::move(m1Map),"m1");
 iEvent.put(std::move(p2Map),"p2");
 iEvent.put(std::move(m2Map),"m2");
 iEvent.put(std::move(p3Map),"p3");
 iEvent.put(std::move(m3Map),"m3");
 iEvent.put(std::move(m4Map),"m4");
 iEvent.put(std::move(m5Map),"m5");
 
}

PileUpMultiplicityCounter::~PileUpMultiplicityCounter() { }
DEFINE_FWK_MODULE(PileUpMultiplicityCounter);
