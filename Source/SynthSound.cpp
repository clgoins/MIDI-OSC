/*
  ==============================================================================

    SynthSound.cpp
    Created: 26 Feb 2023 5:13:14pm
    Author:  Chris

  ==============================================================================
*/

//My implementation of this class doesn't do much, but it's required by Juce's Synthesiser object

#include "SynthSound.h"

SynthSound::SynthSound()
{

}


bool SynthSound::appliesToNote(int midiNoteNumber)
{
    return true;
}


bool SynthSound::appliesToChannel(int midiChannel)
{
    return true;
}