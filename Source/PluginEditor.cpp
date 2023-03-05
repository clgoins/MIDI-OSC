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

    //divides the screen into 15 even spaces
    juce::Rectangle<int> slices[15];
    for (int i = 0; i < 15; i++)
    {
            slices[i].setX(i * getWidth()/15);
            slices[i].setY(25);
            slices[i].setWidth(getWidth() / 15);
            slices[i].setHeight(getHeight() / 2 - 50);

            //shrinks the rectangles that will serve as spaces between components
            if (i % 2 == 0)
            {
                slices[i].reduce(getWidth() / 60, 0);
            }
            //grow the rectangles that hold the actual components to fill the empty space
            else
            {
                slices[i].expand(getWidth() / 60, 0);
            }
    }

    //set screen size  & position of each component
    gainSlider.setBounds(slices[1]);
    atkKnob.setBounds(slices[3].removeFromBottom(slices[3].getHeight() / 2));
    decKnob.setBounds(slices[5].removeFromBottom(slices[5].getHeight() / 2));
    susKnob.setBounds(slices[7].removeFromBottom(slices[7].getHeight() / 2));
    relKnob.setBounds(slices[9].removeFromBottom(slices[9].getHeight() / 2));

    //this adds up the width of the ADSR rectangles so oscSelect can sit above them
    int oscWidth = 0;
    for (int i = 3; i < 10; i++)
    {
        oscWidth += slices[i].getWidth();
    }

    oscSelect.setBounds(slices[3].getX(), slices[3].getY(), oscWidth, slices[3].getHeight()-25);

    pulseSlider.setBounds(slices[11]);

    //i want the thickness & width knobs to sit on top of each other
    thicknessKnob.setBounds(slices[13].removeFromTop(slices[13].getHeight() / 2));
    widthKnob.setBounds(slices[13].getX(),slices[13].getY()+25, slices[13].getWidth(),slices[13].getHeight());


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
