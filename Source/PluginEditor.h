/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class BASICOSCAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BASICOSCAudioProcessorEditor (BASICOSCAudioProcessor&);
    ~BASICOSCAudioProcessorEditor() override;

    //Juce functions for painting and resizing the GUI window
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BASICOSCAudioProcessorEditor)

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BASICOSCAudioProcessor& audioProcessor;

    //custom functions for quickly setting parameters for a bunch of knobs and sliders that all basically look the same
    void setUpSlider(juce::Slider& s, juce::Label& l, juce::String text);
    void setUpKnob(juce::Slider& s, juce::Label& l, juce::String text);

    //all of the knobs and sliders
    juce::Label gainLabel;
    juce::Slider gainSlider;
    
    juce::Label atkLabel;
    juce::Slider atkKnob;

    juce::Label decLabel;
    juce::Slider decKnob;

    juce::Label susLabel;
    juce::Slider susKnob;

    juce::Label relLabel;
    juce::Slider relKnob;

    juce::Label pulseLabel;
    juce::Slider pulseSlider;

    juce::ComboBox oscSelect;
    juce::Label oscLabel;

    juce::Label widthLabel;
    juce::Slider widthKnob;

    juce::Label thicknessLabel;
    juce::Slider thicknessKnob;

    //GUI keyboard component
    juce::MidiKeyboardComponent keyboard { audioProcessor.keyState, juce::MidiKeyboardComponent::horizontalKeyboard };

    //Create attachments from each UI element to a parameter in the AudioProcessorValueTreeState
    //These parameters are 1:1 with the ones defined in createParameters() in PluginProcessor.cpp
    //These are just the handles, the actual attachment is established in the PluginEditor constructor
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment >    gainAttach;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment >    atkAttach;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment >    decAttach;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment >    susAttach;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment >    relAttach;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment >    pulseAttach;
    std::unique_ptr < juce::AudioProcessorValueTreeState::ComboBoxAttachment >  oscAttach;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment >    widthAttach;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment >    thickAttach;

};
