//
//  YourAverageSynth.h
//  YourAverageSynth
//
//  Created by Derek Chan on 1/1/15.
//  Copyright (c) 2015 sunquan8094. All rights reserved.
//

#ifndef __YourAverageSynth__
#define __YourAverageSynth__

#include "AUInstrumentBase.h"
#include "YourAverageSynthVersion.h"

static const UInt32 kNumNotes = 12;

static CFStringRef kParamName_One = CFSTR("Waveform 1");
static CFStringRef kParamName_One_Pitch = CFSTR("Pitch 1");
static CFStringRef kParamName_One_Volume = CFSTR("Volume 1");

static CFStringRef kParamName_Two = CFSTR("Waveform 2");
static CFStringRef kParamName_Two_Pitch = CFSTR("Pitch 2");
static CFStringRef kParamName_Two_Volume = CFSTR("Volume 2");

static CFStringRef kParamName_Three = CFSTR("Waveform 3");
static CFStringRef kParamName_Three_Pitch = CFSTR("Pitch 3");
static CFStringRef kParamName_Three_Volume = CFSTR("Volume 3");

static CFStringRef kParamName_Master_Volume = CFSTR("Master Volume");
static CFStringRef kParamName_Master_Pitch = CFSTR("Master Pitch");
static CFStringRef kParamName_Master_FilterType = CFSTR("Master Filter Type");
static CFStringRef kParamName_Master_FilterCutoff = CFSTR("Master Filter Cutoff");

static const int kParamName_Sin = 0;
static const int kParamName_Saw = 1;
static const int kParamName_Sqr = 2;
static const int kDefault_Wav = 1;

static const int kParamName_lp = 0;
static const int kParamName_hp = 1;

static const float kDefault_Pitch = 0.0;
static const int kDefault_Volume = 100;

static CFStringRef kMenuItem_Sin = CFSTR("Sine");
static CFStringRef kMenuItem_Saw = CFSTR("Saw");
static CFStringRef kMenuItem_Sqr = CFSTR("Square");

static CFStringRef kMenuItem_Lowpass = CFSTR("Lowpass");
static CFStringRef kMenuItem_Highpass = CFSTR("Highpass");

// Define constants to identify the parameters.
enum Parameter {
    kParameter_One = 0,
    kParameter_One_Pitch = 1,
    kParameter_One_Volume = 2,
    
    kParameter_Two = 3,
    kParameter_Two_Pitch = 4,
    kParameter_Two_Volume = 5,
    
    kParameter_Three = 6,
    kParameter_Three_Pitch = 7,
    kParameter_Three_Volume = 8,
    
    kParameter_Master_Volume = 9,
    kParameter_Master_Pitch = 10,
    kParameter_Master_FilterType = 11,
    kParameter_Master_FilterCutoff = 12,
    
    kNumberOfParameters
};

// Define constants to identify factory presets.
enum Preset {
    kPreset_Default = 0,
    kPreset_SawLead  = 1,
    kPreset_Anthem  = 2,
    kNumberOfPresets
};

struct YourAverageSynthNote : public SynthNote {

 public:
    virtual         ~YourAverageSynthNote() {}

    virtual bool     Attack(const MusicDeviceNoteParams &inParams);
    virtual void     Kill(UInt32 inFrame); // voice is being stolen.
    virtual void     Release(UInt32 inFrame);
    virtual void     FastRelease(UInt32 inFrame);
    virtual Float32  Amplitude();
    virtual OSStatus Render(UInt64            inAbsoluteSampleFrame,
                            UInt32            inNumFrames,
                            AudioBufferList** inBufferList,
                            UInt32            inOutBusCount);

 private:
    double phaseOne, phaseTwo, phaseThree, amp, maxamp;
    double up_slope, dn_slope, fast_dn_slope;

};

class YourAverageSynth : public AUMonotimbralInstrumentBase
{
public:
                       YourAverageSynth(AudioUnit inComponentInstance);
    virtual           ~YourAverageSynth();
                                
    virtual OSStatus   Initialize();
    virtual void       Cleanup();
    virtual OSStatus   Version() { return kYourAverageSynthVersion; }

    virtual AUElement *CreateElement(AudioUnitScope   scope,
                                     AudioUnitElement element);

    virtual OSStatus GetParameterInfo(AudioUnitScope          inScope,
                                    AudioUnitParameterID    inParameterID,
                                    AudioUnitParameterInfo &outParameterInfo);
    virtual OSStatus GetParameterValueStrings(AudioUnitScope scope, AudioUnitParameterID, CFArrayRef *outStrings);
private:
    YourAverageSynthNote mNotes[kNumNotes];

};

#endif /* !__YourAverageSynth__ */
