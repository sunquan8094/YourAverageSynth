//
//  YourAverageSynth.cpp
//  YourAverageSynth
//
//  Created by Derek Chan on 1/1/15.
//  Copyright (c) 2015 sunquan8094. All rights reserved.
//

// This is a test implementation of a sin wave synth using
// AUInstrumentBase classes
//
//  It illustrates a basic usage of these classes
//
//  It artificially limits the number of notes at one time to 12, so
//  the note-stealing algorithm is used - you should know how this
//  works!
//
//  Most of the work you need to do is defining a Note class (see
//  YourAverageSynthNote). AUInstrument manages the
//  creation and destruction of notes, the various stages of a note's
//  lifetime.
//
//  Alot of printfs have been left in (but are if'def out) These can
//  be useful as you figure out how this all fits together. This is
//  true in the AUInstrumentBase classes as well; simply define
//  DEBUG_PRINT to 1 and this turns all this on
//
//  The project also defines CA_AUTO_MIDI_MAP (OTHER_C_FLAGS). This
//  adds all the code that is needed to map MIDI messages to specific
//  parameter changes. This can be seen in AU Lab's MIDI Editor window
//  CA_AUTO_MIDI_MAP is implemented in AUMIDIBase.cpp/.h

#include "YourAverageSynth.h"
 
static const UInt32 kMaxActiveNotes = 8;

// Define parameter name strings here.

////////////////////////////////////////////////////////////////////////////////

#pragma mark YourAverageSynth Methods

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUMusicDeviceFactory, YourAverageSynth)

