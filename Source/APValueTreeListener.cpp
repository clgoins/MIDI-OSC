/*
  ==============================================================================

    APValueTreeListener.cpp
    Created: 23 Feb 2023 8:47:37pm
    Author:  Chris

  ==============================================================================
*/

#include "APValueTreeListener.h"

/*
    Objects of this class listen to changes made to the AudioProcessorValueTreeState(vTreeState)
    Any time the user updates a parameter via the GUI, parameterChanged() is called,
    passing in the name of the parameter and its new value as arguments
    this list of conditional statements just call the appropriate functions from SynthVoice and pass in the new parameter value

    synth->getVoice() returns an object of Juce's SynthesiserVoice type.
    The dynamic_cast() calls in each block cast that SynthesiserVoice object to a custom SynthVoice object,
    which then allows me to call all these new custom functions.
*/

APValueTreeListener::APValueTreeListener(juce::Synthesiser& synth)
{
    //keeps a reference to the main Synth object (obtained from PluginProcessor)
    //so that we can pass values to its child SynthVoice objects
    this->synth = &synth;
}

void APValueTreeListener::parameterChanged(const juce::String& parameterID, float newValue)
{

    if (parameterID == "GAIN")
    {
        for (int i = 0; i < synth->getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth->getVoice(i)))
                voice->setGain(newValue);
        }
    }

    else if (parameterID == "ATTACK")
    {
        for (int i = 0; i < synth->getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth->getVoice(i)))
                voice->setAttack(newValue);
        }
        
    }

    else if (parameterID == "DECAY")
    {
        for (int i = 0; i < synth->getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth->getVoice(i)))
                voice->setDecay(newValue);
        }
    }

    else if (parameterID == "SUSTAIN")
    {
        for (int i = 0; i < synth->getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth->getVoice(i)))
                voice->setSustain(newValue);
        }
    }

    else if (parameterID == "RELEASE")
    {
        for (int i = 0; i < synth->getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth->getVoice(i)))
                voice->setRelease(newValue);
        }
    }

    else if (parameterID == "PULSEWIDTH")
    {
        for (int i = 0; i < synth->getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth->getVoice(i)))
                voice->setPulseWidth(newValue);
        }
    }

    else if (parameterID == "OSC")
    {
        Oscillator::Waveform newOsc;

        switch ((int)newValue)
        {
            case 0:
                newOsc = Oscillator::Waveform::Sine;
                break;
            case 1:
                newOsc = Oscillator::Waveform::Square;
                break;
            case 2:
                newOsc = Oscillator::Waveform::Sawtooth;
                break;
            case 3:
                newOsc = Oscillator::Waveform::Triangle;
                break;
            default:
                jassertfalse; //invalid OSC value stored in vTreeState somehow
                break;
        }

        for (int i = 0; i < synth->getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth->getVoice(i)))
                voice->changeOsc(newOsc);
        }
    }

    else if (parameterID == "WIDTH")
    {
        for (int i = 0; i < synth->getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth->getVoice(i)))
                voice->setWidth(newValue);
        }
    }

    else if (parameterID == "THICK")
    {
        for (int i = 0; i < synth->getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth->getVoice(i)))
                voice->setThickness(newValue);
        }
    }

}