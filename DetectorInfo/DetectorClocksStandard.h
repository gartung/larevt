////////////////////////////////////////////////////////////////////////
//
// DetectorClocks.h
//
//     This class provides electronics various electronics clocks. Currently supports
//     three types of clocks: TPC, Optical, and Trigger in order to support the
//     MicroBooNE experiment.
//  
//     Formally known as TimeService.
//
////////////////////////////////////////////////////////////////////////

#ifndef DETINFO_DETCLOCKSSTD_H
#define DETINFO_DETCLOCKSSTD_H

#include "fhiclcpp/ParameterSet.h"

#include "DetectorInfo/DetectorClocks.h"
#include "RawData/TriggerData.h"

namespace detinfo{

  class DetectorClocksStandard : public DetectorClocks {

  public:
    DetectorClocksStandard();
    DetectorClocksStandard(fhicl::ParameterSet const& pset);
    DetectorClocksStandard(DetectorClocksStandard const&) = delete;
    virtual ~DetectorClocksStandard() {};
    
    bool Configure(fhicl::ParameterSet const& pset);
    bool Update(uint64_t ts=0);
    
    virtual double TriggerOffsetTPC() const
    { 
      if (fTriggerOffsetTPC<0)
	return fTriggerOffsetTPC; 
      else
	return -fTriggerOffsetTPC/fTPCClock.Frequency(); //convert ticks to us
    }
    
    void debugReport() const;
    
    std::string TrigModuleName() const { return fTrigModuleName; }

    /// Given Geant4 time [ns], returns relative time [us] w.r.t. electronics time T0 
    virtual double G4ToElecTime(double g4_time) const {return g4_time * 1.e-3 - fG4RefTime; }
    
    /// Trigger electronics clock time in [us]
    virtual double TriggerTime() const { return fTriggerTime; }
    
    /// Beam gate electronics clock time in [us]
    virtual double BeamGateTime() const { return fBeamGateTime; }

    virtual std::vector<std::string> ConfigNames() const { return fConfigName; }
    virtual std::vector<double> ConfigValues() const { return fConfigValue; }

    void SetConfigValue(size_t i, double val) { fConfigValue[i] = val; }

        /// Setter for trigger times
    virtual void SetTriggerTime(double trig_time, double beam_time)
    { 
      fTriggerTime  = trig_time;
      fBeamGateTime = beam_time;
      fTPCClock.SetTime(trig_time);
      fOpticalClock.SetTime(trig_time);
      fTriggerClock.SetTime(trig_time);
    }

    //
    // Getters of TPC ElecClock
    //
    /// Borrow a const TPC clock with time set to Trigger time [us]
    virtual const ::util::ElecClock& TPCClock() const
    { return fTPCClock; }

    /// Create a TPC clock for a given time [us] from clock counting start
    virtual ::util::ElecClock TPCClock(double time) const 
	{ return ::util::ElecClock(time,fTPCClock.FramePeriod(),fTPCClock.Frequency());}

    /// Create a TPC clock for a given sample/frame number in TPC clock frequency
    util::ElecClock TPCClock(unsigned int sample,
		       unsigned int frame) const
      { util::ElecClock clock = TPCClock(); clock.SetTime(sample,frame); return clock; }
    
    //
    // Getters of Optical ElecClock
    //
    /// Borrow a const Optical clock with time set to Trigger time [us]
    virtual const util::ElecClock& OpticalClock() const
    { return fOpticalClock; }

    /// Create a Optical clock for a given time [us] from clock counting start
    virtual util::ElecClock OpticalClock(double time) const 
      { return util::ElecClock(time,fOpticalClock.FramePeriod(),fOpticalClock.Frequency());}

    /// Create a Optical clock for a given sample/frame number in Optical clock frequency
    virtual util::ElecClock OpticalClock(unsigned int sample,
			   unsigned int frame) const
    { util::ElecClock clock = OpticalClock(); clock.SetTime(sample,frame); return clock; }

    //
    // Getters of Trigger ElecClock
    //
    /// Borrow a const Trigger clock with time set to Trigger time [us]
    virtual const util::ElecClock& TriggerClock() const
    { return fTriggerClock; }

    /// Create a Trigger clock for a given time [us] from clock counting start    
    virtual util::ElecClock TriggerClock(double time) const 
    { return util::ElecClock(time,fTriggerClock.FramePeriod(),fTriggerClock.Frequency());}

    /// Create a Trigger clock for a given sample/frame number in Trigger clock frequency
    virtual util::ElecClock TriggerClock(unsigned int sample,
			   unsigned int frame) const
    { util::ElecClock clock = TriggerClock(); clock.SetTime(sample,frame); return clock; }

    //
    // Getters of External ElecClock
    //
    /// Borrow a const Trigger clock with time set to External Time [us]
    virtual const util::ElecClock& ExternalClock() const
    { return fExternalClock; }

    /// Create a External clock for a given time [us] from clock counting start
    virtual util::ElecClock ExternalClock(double time) const
      { return util::ElecClock(time,fExternalClock.FramePeriod(),fTriggerClock.Frequency());}
    
    /// Create a External clock for a given sample/frame number in External clock frequency
    virtual util::ElecClock ExternalClock(unsigned int sample,
				  unsigned int frame) const
      { util::ElecClock clock = ExternalClock(); clock.SetTime(sample,frame); return clock; }
    
    //
    // Getters for time [us] w.r.t. trigger given information from waveform
    //

