#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "LatinoTrees/DataFormats/interface/SkimEvent.h"

namespace reco {
 namespace modules {
  typedef SingleObjectSelector<
            std::vector<reco::SkimEvent>,
            StringCutObjectSelector<reco::SkimEvent>
          > SkimEventSelector;

  DEFINE_FWK_MODULE( SkimEventSelector );
 }
}