// #include "PhysicsTools/TagAndProbe/interface/BaseTreeFiller.h"
#include "LatinoTrees/AnalysisStep/interface/BaseGenericTreeFiller.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include <chrono>
// #include "PhysicsTools/TagAndProbe/interface/ColinsSoperVariables.h"

#include <TList.h>
#include <TObjString.h>

tnp::ProbeVariable::~ProbeVariable() {}

tnp::ProbeFlag::~ProbeFlag() {}

void tnp::ProbeFlag::init(const edm::Event &iEvent) const {
    if (external_) {
        edm::Handle<edm::View<reco::Candidate> > view;
        iEvent.getByToken(srcToken_, view);
        passingProbes_.clear();
        for (size_t i = 0, n = view->size(); i < n; ++i) passingProbes_.push_back(view->refAt(i));
    }

}

void tnp::ProbeFlag::fill(const reco::CandidateBaseRef &probe) const {
    if (external_) {
        value_ = (std::find(passingProbes_.begin(), passingProbes_.end(), probe) != passingProbes_.end());
    } else {
        value_ = bool(cut_(*probe));
    }
}

tnp::BaseGenericTreeFiller::BaseGenericTreeFiller(const char *name, const edm::ParameterSet& iConfig, edm::ConsumesCollector & iC, int maxStdVector) {
 
    _maxStdVector = maxStdVector;
    
    // make trees as requested
    edm::Service<TFileService> fs;
    tree_ = fs->make<TTree>(name,name);

    // add the branches
    addBranches_(tree_, iConfig, iC, "");

    // set up weights, if needed
    if (iConfig.existsAs<double>("eventWeight")) {
        weightMode_ = Fixed;
        weight_ = iConfig.getParameter<double>("eventWeight");
    } else if (iConfig.existsAs<edm::InputTag>("eventWeight")) {
        weightMode_ = External;
        weightSrcToken_ = iC.consumes<double>(iConfig.getParameter<edm::InputTag>("eventWeight"));
    } else {
        weightMode_ = None;
    }
    if (weightMode_ != None) {
        tree_->Branch("weight", &weight_, "weight/F");
    }

    addRunLumiInfo_ = iConfig.existsAs<bool>("addRunLumiInfo") ? iConfig.getParameter<bool>("addRunLumiInfo") : false;
    if (addRunLumiInfo_) {
         tree_->Branch("run",  &run_,  "run/i");
         tree_->Branch("lumi", &lumi_, "lumi/i");
         tree_->Branch("event", &event_, "event/i");
    }
    addEventVariablesInfo_ = iConfig.existsAs<bool>("addEventVariablesInfo") ? iConfig.getParameter<bool>("addEventVariablesInfo") : false;
    if (addEventVariablesInfo_) {
      recVtxsToken_ = iC.consumes<reco::VertexCollection>(edm::InputTag("offlinePrimaryVertices"));
      beamSpotToken_ = iC.consumes<reco::BeamSpot>(edm::InputTag("offlineBeamSpot"));
      metToken_ = iC.consumes<reco::CaloMETCollection>(edm::InputTag("met"));
      tcmetToken_ = iC.consumes<reco::METCollection>(edm::InputTag("tcMet"));
      pfmetToken_ = iC.consumes<reco::PFMETCollection>(edm::InputTag("pfMet"));
      tree_->Branch("event_nPV"        ,&mNPV_                 ,"mNPV/I");
      tree_->Branch("event_met_calomet"    ,&mMET_                ,"mMET/F");
      tree_->Branch("event_met_calosumet"  ,&mSumET_              ,"mSumET/F");
      tree_->Branch("event_met_calometsignificance",&mMETSign_    ,"mMETSign/F");
      tree_->Branch("event_met_tcmet"    ,&mtcMET_                ,"mtcMET/F");
      tree_->Branch("event_met_tcsumet"  ,&mtcSumET_              ,"mtcSumET/F");
      tree_->Branch("event_met_tcmetsignificance",&mtcMETSign_    ,"mtcMETSign/F");
      tree_->Branch("event_met_pfmet"    ,&mpfMET_                ,"mpfMET/F");
      tree_->Branch("event_met_pfsumet"  ,&mpfSumET_              ,"mpfSumET/F");
      tree_->Branch("event_met_pfmetsignificance",&mpfMETSign_    ,"mpfMETSign/F");
      tree_->Branch("event_PrimaryVertex_x"  ,&mPVx_              ,"mPVx/F");
      tree_->Branch("event_PrimaryVertex_y"  ,&mPVy_              ,"mPVy/F");
      tree_->Branch("event_PrimaryVertex_z"  ,&mPVz_              ,"mPVz/F");
      tree_->Branch("event_BeamSpot_x"       ,&mBSx_              ,"mBSx/F");
      tree_->Branch("event_BeamSpot_y"       ,&mBSy_              ,"mBSy/F");
      tree_->Branch("event_BeamSpot_z"       ,&mBSz_              ,"mBSz/F");
    }

    ignoreExceptions_ = iConfig.existsAs<bool>("ignoreExceptions") ? iConfig.getParameter<bool>("ignoreExceptions") : false;
}

