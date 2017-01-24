////////////////////////////////////////////////////////////////////////
//
// NeutrinoEnergyFilter class:
// 
// Filter on the truth neutrino energy
// 
//
// Joseph Zennamo, UChicago, 2017
// jzennamo@uchicago.edu
//
////////////////////////////////////////////////////////////////////////

#ifndef NEUTRINOENERGYFILTER_H
#define NEUTRINOENERGYFILTER_H

//Framework Includes
#include "art/Framework/Core/EDFilter.h"
#include "art/Framework/Core/ModuleMacros.h" 
#include "art/Framework/Principal/Event.h" 
#include "fhiclcpp/ParameterSet.h" 
#include "art/Framework/Principal/Handle.h" 
#include "canvas/Persistency/Common/Ptr.h" 
#include "canvas/Persistency/Common/PtrVector.h" 
#include "art/Framework/Services/Registry/ServiceHandle.h" 
#include "art/Framework/Services/Optional/TFileService.h" 
#include "art/Framework/Services/Optional/TFileDirectory.h" 
#include "messagefacility/MessageLogger/MessageLogger.h" 


//Larsoft Includes
#include "nusimdata/SimulationBase/MCNeutrino.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "nusimdata/SimulationBase/MCFlux.h"
#include "nusimdata/SimulationBase/GTruth.h"

#include "TH1.h"
#include "TH2.h"

namespace filt {

  class NeutrinoEnergyFilter : public art::EDFilter  {
    
  public:
    
    explicit NeutrinoEnergyFilter(fhicl::ParameterSet const& ); 
    virtual ~NeutrinoEnergyFilter();
         
    
    bool filter(art::Event& evt);
    void reconfigure(fhicl::ParameterSet const& p);
    void beginJob();
   

  private: 
 
    std::string fGenieModuleLabel;
    double fEnuMin;
    double fEnuMax;
    TH1D* fSelectedEvents;
    TH1D* fTotalEvents;

  protected: 

  }; // class NeutrinoEnergyFilter

}

namespace filt{

  //-------------------------------------------------
  NeutrinoEnergyFilter::NeutrinoEnergyFilter(fhicl::ParameterSet const & pset)  
  {   
    this->reconfigure(pset);
  }

  //-------------------------------------------------
  NeutrinoEnergyFilter::~NeutrinoEnergyFilter()
  {
  }
  
  //-------------------------------------------------
  void NeutrinoEnergyFilter::reconfigure(fhicl::ParameterSet const& p)
  {
    fGenieModuleLabel  = p.get< std::string      >("GenieModuleLabel");
    fEnuMin            = p.get< double >("EnuMin");
    fEnuMax            = p.get< double >("EnuMax");
  } 

  //-------------------------------------------------
  void NeutrinoEnergyFilter::beginJob()
  {
    art::ServiceHandle<art::TFileService> tfs;
    fSelectedEvents = tfs->make<TH1D>("fSelectedEvents", "Number of Selected Events", 3, 0, 3); //counts the number of selected events 
    fTotalEvents = tfs->make<TH1D>("fTotalEvents", "Total Events", 3, 0, 3); //counts the initial number of events in the unfiltered root input file
  }

  //-------------------------------------------------
  bool NeutrinoEnergyFilter::filter(art::Event &evt)
  { 
        
    art::Handle< std::vector<simb::MCTruth> > mclist;
    evt.getByLabel(fGenieModuleLabel,mclist);
    art::Ptr<simb::MCTruth> mc(mclist,0);
    
    simb::MCNeutrino nu(mc->GetNeutrino());

    fTotalEvents->Fill(1);
    
    double Enu = double(nu.Nu().Trajectory().Momentum(0).E());
     
    if( Enu > fEnuMin && Enu < fEnuMax){
      fSelectedEvents->Fill(1);
      return true;      
    }

    fSelectedEvents->Fill(0);
    return false;      


  } // bool  
}

//--------------------------------------------------

namespace filt {

  DEFINE_ART_MODULE(NeutrinoEnergyFilter)

} //namespace filt

#endif // FINALSTATEPARTICLEFILTER_H
