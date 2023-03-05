/*
  ==============================================================================

    Oscillator.h    (v2)
    Created: 26 Feb 2023 5:53PM
    Author:  Chris

  ==============================================================================
*/

#include "Oscillator.h"

//public methods ==================================================================

Oscillator::Oscillator()
{

}


//sets the Oscillators sample rate data, provided by the parent SynthVoice object
void Oscillator::prepare(double sampleRate)
{
    this->sampleRate = sampleRate;
    sampleRateSet = true;
}


//Generates the next audio sample
void Oscillator::process(StereoSample& stereoSample)
{
    jassert(sampleRateSet);     //prepare() must be called to specify sample rate first!
    float sample;

    //do some math to get the next sample value
    switch (activeWaveform)
    {
        case Waveform::Sine:
            //TODO: this one is really innefficient. consider researching a faster sin() approximation
            sample = std::sin(tick() * juce::MathConstants<float>::pi);
            break;
        case Waveform::Sawtooth:
            sample = tick();
            break;
        case Waveform::Square:
            sample = tick() < pulseWidth ? -1.0f : 1.0f;
            break;
        case Waveform::Triangle:
            sample = (2 * (double)std::abs(tick())) - 1;
            break;
        default:
            jassertfalse;   //invalid waveform specified
            break;
    }

    //pan the sample across the stereo field according to pan value
    panner.process(stereoSample);


    //adds the final sample value to stereoSample for the parent SynthVoice object to move on with
    stereoSample.leftData += sample;
    stereoSample.rightData += sample;

}


//changes the pulse width used by the square wave oscillator
void Oscillator::setPulseWidth(float pulseWidth)
{
    if (pulseWidth < 0 || pulseWidth > 1)
        jassertfalse;    //pulsewidth must be between 0 and 1 inclusive
    
    this->pulseWidth = pulseWidth;
}


//selects which function to use to calculate final oscillator value
void Oscillator::setWaveShape(Oscillator::Waveform newWaveShape)
{
    activeWaveform = newWaveShape;
}


//sets oscillators current frequency and calculates deltaPhase
void Oscillator::setFrequency(float frequency)
{
    jassert(sampleRateSet);     //prepare() must be called to specify sample rate first!

    this->frequency = frequency * fineTune;

    //each time the oscillator ticks, we should increment the phase by deltaPhase
    //this value is set such that the phase will count from -1 to 1 "frequency" times per second
    //i.e. if frequency = 440, we should count from -1 to 1 440 times per second

    deltaPhase = (2 / sampleRate) * this->frequency;
}


void Oscillator::setFineTune(float factor)
{
    fineTune = factor;
}


void Oscillator::resetPhase()
{
    phase = -1;
}


void Oscillator::setPan(float pan)
{
    panner.setPan(pan);
}


//private methods =================================================================

//steps the phase forward once and return the new phase value
float Oscillator::tick()
{
    phase += deltaPhase;

    //if phase passes above 1, reset it to -1
    while (phase >= 1)
        phase -= 2;

    return phase;
}