tnp::BaseGenericTreeFiller::BaseGenericTreeFiller(BaseGenericTreeFiller &main, const edm::ParameterSet &iConfig, edm::ConsumesCollector && iC, const std::string &branchNamePrefix) :
    addEventVariablesInfo_(false),
    tree_(0)
{
    addBranches_(main.tree_, iConfig, iC, branchNamePrefix);
}

void tnp::BaseGenericTreeFiller::addBranches_(TTree *tree, const edm::ParameterSet &iConfig, edm::ConsumesCollector & iC, const std::string &branchNamePrefix) {
  
 // set up variables
    edm::ParameterSet variables = iConfig.getParameter<edm::ParameterSet>("variables");
    //.. the ones that are strings
    std::vector<std::string> stringVars = variables.getParameterNamesForType<std::string>();
    for (std::vector<std::string>::const_iterator it = stringVars.begin(), ed = stringVars.end(); it != ed; ++it) {
        //---- TLorentzVectors
        if (std::strncmp((branchNamePrefix + *it).c_str(), "v_", strlen("v_")) == 0) {
         varsMap_["_TEMP_"+branchNamePrefix + *it+"_x_"] = new tnp::ProbeVariable("_TEMP_"+branchNamePrefix + *it+"_x_", variables.getParameter<std::string>(*it)+".x()");
         varsMap_["_TEMP_"+branchNamePrefix + *it+"_y_"] = new tnp::ProbeVariable("_TEMP_"+branchNamePrefix + *it+"_y_", variables.getParameter<std::string>(*it)+".y()");
         varsMap_["_TEMP_"+branchNamePrefix + *it+"_z_"] = new tnp::ProbeVariable("_TEMP_"+branchNamePrefix + *it+"_z_", variables.getParameter<std::string>(*it)+".z()");
         varsMap_["_TEMP_"+branchNamePrefix + *it+"_t_"] = new tnp::ProbeVariable("_TEMP_"+branchNamePrefix + *it+"_t_", variables.getParameter<std::string>(*it)+".t()");

         varsMap_[branchNamePrefix + *it]                = new tnp::ProbeVariable(branchNamePrefix + *it, variables.getParameter<std::string>(*it));
        }
        //---- std::vector <float>
        else if (std::strncmp((branchNamePrefix + *it).c_str(), "std_vector_", strlen("std_vector_")) == 0) {
         
         std::string nameVariable = variables.getParameter<std::string>(*it);
         int lenghtVector_nameVariable_int;
         lenghtVector_nameVariable_int = _maxStdVector;
         //---- check if explicit vector length, if not, default will be used
         if (nameVariable.find("/") != std::string::npos) {
          std::size_t position = nameVariable.find("/");  //---- position of "/" in str
          std::string lenghtVector_nameVariable = nameVariable.substr (position+1);
          lenghtVector_nameVariable_int = atoi(lenghtVector_nameVariable.c_str());
          nameVariable =  nameVariable.substr(0,position);
          _map_vectorLength[branchNamePrefix + *it] = lenghtVector_nameVariable_int;
         }
         
         for (int i=0; i<lenghtVector_nameVariable_int; i++) {
          //---- if I call "pt" it will do "pt(0)", "pt(1)", ...
          //---- if I call "pt(ggg" it will do "pt(ggg,0)", "pt(ggg,1)", ...
          if (nameVariable.find("(") != std::string::npos) {
           varsMap_["_VECTORTEMP_"+branchNamePrefix + *it+"_"+std::to_string(i+1)+"_"] = new tnp::ProbeVariable("_VECTORTEMP_"+branchNamePrefix + *it+"_"+std::to_string(i+1)+"_", nameVariable+","+std::to_string(i)+")");
          }
          else {
           varsMap_["_VECTORTEMP_"+branchNamePrefix + *it+"_"+std::to_string(i+1)+"_"] = new tnp::ProbeVariable("_VECTORTEMP_"+branchNamePrefix + *it+"_"+std::to_string(i+1)+"_", nameVariable+"("+std::to_string(i)+")");
          }
         }
         //--->                                                     the second field is NOT used!
         varsMap_[branchNamePrefix + *it]  = new tnp::ProbeVariable(branchNamePrefix + *it, branchNamePrefix + *it);
//          vars_.push_back(tnp::ProbeVariable(branchNamePrefix + *it, nameVariable));        
         }
        //---- std::vector <float> with variable length
        else if (std::strncmp((branchNamePrefix + *it).c_str(), "std_variable_vector_", strlen("std_variable_vector_")) == 0) {
         std::string nameVariable = variables.getParameter<std::string>(*it);
         int lenghtVector_nameVariable_int;
         lenghtVector_nameVariable_int = _maxStdVector;
         //---- check if explicit vector length, if not, default will be used
         if (nameVariable.find("/") != std::string::npos) {
          std::size_t position = nameVariable.find("/");  //---- position of "/" in str
          std::string lenghtVector_nameVariable = nameVariable.substr (position+1);
          lenghtVector_nameVariable_int = atoi(lenghtVector_nameVariable.c_str());
          nameVariable =  nameVariable.substr(0,position);
          _map_variables_vectorLength[branchNamePrefix + *it] = lenghtVector_nameVariable_int;
         }

         for (int i=0; i<lenghtVector_nameVariable_int; i++) {
          //---- if I call "pt" it will do "pt(0)", "pt(1)", ...
          //---- if I call "pt(ggg" it will do "pt(ggg,0)", "pt(ggg,1)", ...
          if (nameVariable.find("(") != std::string::npos) {
           varsMap_["_VECTORVARIABLETEMP_"+branchNamePrefix + *it+"_"+std::to_string(i+1)+"_"] = new tnp::ProbeVariable("_VECTORVARIABLETEMP_"+branchNamePrefix + *it+"_"+std::to_string(i+1)+"_", nameVariable+","+std::to_string(i)+")");
          }
          else {
           varsMap_["_VECTORVARIABLETEMP_"+branchNamePrefix + *it+"_"+std::to_string(i+1)+"_"] = new tnp::ProbeVariable("_VECTORVARIABLETEMP_"+branchNamePrefix + *it+"_"+std::to_string(i+1)+"_", nameVariable+"("+std::to_string(i)+")");           
          }
         }
         //--->                                                     the second field is NOT used!
         varsMap_[branchNamePrefix + *it] = new tnp::ProbeVariable(branchNamePrefix + *it, branchNamePrefix + *it);
//          vars_.push_back(tnp::ProbeVariable(branchNamePrefix + *it, nameVariable));        
        }
        
        //---- simple variable
        else {
         varsMap_[branchNamePrefix + *it] = new tnp::ProbeVariable(branchNamePrefix + *it, variables.getParameter<std::string>(*it));
        }
    }
        
    //.. the ones that are InputTags
    std::vector<std::string> inputTagVars = variables.getParameterNamesForType<edm::InputTag>();
    for (std::vector<std::string>::const_iterator it = inputTagVars.begin(), ed = inputTagVars.end(); it != ed; ++it) {
        varsMap_[branchNamePrefix + *it] = new tnp::ProbeVariable(branchNamePrefix + *it, iC.consumes<edm::ValueMap<float> >(variables.getParameter<edm::InputTag>(*it)));
    }
    // set up flags
    edm::ParameterSet flags = iConfig.getParameter<edm::ParameterSet>("flags");
    //.. the ones that are strings
    std::vector<std::string> stringFlags = flags.getParameterNamesForType<std::string>();
    for (std::vector<std::string>::const_iterator it = stringFlags.begin(), ed = stringFlags.end(); it != ed; ++it) {
        flags_.push_back(tnp::ProbeFlag(branchNamePrefix + *it, flags.getParameter<std::string>(*it)));
    }
    //.. the ones that are InputTags
    std::vector<std::string> inputTagFlags = flags.getParameterNamesForType<edm::InputTag>();
    for (std::vector<std::string>::const_iterator it = inputTagFlags.begin(), ed = inputTagFlags.end(); it != ed; ++it) {
        flags_.push_back(tnp::ProbeFlag(branchNamePrefix + *it, iC.consumes<edm::View<reco::Candidate> >(flags.getParameter<edm::InputTag>(*it))));
    }

    // then make all the variables in the trees
//     for (std::vector<tnp::ProbeVariable>::iterator it = vars_.begin(), ed = vars_.end(); it != ed; ++it) {
//         tree->Branch(it->name().c_str(), it->address(), (it->name()+"/F").c_str());
//     }

    // then make all the FLOAT and 4D variables in the trees
    for (std::map<std::string,tnp::ProbeVariable*>::iterator it = varsMap_.begin(), ed = varsMap_.end(); it != ed; ++it) {
     if (std::strncmp(it->first.c_str(), "_TEMP_", strlen("_TEMP_")) != 0
      && std::strncmp(it->first.c_str(), "_VECTORTEMP_", strlen("_VECTORTEMP_")) != 0         
      && std::strncmp(it->first.c_str(), "_VECTORVARIABLETEMP_", strlen("_VECTORVARIABLETEMP_")) != 0         
     ) { //---- only *not* temporary variables  
      //---- TLorentzVectors
      
      if (std::strncmp(it->first.c_str(), "v_", strlen("v_")) == 0) {
       tree->Branch(it->first.c_str(), "math::XYZTLorentzVector", it->second->address4D());
      }
      //---- std::vector <float>
      else if (std::strncmp(it->first.c_str(), "std_vector_", strlen("std_vector_")) == 0) {
       tree->Branch(it->first.c_str(), it->second->address_std_vector());
//        tree->Branch(it->name().c_str(), "std::vector<float>", it->address_std_vector());       
      }     
      //---- std::vector <float>  with variable length
      else if (std::strncmp(it->first.c_str(), "std_variable_vector_", strlen("std_variable_vector_")) == 0) {
       tree->Branch(it->first.c_str(), it->second->address_std_vector_variable_length());
       //        tree->Branch(it->name().c_str(), "std::vector<float>", it->address_std_vector_variable_length());       
      } 
      //---- float
      else {
       tree->Branch(it->first.c_str(), it->second->address(), (it->first+"/F").c_str());
      }
     }
    }
    
    for (std::vector<tnp::ProbeFlag>::iterator it = flags_.begin(), ed = flags_.end(); it != ed; ++it) {
        tree->Branch(it->name().c_str(), it->address(), (it->name()+"/I").c_str());
    }

}

