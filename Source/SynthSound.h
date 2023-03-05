/*
  ==============================================================================

    SynthSound.h
    Created: 26 Feb 2023 5:13:14pm
    Author:  Chris

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
    public:

        //pure abstract functions required by Juce's SynthesiserSound class
        SynthSound();
        bool appliesToNote      (int midiNoteNumber)    override;
        bool appliesToChannel   (int midiChannel)       override;

};