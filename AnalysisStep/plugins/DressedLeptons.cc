// -*- C++ -*-
//
// Package:    CommonTools/DressedLeptons
// Class:      DressedLeptons
// 
/**\class DressedLeptons DressedLeptons.cc CommonTools/DressedLeptons/plugins/DressedLeptons.cc

   Description: [one line class summary]

   Implementation:
   [Notes on implementation]
*/
//
// Original Author:  Lorenzo Russo
//         Created:  Wed, 06 May 2015 15:44:22 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"

using namespace reco;

//
// class declaration
//come se fosse il .h file

class DressedLeptons : public edm::EDProducer {
public:
  explicit DressedLeptons(const edm::ParameterSet&);
  ~DressedLeptons();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() override;
  virtual void produce(edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
      
  int PdgId_;
  double DeltaR_;
  edm::InputTag collection_name_;

  //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

  // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
DressedLeptons::DressedLeptons(const edm::ParameterSet& iConfig)// l'oggetto iCinfig è istanza della classe ParameterSet 
{

  PdgId_ =( iConfig.getParameter<int>("PdgId") ); //copio le variabili di input "XXX" nella variabile privata XXX_
  DeltaR_ =( iConfig.getParameter<double>("DeltaR") );
  collection_name_=( iConfig.getParameter<edm::InputTag>("genParticles") );
  produces<std::vector<CompositeCandidate> > (""); //output in uscita: vettore di CompisiteCandidate
    


}


DressedLeptons::~DressedLeptons()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
DressedLeptons::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  // get GEN Particle candidates
  Handle<GenParticleCollection> genParticles;
  iEvent.getByLabel(collection_name_, genParticles);//"prunedGenParticles
  

  std::vector<const GenParticle*> leptons; 
  std::vector<const GenParticle*> photons;
  math::XYZTLorentzVector dl;


  //loop over gen particles and find all leptons and photons (status 1)
  for(size_t i = 0; i < genParticles->size(); ++ i) {
    const GenParticle & p = (*genParticles)[i];
    int id = p.pdgId();
    if(  fabs(id)== fabs(PdgId_)  &&  p.status()==1  ){ 
      leptons.push_back(&p); 
    }
    if(id == 22 &&  p.status()==1){
      photons.push_back(&p);    
    }
  }   
  
 

  std::auto_ptr< std::vector<CompositeCandidate> > vec_cc_Out(new std::vector<CompositeCandidate>);
 
  //logic is:
  //1) loop over leptons
  //2) find photons in a cone around the leptons
  //3) for every photon in the cone, check that the current lepton is the closest lepton (i.e.) that there is not anoother lepton closer to the photon
  //4) if 3) attach the photon to the lepton

  //1) loop over leptons
  for(size_t i = 0; i<leptons.size();++i  ){
    CompositeCandidate aa(*leptons[i]);
    
    for(size_t j = 0; j<photons.size();++j  ){
      double dr= deltaR( *leptons[i],*photons[j]);
      //2) find photons in a cone around the leptons
      if( dr < DeltaR_  ){
	int c=0;
	
        // now loop again over leptons and count increment a counter for every lepton that is farer from the lepton than the current one
        for(size_t k = 0; k<leptons.size();++k  ){
	  if( deltaR( *leptons[i],*photons[j]) < deltaR( *leptons[k],*photons[j])){
	    c++;    	   
	  }
	}
        // if the counter is equals to leptons.size() - 1 then our lepton is the closest to the photon so we attach it 
	//std::cout<<"------------------------------------------" <<std::endl; 
	if(c==( fabs(leptons.size()) -1 )){

	  aa.addDaughter(  *photons[j]  );
	  aa.setP4(  (((*leptons[i]).p4())+((*photons[j]).p4()))  );
	}
      }
      
	
    }
    vec_cc_Out->push_back(  aa  );     
  }//for leptons
  
  
  iEvent.put(vec_cc_Out);
  
    
}



/* This is an event example
//Read 'ExampleData' from the Event
Handle<ExampleData> pIn;
iEvent.getByLabel("example",pIn);

//Use the ExampleData to create an ExampleData2 which 
// is put into the Event
std::unique_ptr<ExampleData2> pOut(new ExampleData2(*pIn));
iEvent.put(std::move(pOut));
*/

/* this is an EventSetup example
//Read SetupData from the SetupRecord in the EventSetup
ESHandle<SetupData> pSetup;
iSetup.get<SetupRecord>().get(pSetup);
*/
 


// ------------ method called once each job just before starting event loop  ------------
void 
DressedLeptons::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DressedLeptons::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
  void
  DressedLeptons::beginRun(edm::Run const&, edm::EventSetup const&)
  {
  }
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
  void
  DressedLeptons::endRun(edm::Run const&, edm::EventSetup const&)
  {
  }
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
  void
  DressedLeptons::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
  {
  }
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
  void
  DressedLeptons::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
  {
  }
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DressedLeptons::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(DressedLeptons);
