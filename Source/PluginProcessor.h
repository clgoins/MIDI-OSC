/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "APValueTreeListener.h"
#include "SynthSound.h"
#include "SynthVoice.h"
#include "StereoSample.h"

//==============================================================================
/**
*/
class BASICOSCAudioProcessor : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
    , public juce::AudioProcessorARAExtension
#endif
{
public:
    //==============================================================================
    BASICOSCAudioProcessor();
    ~BASICOSCAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;



    //Value Tree to connect back end stuff with GUI elements
    juce::AudioProcessorValueTreeState vTreeState;

    //Keeps track of on-screen midi keyboard
    juce::MidiKeyboardState keyState;



private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BASICOSCAudioProcessor)



    //populates vTreeState with the parameters we want to expose to the user
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    //Main Synthesizer
    juce::Synthesiser mainSynth;

    //Listens for changes to vTreeState values. 
    //Needs reference to synthsiser object to propagate changes to the SynthVoice objects
    APValueTreeListener vTreeListener { mainSynth };


};