tnp::BaseGenericTreeFiller::~BaseGenericTreeFiller() { }

void tnp::BaseGenericTreeFiller::init(const edm::Event &iEvent) const {
 
    run_  = iEvent.id().run();
    lumi_ = iEvent.id().luminosityBlock();
    event_ = iEvent.id().event();

    for (std::map<std::string,tnp::ProbeVariable*>::const_iterator it = varsMap_.begin(), ed = varsMap_.end(); it != ed; ++it) {
        it->second->init(iEvent);
    }

    for (std::vector<tnp::ProbeFlag>::const_iterator it = flags_.begin(), ed = flags_.end(); it != ed; ++it) {
        it->init(iEvent);
    }
    if (weightMode_ == External) {
        edm::Handle<double> weight;
        iEvent.getByToken(weightSrcToken_, weight);
        weight_ = *weight;
    }

    if (addEventVariablesInfo_) {
        /// *********** store some event variables: MET, SumET ******
        //////////// Primary vertex //////////////
        edm::Handle<reco::VertexCollection> recVtxs;
        iEvent.getByToken(recVtxsToken_,recVtxs);
        mNPV_ = 0;
        mPVx_ =  100.0;
        mPVy_ =  100.0;
        mPVz_ =  100.0;

        for(unsigned int ind=0;ind<recVtxs->size();ind++) {
          if (!((*recVtxs)[ind].isFake()) && ((*recVtxs)[ind].ndof()>4)
              && (fabs((*recVtxs)[ind].z())<=24.0) &&
              ((*recVtxs)[ind].position().Rho()<=2.0) ) {
            mNPV_++;
            if(mNPV_==1) { // store the first good primary vertex
              mPVx_ = (*recVtxs)[ind].x();
              mPVy_ = (*recVtxs)[ind].y();
              mPVz_ = (*recVtxs)[ind].z();
            }
          }
        }


        //////////// Beam spot //////////////
        edm::Handle<reco::BeamSpot> beamSpot;
        iEvent.getByToken(beamSpotToken_, beamSpot);
        mBSx_ = beamSpot->position().X();
        mBSy_ = beamSpot->position().Y();
        mBSz_ = beamSpot->position().Z();


        ////////////// CaloMET //////
        edm::Handle<reco::CaloMETCollection> met;
        iEvent.getByToken(metToken_,met);
        if (met->size() == 0) {
          mMET_   = -1;
          mSumET_ = -1;
          mMETSign_ = -1;
        }
        else {
          mMET_   = (*met)[0].et();
          mSumET_ = (*met)[0].sumEt();
          mMETSign_ = (*met)[0].significance();
        }

        /////// TcMET information /////
        edm::Handle<reco::METCollection> tcmet;
        iEvent.getByToken(tcmetToken_, tcmet);
        if (tcmet->size() == 0) {
          mtcMET_   = -1;
          mtcSumET_ = -1;
          mtcMETSign_ = -1;
        }
        else {
          mtcMET_   = (*tcmet)[0].et();
          mtcSumET_ = (*tcmet)[0].sumEt();
          mtcMETSign_ = (*tcmet)[0].significance();
        }

        /////// PfMET information /////
        edm::Handle<reco::PFMETCollection> pfmet;
        iEvent.getByToken(pfmetToken_, pfmet);
        if (pfmet->size() == 0) {
          mpfMET_   = -1;
          mpfSumET_ = -1;
          mpfMETSign_ = -1;
        }
        else {
          mpfMET_   = (*pfmet)[0].et();
          mpfSumET_ = (*pfmet)[0].sumEt();
          mpfMETSign_ = (*pfmet)[0].significance();
        }
    }

}

