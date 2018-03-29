#ifndef DETPEDESTALSERVICE_H
#define DETPEDESTALSERVICE_H

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "larcore/CoreUtils/ServiceUtil.h" // unused; for includer's convenience
#include "larevt/CalibrationDBI/Interface/DetPedestalProvider.h"

namespace lariov{

  /**
     \class DetPedestalService
     art service interface for detector pedestal conditions retrieval
  */
  class DetPedestalService {
  
    public:
      using provider_type = DetPedestalProvider;

      virtual ~DetPedestalService() = default;

      //retrieve pedestal provider
      const DetPedestalProvider& GetPedestalProvider() const {
        return this->DoGetPedestalProvider();
      }
      
      DetPedestalProvider const* provider() const
        { return &DoGetPedestalProvider(); }
      
      
    private:
      
      virtual const DetPedestalProvider& DoGetPedestalProvider() const = 0;
  };
}//end namespace lariov

DECLARE_ART_SERVICE_INTERFACE(lariov::DetPedestalService, LEGACY)


// check that the requirements for lariov::ChannelStatusService are satisfied
template struct lar::details::ServiceRequirementsChecker<lariov::DetPedestalService>;

#endif
