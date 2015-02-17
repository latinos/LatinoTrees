// -*- C++ -*-
//
// Package:    LatinoTrees/JetUncorrector
// Class:      JetUncorrector
// 
/**\class JetUncorrector JetUncorrector.cc LatinoTrees/JetUncorrector/plugins/JetUncorrector.cc

 Description: jet uncorrector, it return the "raw" jet collection pT ordered

 Implementation:
     needed for modular dependence of jet calibration.
     The input must be "raw" jet, but this collection is virtually lost in miniAOD.
     It's though possible to access "raw" jets from miniAOD jets, 
     then it's possible to reconstruct a new collection of "raw" jets
   
*/
//
// Original Author:  Andrea Massironi
//         Created:  Tue, 17 Feb 2015 09:06:08 GMT
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

//---- PAT jets
#include "DataFormats/PatCandidates/interface/Jet.h"

//---- JetMet
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CommonTools/Utils/interface/PtComparator.h"

//
// class declaration
//

class JetUncorrector : public edm::EDProducer {
   public:
      explicit JetUncorrector(const edm::ParameterSet&);
      ~JetUncorrector();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

      typedef std::vector<pat::Jet> JetCollection;
      
   private:
      virtual void beginJob() override;
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
      edm::EDGetTokenT<JetCollection> mInput;
      bool mVerbose;
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
JetUncorrector::JetUncorrector(const edm::ParameterSet& fConfig)
   : mInput(consumes<JetCollection>(fConfig.getParameter <edm::InputTag> ("src")))
   , mVerbose (fConfig.getUntrackedParameter <bool> ("verbose", false)) 
{
 
 std::string alias = fConfig.getUntrackedParameter <std::string> ("alias", "");
 if (alias.empty ())
  produces <JetCollection>();
 else
  produces <JetCollection>().setBranchAlias(alias);
}





JetUncorrector::~JetUncorrector() {
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void JetUncorrector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
 
//  jet.jecFactor(0)
 
 edm::Handle<JetCollection> jets;                         //Define Inputs
 iEvent.getByToken (mInput, jets);                        //Get Inputs
 std::auto_ptr<JetCollection> result (new JetCollection); //Corrected jets --> actually the "uncorrected" ones
 typename JetCollection::const_iterator jet;
 for (jet = jets->begin(); jet != jets->end(); jet++) {
//   const pat::Jet* referenceJet = &*jet;
//   int index = jet-jets->begin();
//   edm::RefToBase<reco::Jet> jetRef(edm::Ref<JetCollection>(jets,index));
  if (mVerbose) std::cout<<"JetCorrectionProducer::produce-> original jet: "<<jet->print()<<std::endl;

  //---- get the "raw" jet
  pat::Jet correctedJet = jet->correctedJet(0);
   
  if (mVerbose) std::cout<<"JetUncorrector::produce-> uncorrected jet: " <<correctedJet.print ()<<std::endl;
  
  result->push_back (correctedJet);
 }
 NumericSafeGreaterByPt<pat::Jet> compJets;
 // reorder corrected jets
 std::sort (result->begin (), result->end (), compJets);
 // put corrected jet collection into event
 iEvent.put(result);
 
}

// ------------ method called once each job just before starting event loop  ------------
void JetUncorrector::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void JetUncorrector::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
JetUncorrector::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
JetUncorrector::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
JetUncorrector::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
JetUncorrector::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void JetUncorrector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(JetUncorrector);