void tnp::BaseGenericTreeFiller::fill(const reco::CandidateBaseRef &probe)  {
 //auto startTime = std::chrono::high_resolution_clock::now();

 for (std::map<std::string,tnp::ProbeVariable*>::const_iterator it = varsMap_.begin(), ed = varsMap_.end(); it != ed; ++it) {
  if (std::strncmp(it->first.c_str(), "v_", strlen("v_")) != 0 &&
      std::strncmp(it->first.c_str(), "std_vector_", strlen("std_vector_")) != 0 &&
      std::strncmp(it->first.c_str(), "std_variable_vector_", strlen("std_variable_vector_")) != 0
  ) { //---- first normal variables  
    it->second->fill(probe);
  }
 }
 //auto endTime = std::chrono::high_resolution_clock::now();
 //std::cout << "time in filling normal variables: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count() << " ms" << std::endl ;

 //startTime = std::chrono::high_resolution_clock::now(); 
 for (std::map<std::string,tnp::ProbeVariable*>::const_iterator it = varsMap_.begin(), ed = varsMap_.end(); it != ed; ++it) {
  if (
   std::strncmp(it->first.c_str(), "_TEMP_", strlen("_TEMP_")) != 0 &&
   std::strncmp(it->first.c_str(), "_VECTORTEMP_", strlen("_VECTORTEMP_")) != 0 &&
   std::strncmp(it->first.c_str(), "_VECTORVARIABLETEMP_", strlen("_VECTORVARIABLETEMP_")) != 0
  ) { //---- only *not* temporary variables  
   if (std::strncmp(it->first.c_str(), "v_", strlen("v_")) == 0) { //---- now the vectors
    float x = 0;
    float y = 0;
    float z = 0;
    float t = 0; 
    x = varsMap_[std::string("_TEMP_" + it->first +"_x_")]->internal_value();
    y = varsMap_[std::string("_TEMP_" + it->first +"_y_")]->internal_value();
    z = varsMap_[std::string("_TEMP_" + it->first +"_z_")]->internal_value();
    t = varsMap_[std::string("_TEMP_" + it->first +"_t_")]->internal_value();
    it->second->fill4D(x,y,z,t);
   }
   else if (std::strncmp(it->first.c_str(), "std_vector_", strlen("std_vector_")) == 0) { //---- now the std::vector <float>
    int lenghtVector_nameVariable_int = _maxStdVector;
    if (_map_vectorLength.find( it->first )  != _map_vectorLength.end()) {
      lenghtVector_nameVariable_int = _map_vectorLength.at(it->first);
    }
    for (int i=0; i<lenghtVector_nameVariable_int; i++) {
      it->second->fillStdVector(varsMap_["_VECTORTEMP_" + it->first +"_"+std::to_string(i+1)+"_"]->internal_value(),i);
    }
   }
   else if (std::strncmp(it->first.c_str(), "std_variable_vector_", strlen("std_variable_vector_")) == 0) { //---- now the std::vector <float> with variable length
    int lenghtVector_nameVariable_int = _maxStdVector;
    if (_map_variables_vectorLength.find( it->first )  != _map_variables_vectorLength.end()) {
      lenghtVector_nameVariable_int = _map_variables_vectorLength.at(it->first);
    }
    for (int i=0; i<lenghtVector_nameVariable_int; i++) {
      it->second->fillStdVector(varsMap_["_VECTORVARIABLETEMP_" + it->first +"_"+std::to_string(i+1)+"_"]->internal_value(), i);
    }
   }   
   
  }
 }
 //endTime = std::chrono::high_resolution_clock::now();
 //std::cout << "time in filling other variables: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count() << " ms" << std::endl ;
 
 //startTime = std::chrono::high_resolution_clock::now(); 
 for (std::vector<tnp::ProbeFlag>::const_iterator it = flags_.begin(), ed = flags_.end(); it != ed; ++it) {
  if (ignoreExceptions_)  {
   try{ it->fill(probe); } catch(cms::Exception &ex ){}
  } else {
   it->fill(probe);
  }
 }
 //endTime = std::chrono::high_resolution_clock::now();
 //std::cout << "time in filling flag variables: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count() << " ms" << std::endl ;

   
 //startTime = std::chrono::high_resolution_clock::now(); 
 if (tree_) tree_->Fill();
 //endTime = std::chrono::high_resolution_clock::now();
 //std::cout << "time in filling tree: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count() << " ms" << std::endl ;
 
}


void tnp::BaseGenericTreeFiller::writeProvenance(const edm::ParameterSet &pset) const {
    TList *list = tree_->GetUserInfo();
    list->Add(new TObjString(pset.dump().c_str()));
}
