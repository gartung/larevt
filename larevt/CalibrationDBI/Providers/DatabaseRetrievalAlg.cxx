#ifndef DATABASERETRIEVALALG_CXX
#define DATABASERETRIEVALALG_CXX 1

#include "DatabaseRetrievalAlg.h"

namespace lariov {

  /// Configure using fhicl::ParameterSet
  void DatabaseRetrievalAlg::Reconfigure(fhicl::ParameterSet const& p) {

    std::string foldername = p.get<std::string>("DBFolderName");
    std::string url        = p.get<std::string>("DBUrl");
    std::string tag        = p.get<std::string>("DBTag", "");
    bool applyScaling      = p.get<bool>("ApplyScaling",false);
    std::uint64_t Modifier   = p.get<std::uint64_t>("Scaling",1);
   // std::string  OverrideDate = p.get<std::string>("OverrideDate","");
    DBTimeStamp_t  OverrideTimeStamp = p.get<std::uint64_t>("OverrideStamp",0);
    std::string OverrideURL = p.get<std::string>("OverrideURL","");

    (void)applyScaling;
    (void)Modifier;
    //if(applyScaling && !OverrideDate.length() )

    if(applyScaling && OverrideTimeStamp==0 && OverrideURL.length()==0)                              // apply scaling
      fFolder.reset(new DBFolder(foldername, url, tag,applyScaling,Modifier));
    //else if(!applyScaling && OverrideDate.length() ) 

    else if(!applyScaling && OverrideTimeStamp!=0 && OverrideURL.length()==0)                        // modify timestamp
      //fFolder.reset(new DBFolder(foldername, url, tag,OverrideDate));
      fFolder.reset(new DBFolder(foldername, url, tag,OverrideTimeStamp));
    //else if( applyScaling && OverrideDate.length() )

    else if(!applyScaling && OverrideTimeStamp==0 && OverrideURL.length()!=0)                        // modify URL
      fFolder.reset(new DBFolder(foldername,url,tag,OverrideURL));

    else if(!applyScaling && OverrideTimeStamp==0 && OverrideURL.length()==0){                       // nothing is changed
      fFolder.reset(new DBFolder(foldername, url, tag));
    }

    else                                                                                             // try to modify more then one variable at once
      std::cout << "----- error, can't do the three of them ----- "  << std::endl;

  }
}
#endif