// Define the presets.
static AUPreset kPresets[kNumberOfPresets] = {
    { kPreset_Default, CFSTR("Default") },
    { kPreset_SawLead, CFSTR("Saw Lead") },
    { kPreset_Anthem, CFSTR("Anthem Chord Synth") },
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      YourAverageSynth::YourAverageSynth
//
// This synth has No inputs, One output
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
YourAverageSynth::YourAverageSynth(AudioUnit inComponentInstance)
: AUMonotimbralInstrumentBase(inComponentInstance, 0, 1)
{
    CreateElements();
        
    Globals()->UseIndexedParameters(kNumberOfParameters);
    Globals()->SetParameter(kParameter_One, 1);
    // Globals()->SetParameter(kParameter_Two, 1.0);

    SetAFactoryPresetAsCurrent(kPresets[kPreset_Default]);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      YourAverageSynth::~YourAverageSynth
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
YourAverageSynth::~YourAverageSynth()
{}

void YourAverageSynth::Cleanup()
{
#if DEBUG_PRINT
    printf("YourAverageSynth::Cleanup\n");
#endif
}

OSStatus YourAverageSynth::Initialize()
{       
#if DEBUG_PRINT
    printf("->YourAverageSynth::Initialize\n");
#endif
    AUMonotimbralInstrumentBase::Initialize();
        
    SetNotes(kNumNotes, kMaxActiveNotes, mNotes, sizeof(YourAverageSynthNote));
#if DEBUG_PRINT
    printf("<-YourAverageSynth::Initialize\n");
#endif

    return noErr;
}

AUElement* YourAverageSynth::CreateElement(AudioUnitScope   scope,
                                                        AudioUnitElement element)
{
    switch (scope) {

    case kAudioUnitScope_Group:
        return new SynthGroupElement(this, element, new MidiControls);

    case kAudioUnitScope_Part:
        return new SynthPartElement(this, element);

    default:
        return AUBase::CreateElement(scope, element);
    }
}

OSStatus YourAverageSynth::GetParameterInfo(AudioUnitScope          inScope,
                                    AudioUnitParameterID    inParameterID,
                                    AudioUnitParameterInfo &outParameterInfo)
{
    AudioUnitParameterInfo& info = outParameterInfo;
    info.flags = (kAudioUnitParameterFlag_IsWritable |
                  kAudioUnitParameterFlag_IsReadable);

    if (inScope != kAudioUnitScope_Global)
        return kAudioUnitErr_InvalidScope;

    switch (inParameterID) {

        case kParameter_One:
            AUBase::FillInParameterName (info, kParamName_One, false);
            info.unit         = kAudioUnitParameterUnit_Indexed;
            info.minValue     = 0;
            info.maxValue     = 2;
            info.defaultValue = 1;
            break;

            
        case kParameter_One_Volume:
            AUBase::FillInParameterName (info, kParamName_One_Volume, false);
            info.unit = kAudioUnitParameterUnit_Percent;
            info.minValue = 0;
            info.maxValue = 100;
            info.defaultValue = 100;
            break;
            
        case kParameter_One_Pitch:
            AUBase::FillInParameterName(info, kParamName_One_Pitch, false);
            info.unit = kAudioUnitParameterUnit_RelativeSemiTones;
            info.minValue = -64.00;
            info.maxValue = 64.00;
            info.defaultValue = 0.00;
            break;
            
        case kParameter_Two:
            AUBase::FillInParameterName (info, kParamName_Two, false);
            info.unit         = kAudioUnitParameterUnit_Indexed;
            info.minValue     = 0;
            info.maxValue     = 2;
            info.defaultValue = 1;
            break;
            
            
        case kParameter_Two_Volume:
            AUBase::FillInParameterName (info, kParamName_Two_Volume, false);
            info.unit = kAudioUnitParameterUnit_Percent;
            info.minValue = 0;
            info.maxValue = 100;
            info.defaultValue = 100;
            break;
            
        case kParameter_Two_Pitch:
            AUBase::FillInParameterName(info, kParamName_Two_Pitch, false);
            info.unit = kAudioUnitParameterUnit_RelativeSemiTones;
            info.minValue = -64.00;
            info.maxValue = 64.00;
            info.defaultValue = 0.00;
            break;
            
        case kParameter_Three:
            AUBase::FillInParameterName (info, kParamName_Three, false);
            info.unit         = kAudioUnitParameterUnit_Indexed;
            info.minValue     = 0;
            info.maxValue     = 2;
            info.defaultValue = 1;
            break;
            
        case kParameter_Three_Volume:
            AUBase::FillInParameterName (info, kParamName_Three_Volume, false);
            info.unit = kAudioUnitParameterUnit_Percent;
            info.minValue = 0;
            info.maxValue = 100;
            info.defaultValue = 100;
            break;
            
        case kParameter_Three_Pitch:
            AUBase::FillInParameterName(info, kParamName_Three_Pitch, false);
            info.unit = kAudioUnitParameterUnit_RelativeSemiTones;
            info.minValue = -64.00;
            info.maxValue = 64.00;
            info.defaultValue = 0.00;
            break;
        
        case kParameter_Master_Volume:
            AUBase::FillInParameterName(info, kParamName_Master_Volume, false);
            info.unit = kAudioUnitParameterUnit_Percent;
            info.minValue = 0;
            info.maxValue = 100;
            info.defaultValue = 100;
            break;
        
        case kParameter_Master_Pitch:
            AUBase::FillInParameterName(info, kParamName_Master_Pitch, false);
            info.unit = kAudioUnitParameterUnit_RelativeSemiTones;
            info.minValue = -64.00;
            info.maxValue = 64.00;
            info.defaultValue = 0.00;
            break;
            
        default:
            return kAudioUnitErr_InvalidParameter;
        }

    return noErr;
}


#pragma mark YourAverageSynthNote Methods

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool YourAverageSynthNote::Attack(const MusicDeviceNoteParams &inParams)
{ 
#if DEBUG_PRINT
    printf("YourAverageSynthNote::Attack %p %d\n",
           this, GetState());
#endif
    double sampleRate = SampleRate();
    phaseOne = 0.;
    phaseTwo = 0.;
    phaseThree = 0.;
    maxamp = 0.4 * pow(inParams.mVelocity/127., 3.);
    amp = maxamp;
    up_slope = maxamp / (0.1 * sampleRate);
    dn_slope = -maxamp / (0.9 * sampleRate);
    fast_dn_slope = -maxamp / (0.005 * sampleRate);
    return true;
}

void YourAverageSynthNote::Release(UInt32 inFrame)
{
    SynthNote::Release(inFrame);
#if DEBUG_PRINT
    printf("YourAverageSynthNote::Release %p %d\n",
           this, GetState());
#endif
}

void YourAverageSynthNote::FastRelease(UInt32 inFrame) // voice is being stolen.
{
    SynthNote::Release(inFrame);
#if DEBUG_PRINT
    printf("YourAverageSynthNote::Release %p %d\n",
           this, GetState());
#endif
}

Float32 YourAverageSynthNote::Amplitude()
{
    return amp;
}

void YourAverageSynthNote::Kill(UInt32 inFrame) // voice is being stolen.
{
    SynthNote::Kill(inFrame);
#if DEBUG_PRINT
    printf("YourAverageSynthNote::Kill %p %d\n",
           this, GetState());
#endif
}

OSStatus YourAverageSynthNote::Render(UInt64           inAbsoluteSampleFrame,
                                                UInt32            inNumFrames,
                                                AudioBufferList **inBufferList,
                                                UInt32            inOutBusCount)
{
    float *left, *right;
        
    // YourAverageSynthNote only writes into the first bus regardless of what is handed to us.
    const int bus0 = 0;
    int numChans = inBufferList[bus0]->mNumberBuffers;
    if (numChans > 2)
        return -1;
        
    left = (float*)inBufferList[bus0]->mBuffers[0].mData;
    right = numChans == 2 ? (float*)inBufferList[bus0]->mBuffers[1].mData : 0;

    double sampleRate = SampleRate();
    double masterPitchFactor = pow(2.0, (GetGlobalParameter(kParameter_Master_Pitch)) / 12.0);
    double freqOne = Frequency() * pow(2.0, (GetGlobalParameter(kParameter_One_Pitch)) / 12.0) * (1 / sampleRate) * masterPitchFactor;
    double freqTwo = Frequency() * pow(2.0, (GetGlobalParameter(kParameter_Two_Pitch)) / 12.0) * (1 / sampleRate) * masterPitchFactor;
    double freqThree = Frequency() * pow(2.0, (GetGlobalParameter(kParameter_Three_Pitch)) / 12.0) * (1 / sampleRate) * masterPitchFactor;
    
    double volFactorOne = (double) GetGlobalParameter(kParameter_One_Volume) / 100.0;
    double volFactorTwo = (double) GetGlobalParameter(kParameter_Two_Volume) / 100.0;
    double volFactorThree = (double) GetGlobalParameter(kParameter_Three_Volume) / 100.0;
    
    double cumFactor = pow(volFactorOne, 2.0) + pow(volFactorTwo, 2.0) + pow(volFactorThree, 2.0);
    
    double volTakeupOne = volFactorOne / cumFactor;
    double volTakeupTwo = volFactorTwo / cumFactor;
    double volTakeupThree = volFactorThree / cumFactor;
    
    double masterVolFactor = pow((double)(GetGlobalParameter(kParameter_Master_Volume) / 100.0), 2.0);
        
#if DEBUG_PRINT_RENDER
    printf("YourAverageSynthNote::Render %p %d %g %g %g\n",
           this, GetState(), phaseOne, phaseTwo, amp);
#endif
    switch (GetState()) {

    case kNoteState_Attacked :
    case kNoteState_Sostenutoed :
    case kNoteState_ReleasedButSostenutoed :
    case kNoteState_ReleasedButSustained :
        for (UInt32 frame = 0; frame < inNumFrames; frame++) {
            if (amp < maxamp)
                amp += up_slope;
            
            float sp_uno, sp_dos, sp_three;
            
            if (GetGlobalParameter(kParameter_One) == kParamName_Saw) {
                sp_uno = phaseOne - floorf(phaseOne);
            }
            else if (GetGlobalParameter(kParameter_One) == kParamName_Sin) {
                sp_uno = sinf(2 * M_PI * phaseOne);
            }
            else if (GetGlobalParameter(kParameter_One) == kParamName_Sqr) {
                sp_uno = (sinf(2 * M_PI * phaseOne) >= 0) ? 1 : -1;
            }
            
            sp_uno *= pow(volTakeupOne, 1.0);
            
            if (GetGlobalParameter(kParameter_Two) == kParamName_Saw) {
                sp_dos = (phaseTwo - floorf(phaseTwo));
            }
            else if (GetGlobalParameter(kParameter_Two) == kParamName_Sin) {
                sp_dos = sinf(2 * M_PI * phaseTwo);
            }
            else if (GetGlobalParameter(kParameter_Two) == kParamName_Sqr) {
                sp_dos = (sinf(2 * M_PI * phaseTwo) >= 0) ? 1 : -1;
            }
            
            sp_dos *= pow(volTakeupTwo, 1.0);
            
            if (GetGlobalParameter(kParameter_Three) == kParamName_Saw) {
                sp_three = (phaseThree - floorf(phaseThree));
            }
            else if (GetGlobalParameter(kParameter_Three) == kParamName_Sin) {
                sp_three = sinf(2 * M_PI * phaseThree);
            }
            else if (GetGlobalParameter(kParameter_Three) == kParamName_Sqr) {
                sp_three = (sinf(2 * M_PI * phaseThree) >= 0) ? 1 : -1;
            }
            
            sp_three *= pow(volTakeupThree, 1.0);
            
            float sp = (sp_uno + sp_dos + sp_three);
            
            float sp2 = sp * sp;
            float sp5 = sp2 * sp2 * sp;
            float out = sp5 * amp * masterVolFactor;
            phaseOne += freqOne;
            if (phaseOne > 1)
                phaseOne -= 1;
            phaseTwo += freqTwo;
            if (phaseTwo > 1)
                phaseTwo -= 1;
            phaseThree += freqThree;
            if (phaseThree > 1)
                phaseThree -= 1;
            left[frame] += out;
            if (right)
                right[frame] += out;
        }
        break;
            
    case kNoteState_Released :
        {
            UInt32 endFrame = 0xFFFFFFFF;
            for (UInt32 frame = 0; frame < inNumFrames; frame++) {
                if (endFrame == 0xFFFFFFFF)
                    endFrame = frame;
                
                float sp_uno, sp_dos, sp_three;
                
                if (GetGlobalParameter(kParameter_One) == kParamName_Saw) {
                    sp_uno = phaseOne - floorf(phaseOne);
                }
                else if (GetGlobalParameter(kParameter_One) == kParamName_Sin) {
                    sp_uno = sinf(2 * M_PI * phaseOne);
                }
                else if (GetGlobalParameter(kParameter_One) == kParamName_Sqr) {
                    sp_uno = (sinf(2 * M_PI * phaseOne) >= 0) ? 1 : -1;
                }
                
                sp_uno *= pow(volTakeupOne, 1.0);
                
                if (GetGlobalParameter(kParameter_Two) == kParamName_Saw) {
                    sp_dos = (phaseTwo - floorf(phaseTwo));
                }
                else if (GetGlobalParameter(kParameter_Two) == kParamName_Sin) {
                    sp_dos = sinf(2 * M_PI * phaseTwo);
                }
                else if (GetGlobalParameter(kParameter_Two) == kParamName_Sqr) {
                    sp_dos = (sinf(2 * M_PI * phaseTwo) >= 0) ? 1 : -1;
                }
                
                sp_dos *= pow(volTakeupTwo, 1.0);
                
                if (GetGlobalParameter(kParameter_Three) == kParamName_Saw) {
                    sp_three = (phaseThree - floorf(phaseThree));
                }
                else if (GetGlobalParameter(kParameter_Three) == kParamName_Sin) {
                    sp_three = sinf(2 * M_PI * phaseThree);
                }
                else if (GetGlobalParameter(kParameter_Three) == kParamName_Sqr) {
                    sp_three = (sinf(2 * M_PI * phaseThree) >= 0) ? 1 : -1;
                }
                
                sp_three *= pow(volTakeupThree, 1.0);
                
                float sp = (sp_uno + sp_dos + sp_three);
                
                float sp2 = sp * sp;
                float sp5 = sp2 * sp2 * sp;
                float out = sp5 * amp * masterVolFactor;
                phaseOne += freqOne;
                if (phaseOne > 1)
                    phaseOne -= 1;
                phaseTwo += freqTwo;
                if (phaseTwo > 1)
                    phaseTwo -= 1;
                phaseThree += freqThree;
                if (phaseThree > 1)
                    phaseThree -= 1;
                left[frame] += out;
                if (right)
                    right[frame] += out;
            }
            if (endFrame != 0xFFFFFFFF) {
#if DEBUG_PRINT
                printf("YourAverageSynthNote::NoteEnded  %p %d %g %g %g\n",
                       this, GetState(), phaseOne, phaseTwo, amp);
#endif
                NoteEnded(endFrame);
            }
        }
        break;
                        
    case kNoteState_FastReleased :
        {
            UInt32 endFrame = 0xFFFFFFFF;
            for (UInt32 frame = 0; frame < inNumFrames; ++frame) {
                if (amp > 0.0)
                    amp += fast_dn_slope;
                else if (endFrame == 0xFFFFFFFF)
                    endFrame = frame;
                
                float sp_uno, sp_dos, sp_three;
                
                if (GetGlobalParameter(kParameter_One) == kParamName_Saw) {
                    sp_uno = phaseOne - floorf(phaseOne);
                }
                else if (GetGlobalParameter(kParameter_One) == kParamName_Sin) {
                    sp_uno = sinf(2 * M_PI * phaseOne);
                }
                else if (GetGlobalParameter(kParameter_One) == kParamName_Sqr) {
                    sp_uno = (sinf(2 * M_PI * phaseOne) >= 0) ? 1 : -1;
                }
                
                sp_uno *= pow(volTakeupOne, 1.0);
                
                if (GetGlobalParameter(kParameter_Two) == kParamName_Saw) {
                    sp_dos = (phaseTwo - floorf(phaseTwo));
                }
                else if (GetGlobalParameter(kParameter_Two) == kParamName_Sin) {
                    sp_dos = sinf(2 * M_PI * phaseTwo);
                }
                else if (GetGlobalParameter(kParameter_Two) == kParamName_Sqr) {
                    sp_dos = (sinf(2 * M_PI * phaseTwo) >= 0) ? 1 : -1;
                }
                
                sp_dos *= pow(volTakeupTwo, 1.0);
                
                if (GetGlobalParameter(kParameter_Three) == kParamName_Saw) {
                    sp_three = (phaseThree - floorf(phaseThree));
                }
                else if (GetGlobalParameter(kParameter_Three) == kParamName_Sin) {
                    sp_three = sinf(2 * M_PI * phaseThree);
                }
                else if (GetGlobalParameter(kParameter_Three) == kParamName_Sqr) {
                    sp_three = (sinf(2 * M_PI * phaseThree) >= 0) ? 1 : -1;
                }
                
                sp_three *= pow(volTakeupThree, 1.0);
                
                float sp = (sp_uno + sp_dos + sp_three);
                
                float sp2 = sp * sp;
                float sp5 = sp2 * sp2 * sp;
                float out = sp5 * amp * masterVolFactor;
                phaseOne += freqOne;
                if (phaseOne > 1)
                    phaseOne -= 1;
                phaseTwo += freqTwo;
                if (phaseTwo > 1)
                    phaseTwo -= 1;
                phaseThree += freqThree;
                if (phaseThree > 1)
                    phaseThree -= 1;
                left[frame] += out;
                if (right)
                    right[frame] += out;
            }
            if (endFrame != 0xFFFFFFFF) {
#if DEBUG_PRINT
                printf("YourAverageSynthNote::NoteEnded  %p %d %g %g\n",
                       this, GetState(), phase, amp);
#endif
                NoteEnded(endFrame);
            }
        }
        break;

    default :
        break;
    }

    return noErr;
}

OSStatus YourAverageSynth::GetParameterValueStrings(AudioUnitScope scope, AudioUnitParameterID id, CFArrayRef *outStrings) {
    if (scope == kAudioUnitScope_Global && (id == kParameter_One || id == kParameter_Two || id == kParameter_Three)) {
        if (outStrings == NULL) return noErr;
        CFStringRef strings [] = { kMenuItem_Sin, kMenuItem_Saw, kMenuItem_Sqr};
        *outStrings = CFArrayCreate (NULL, (const void **) strings, (sizeof (strings) / sizeof (strings [0])), NULL);
        return noErr;
    }
    return kAudioUnitErr_InvalidParameter;
}