    /// Given TPC time-tick (waveform index), returns time [us] w.r.t. trigger time stamp
    virtual double TPCTick2TrigTime(double tick) const
    { return fTPCClock.TickPeriod() * tick + TriggerOffsetTPC(); }
    /// Given TPC time-tick (waveform index), returns time [us] w.r.t. beam gate time
    virtual double TPCTick2BeamTime(double tick) const
    { return fTPCClock.TickPeriod() * tick + TriggerOffsetTPC() + TriggerTime() - BeamGateTime(); }
    /// Given Optical time-tick (waveform index), sample and frame number, returns time [us] w.r.t. trigger time stamp
    virtual double OpticalTick2TrigTime(double tick, size_t sample, size_t frame) const
    { return fOpticalClock.TickPeriod() * tick + fOpticalClock.Time(sample,frame) - TriggerTime();  }
    /// Given Optical time-tick (waveform index), sample and frame number, returns time [us] w.r.t. beam gate time stamp
    virtual double OpticalTick2BeamTime(double tick, size_t sample, size_t frame) const
    { return fOpticalClock.TickPeriod() * tick + fOpticalClock.Time(sample,frame) - BeamGateTime(); }
    /// Given External time-tick (waveform index), sample and frame number, returns time [us] w.r.t. trigger time stamp
    virtual double ExternalTick2TrigTime(double tick, size_t sample, size_t frame) const
    { return fExternalClock.TickPeriod() * tick + fExternalClock.Time(sample,frame) - TriggerTime(); }
    /// Given External time-tick (waveform index), sample and frame number, returns time [us] w.r.t. beam gate time stamp
    virtual double ExternalTick2BeamTime(double tick, size_t sample, size_t frame) const
    { return fExternalClock.TickPeriod() * tick + fExternalClock.Time(sample,frame) - BeamGateTime(); }
    
    //
    // Getters for time [tdc] (electronics clock counting ... in double precision) 
    //

    /// Given TPC time-tick (waveform index), returns electronics clock count [tdc]
    virtual double TPCTick2TDC(double tick) const
    { return ( (TriggerTime() + TriggerOffsetTPC()) / fTPCClock.TickPeriod() + tick ); }
    /// Given G4 time [ns], returns corresponding TPC electronics clock count [tdc]
    virtual double TPCG4Time2TDC(double g4time) const
    { return G4ToElecTime(g4time) / fTPCClock.TickPeriod(); }
    /// Given Optical time-tick (waveform index), sample and frame number, returns time electronics clock count [tdc]
    virtual double OpticalTick2TDC(double tick, size_t sample, size_t frame) const
    { return fOpticalClock.Ticks(sample,frame) + tick; }
    /// Given G4 time [ns], returns corresponding Optical electronics clock count [tdc]  
    virtual double OpticalG4Time2TDC(double g4time) const
    { return G4ToElecTime(g4time) / fOpticalClock.TickPeriod(); }
    /// Given External time-tick (waveform index), sample and frame number, returns time electronics clock count [tdc]
    virtual double ExternalTick2TDC(double tick, size_t sample, size_t frame) const
    { return fExternalClock.Ticks(sample,frame) + tick; }
    /// Given G4 time [ns], returns corresponding External electronics clock count [tdc]
    virtual double ExternalG4Time2TDC(double g4time) const
    { return G4ToElecTime(g4time) / fExternalClock.TickPeriod(); }

    //
    // Getters for time [us] (electronics clock counting ... in double precision)
    //
    /// Given TPC time-tick (waveform index), returns electronics clock [us]
    virtual double TPCTick2Time(double tick) const
    { return TriggerTime() + TriggerOffsetTPC() + tick * fTPCClock.TickPeriod(); }
    /// Given Optical time-tick (waveform index), sample and frame number, returns electronics clock [us]
    virtual double OpticalTick2Time(double tick, size_t sample, size_t frame) const
    { return fOpticalClock.Time(sample,frame) + tick * fOpticalClock.TickPeriod(); }
    /// Given External time-tick (waveform index), sample and frame number, returns electronics clock [us]
    virtual double ExternalTick2Time(double tick, size_t sample, size_t frame) const
    { return fExternalClock.Time(sample,frame) + tick * fExternalClock.TickPeriod(); }

    //
    // Getters for time [ticks] (waveform index number)
    //

    /// Given electronics clock count [tdc] returns TPC time-tick
    virtual double TPCTDC2Tick(double tdc) const
    { return ( tdc - (TriggerTime() + TriggerOffsetTPC()) / fTPCClock.TickPeriod() ); }
    /// Given G4 time returns electronics clock count [tdc]
    virtual double TPCG4Time2Tick(double g4time) const
    { return (G4ToElecTime(g4time) - (TriggerTime() + TriggerOffsetTPC())) / fTPCClock.TickPeriod(); }

    bool InheritClockConfig() { return fInheritClockConfig; }
    
    /// Internal function to apply loaded parameters to member attributes
    void ApplyParams();

    /// Internal function used to search for the right configuration set in the data file
    bool IsRightConfig(const fhicl::ParameterSet& ps) const;
    
  protected:

    std::vector<std::string> fConfigName;

    std::vector<double>      fConfigValue;

    bool fInheritClockConfig;

    std::string fTrigModuleName;

        /// Electronics clock counting start time in G4 time frame [us]
    double fG4RefTime;

    /// Frame period
    double fFramePeriod;

    /// TPC clock
    ::util::ElecClock fTPCClock;

    /// Optical clock
    ::util::ElecClock fOpticalClock;

    /// Trigger clock
    ::util::ElecClock fTriggerClock;

    /// External clock
    ::util::ElecClock fExternalClock;

    /// Time offset from trigger to TPC readout start
    double fTriggerOffsetTPC;

    /// Trigger time in [us]
    double fTriggerTime;

    /// BeamGate time in [us]
    double fBeamGateTime;

  }; // class DetectorClocksStandard

} //namespace detinfo


#endif 
