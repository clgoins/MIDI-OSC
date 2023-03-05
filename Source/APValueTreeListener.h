/*
  ==============================================================================

    APValueTreeLIstener.h
    Created: 23 Feb 2023 8:11:07pm
    Author:  Chris

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthVoice.h"

//Listener Object for vTreeState
//Any time a parameter in vTreeState is changed, parameterChanged() is called
//This object keeps a reference to the main Synthesiser object to pass values to it's child SynthVoice objects

class APValueTreeListener : public juce::AudioProcessorValueTreeState::Listener
{


public:
    APValueTreeListener(juce::Synthesiser& synth);

private:

    void parameterChanged(const juce::String& parameterID, float newValue);
    juce::Synthesiser* synth;
};