/*
  ==============================================================================

    Oscillator.h    (v2)
    Created: 26 Feb 2023 5:53PM
    Author:  Chris

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "StereoSample.h"
#include "Processes.h"

class Oscillator
{
public:

    enum Waveform
    {
        Sine, Sawtooth, Square, Triangle
    };

    Oscillator();

    //provides Oscillator with current audio sample rate
    //Must be provided to generate accurate frequencies
    void prepare(double sampleRate);

    //frequency of tone generated
    void setFrequency(float frequency);

    //Generates the next sample. Call repeatedly and pass return value to audio buffer to generate a tone.
    void process(StereoSample& stereoSample);

    //adjusts the pulse width of the square wave produced.
    //Should be between 0.0 and 1.0. ---> 0.0 will create a perfect square wave
    void setPulseWidth(float pulseWidth);   

    //Defines the shape of the wave created by Process
    void setWaveShape(Oscillator::Waveform newWaveShape);

    //the value passed here gets multiplied by the base frequency to detune the oscillator
    //e.g. a value of 2 will tune the osc up an octave and a value of 0.5 will tune it down an octave
    //tuning n semitones would be a value equal to 2 ^ (n / 12)
    void setFineTune(float factor);
    
    //sets the pan value for this oscillator, from -1 (hard left) to 1 (hard right)
    void setPan(float pan);

    //resets phase back to -1
    void resetPhase();




private:
    
    float tick();

    //current phase position
    float phase = -1;

    //how much the phase needs to change each tick
    float deltaPhase = 0;

    //audio sampleRate. MUST BE SET with prepare() before doing anything else
    double sampleRate = 0;
    bool sampleRateSet = false;

    //frequency of the note to playback
    float frequency = 0;

    //multiplier for frequency to get some cool sounds
    float fineTune = 1;

    //pulse width for square waves
    float pulseWidth = 0;

    //instance of Pan class to pan individual oscillators left and right
    Pan panner;
    
    //which wave shape to generate. current options are Sine, Square, Sawtooth, and Triangle
    Waveform activeWaveform = Waveform::Sine;

};