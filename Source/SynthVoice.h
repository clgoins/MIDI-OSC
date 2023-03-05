/*
  ==============================================================================

    SynthVoice.h
    Created: 26 Feb 2023 5:13:20pm
    Author:  Chris

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"
#include "Oscillator.h"
#include "StereoSample.h"
#include "Processes.h"

class SynthVoice : public juce::SynthesiserVoice, public juce::MidiKeyboardState::Listener
{
public:
    //Functions required by Juce's SynthesiserVoice
    SynthVoice();
    bool 	canPlaySound(juce::SynthesiserSound * sound) override;
    void 	startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void 	stopNote(float velocity, bool allowTailOff) override;
    void 	pitchWheelMoved(int newPitchWheelValue) override;
    void 	controllerMoved(int controllerNumber, int newControllerValue) override;
    void 	renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples) override;
    void    prepare(double sampleRate);

    //Custom functions used by the AudioProcessorValueTreeState to propagate user changes made via the GUI
    void    setGain(float newGain);
    void    setAttack(float atk);
    void    setDecay(float dec);
    void    setSustain(float sus);
    void    setRelease(float rel);
    void    setPulseWidth(float pulseWidth);
    void    setThickness(float thickness);
    void    setWidth(float width);
    void    changeOsc(Oscillator::Waveform newOsc);

    //Functions required by Juce's MidiKeyboardState::Listener
    //These are triggered by the on-screen keyboard, and just call startNote() and stopNote(), where they're processed like a regular MIDI controller
    void 	handleNoteOn(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void 	handleNoteOff(juce::MidiKeyboardState * source, int midiChannel, int midiNoteNumber, float velocity) override;

private:
    
    //7 oscillators total
    //mainOsc is always panned center and tuned to the fundamental frequency
    Oscillator mainOsc;

    //these oscillator banks will pan out with the Width knob and detune with the Thickness knob
    Oscillator leftOsc[3];
    Oscillator rightOsc[3];

    //values for gain and adsr
    float gain = 0.25f;
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;

    //multiplying an oscillators base frequency by these constants produces a shift of +/- 25 cents (0.25 semitones)
    //these numbers were calculated using the formula: 2^(n/12) ---> where n = number of semitones = 0.25 in this case
    //these values are used by the Thickness knob to basically define a max amount of detune allowed
    //the decision to hardcode 0.25 semitones is slightly arbitrary, but it allows for a fairly substantial detune without the tone becoming unusable
    float detuneUp = 1.0145453;
    float detuneDown = 0.9856632;
};