////////////////////////////////////////////////////////////////////////
// \file LArProperties.h
//
// \brief pure virtual base interface for LAr properties
//
// \author jpaley@fnal.gov
// 
////////////////////////////////////////////////////////////////////////
#ifndef DETINFO_LARPROPERTIES_H
#define DETINFO_LARPROPERTIES_H


///General LArSoft Utilities
namespace detinfo{
  
  class LArProperties {
    public:

      LArProperties(const LArProperties &) = delete;
      LArProperties(LArProperties &&) = delete;
      LArProperties& operator = (const LArProperties &) = delete;
      LArProperties& operator = (LArProperties &&) = delete;
      virtual ~LArProperties() = default;
      
      virtual double Density(double temperature=0.) const = 0;    
      virtual double Temperature()                  const = 0;
      virtual double RadiationLength()  	    const = 0;    
      virtual double Argon39DecayRate()             const = 0;
      virtual double Eloss(double mom, double mass, double tcut) const = 0;
      virtual double ElossVar(double mom, double mass) const = 0;
	
      virtual double ScintResolutionScale() const = 0;
      virtual double ScintFastTimeConst()   const = 0;
      virtual double ScintSlowTimeConst()   const = 0; 
      virtual double ScintBirksConstant()   const = 0;	
      virtual bool ScintByParticleType()    const = 0;

      virtual double ScintYield(bool prescale = false)         const = 0; 
      virtual double ScintPreScale(bool prescale = true)       const = 0; 
      virtual double ScintYieldRatio()                         const = 0; 
	
      virtual double ProtonScintYield(bool prescale = false)   const = 0;
      virtual double ProtonScintYieldRatio()                   const = 0; 
      virtual double MuonScintYield(bool prescale = false)     const = 0; 
      virtual double MuonScintYieldRatio()                     const = 0; 
      virtual double KaonScintYield(bool prescale = false)     const = 0; 
      virtual double KaonScintYieldRatio()                     const = 0; 
      virtual double PionScintYield(bool prescale = false)     const = 0; 
      virtual double PionScintYieldRatio()                     const = 0; 
      virtual double ElectronScintYield(bool prescale = false) const = 0; 
      virtual double ElectronScintYieldRatio()                 const = 0; 
      virtual double AlphaScintYield(bool prescale = false)    const = 0; 
      virtual double AlphaScintYieldRatio()                    const = 0; 
      virtual bool CerenkovLightEnabled()                      const = 0; 
	
	
      virtual std::map<double, double> SlowScintSpectrum() const = 0;   
      virtual std::map<double, double> FastScintSpectrum() const = 0;
      virtual std::map<double, double> RIndexSpectrum() const = 0;
      virtual std::map<double, double> AbsLengthSpectrum() const = 0;
      virtual std::map<double, double> RayleighSpectrum() const = 0;
	
      virtual std::map<std::string, std::map<double, double> > SurfaceReflectances() const = 0;
      virtual std::map<std::string, std::map<double, double> > SurfaceReflectanceDiffuseFractions() const = 0;

    protected:
      LArProperties() = default;
      
    }; // class LArProperties
} //namespace detinfo

#endif // DETINFO_ILARPROPERTIES_H
