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

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

class PileUpMultiplicityCounter : public edm::EDProducer {
    public:
        explicit PileUpMultiplicityCounter(const edm::ParameterSet&);
        ~PileUpMultiplicityCounter();

    private:
        virtual void produce(edm::Event&, const edm::EventSetup&);
        edm::InputTag puTag_;
};

PileUpMultiplicityCounter::PileUpMultiplicityCounter(const edm::ParameterSet& iConfig) :
    puTag_(iConfig.getParameter<edm::InputTag>("puLabel"))
{
     produces<float> ("tr");
     produces<float> ("it");
     produces<float> ("m1");
     produces<float> ("p1");
}


void PileUpMultiplicityCounter::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
    std::auto_ptr<float> tr(new float(0));
    std::auto_ptr<float> it(new float(0));
    std::auto_ptr<float> m1(new float(0));
    std::auto_ptr<float> p1(new float(0));

    edm::Handle<std::vector<PileupSummaryInfo> > puInfoH;
    iEvent.getByLabel(puTag_,puInfoH);
    for(size_t i=0;i<puInfoH->size();++i) {
        if( puInfoH->at(i).getBunchCrossing()==0 ) *tr = puInfoH->at(i).getTrueNumInteractions();
        if( puInfoH->at(i).getBunchCrossing()==0 ) *it = puInfoH->at(i).getPU_NumInteractions();
        if( puInfoH->at(i).getBunchCrossing()==-1) *m1 = puInfoH->at(i).getPU_NumInteractions();
        if( puInfoH->at(i).getBunchCrossing()==1 ) *p1 = puInfoH->at(i).getPU_NumInteractions();
    }

    iEvent.put(tr,"tr");
    iEvent.put(it,"it");
    iEvent.put(p1,"p1");
    iEvent.put(m1,"m1");
}

PileUpMultiplicityCounter::~PileUpMultiplicityCounter() { }
DEFINE_FWK_MODULE(PileUpMultiplicityCounter);