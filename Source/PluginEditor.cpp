/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
BASICOSCAudioProcessorEditor::BASICOSCAudioProcessorEditor (BASICOSCAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //Set editor size
    setSize (750,400);

    //create and add gainSlider
    setUpSlider(gainSlider, gainLabel, "Gain");

    //create and add ADSR controls
    setUpKnob(atkKnob, atkLabel, "ATK");
    setUpKnob(decKnob, decLabel, "DEC");
    setUpKnob(susKnob, susLabel, "SUS");
    setUpKnob(relKnob, relLabel, "REL");

    //create and add pulse width slider
    setUpSlider(pulseSlider, pulseLabel, "Pulse Width");

    //create and add thickness & width knobs
    setUpKnob(thicknessKnob, thicknessLabel, "Thickness");
    setUpKnob(widthKnob, widthLabel, "Width");

    //create and add oscillator selector
    oscSelect.addItem("Sine", 1);
    oscSelect.addItem("Pulse", 2);
    oscSelect.addItem("Saw", 3);
    oscSelect.addItem("Triangle", 4);
    oscLabel.setText("Oscillator", juce::dontSendNotification);
    oscLabel.setJustificationType(juce::Justification::centred);
    oscLabel.attachToComponent(&oscSelect, false);
    addAndMakeVisible(oscSelect);

    //create and add GUI keyboard component
    addAndMakeVisible(keyboard);

    //Establish the connections between our GUI elements and AudioProcessorValueTreeState parameters
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    gainAttach  = std::make_unique<SliderAttachment> ( audioProcessor.vTreeState, "GAIN",       gainSlider );
    atkAttach   = std::make_unique<SliderAttachment> ( audioProcessor.vTreeState, "ATTACK",     atkKnob );
    decAttach   = std::make_unique<SliderAttachment> ( audioProcessor.vTreeState, "DECAY",      decKnob );
    susAttach   = std::make_unique<SliderAttachment> ( audioProcessor.vTreeState, "SUSTAIN",    susKnob );
    relAttach   = std::make_unique<SliderAttachment> ( audioProcessor.vTreeState, "RELEASE",    relKnob );
    pulseAttach = std::make_unique<SliderAttachment> ( audioProcessor.vTreeState, "PULSEWIDTH", pulseSlider );
    widthAttach = std::make_unique<SliderAttachment>(audioProcessor.vTreeState,   "WIDTH",      widthKnob);
    thickAttach = std::make_unique<SliderAttachment>(audioProcessor.vTreeState,   "THICK",      thicknessKnob);
    oscAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ( audioProcessor.vTreeState, "OSC", oscSelect );
}


BASICOSCAudioProcessorEditor::~BASICOSCAudioProcessorEditor()
{
}


//Paint defines the look of this parent component
void BASICOSCAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Fill background with solid color
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));


}


//resized defines the size and position of each child component
void BASICOSCAudioProcessorEditor::resized()
{
    //manually setting the size & position of each control
    gainSlider.setBounds(38, 25, 74, 150);
    atkKnob.setBounds(138, 100, 74, 75);
    decKnob.setBounds(238, 100, 74, 75);
    susKnob.setBounds(338, 100, 74, 75);
    relKnob.setBounds(438, 100, 74, 75);
    oscSelect.setBounds(138, 25, 374, 50);
    pulseSlider.setBounds(538, 25, 74, 150);
    thicknessKnob.setBounds(638, 25, 74, 75);
    widthKnob.setBounds(638, 125, 74, 75);

    //GUI keyboard should span the width of the screen and sit in the bottom half.
    keyboard.setBounds(0,200,750,200);


}

//These two functions take a reference to a juce::Slider and a juce::Label and gives them some default properties

//This one sets the slider up as a Rotary knob
void BASICOSCAudioProcessorEditor::setUpKnob(juce::Slider& k, juce::Label& l, juce::String text)
{
    k.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    k.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    l.setText(text, juce::dontSendNotification);
    l.setJustificationType(juce::Justification::centred);
    l.attachToComponent(&k, false);
    addAndMakeVisible(k);
}

//This one sets the slider up as a vertical bar
void BASICOSCAudioProcessorEditor::setUpSlider(juce::Slider& s, juce::Label& l, juce::String text)
{
    s.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    s.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    l.setText(text, juce::dontSendNotification);
    l.setJustificationType(juce::Justification::centred);
    l.attachToComponent(&s, false);
    addAndMakeVisible(s);
}
