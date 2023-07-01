/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
BASICOSCAudioProcessor::BASICOSCAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), vTreeState (*this, nullptr, "Parameters", createParameters())
#endif
{

    //adds vTreeListener as a listener to vTreeState for each parameter; to detect user changes
    vTreeState.addParameterListener("GAIN", &vTreeListener);
    vTreeState.addParameterListener("ATTACK", &vTreeListener);
    vTreeState.addParameterListener("DECAY", &vTreeListener);
    vTreeState.addParameterListener("SUSTAIN", &vTreeListener);
    vTreeState.addParameterListener("RELEASE", &vTreeListener);
    vTreeState.addParameterListener("PULSEWIDTH", &vTreeListener);
    vTreeState.addParameterListener("OSC", &vTreeListener);
    vTreeState.addParameterListener("WIDTH", &vTreeListener);
    vTreeState.addParameterListener("THICK", &vTreeListener);

    //adds a single sound to the synth and 4 voices
    //4 voice polyphony for those sweet sweet add9 chords
    mainSynth.addSound(new SynthSound());
    mainSynth.addVoice(new SynthVoice());
    mainSynth.addVoice(new SynthVoice());
    mainSynth.addVoice(new SynthVoice());
    mainSynth.addVoice(new SynthVoice());

    //adds a MidiKeyboard listener to each synth voice to listen for 
    //MIDI events coming from the GUI keyboard
    for (int i = 0; i < mainSynth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(mainSynth.getVoice(i)))
        {
            keyState.addListener(voice);
        }
    }

}

BASICOSCAudioProcessor::~BASICOSCAudioProcessor()
{
}


//called anytime audio playback begins
//passes sample rate data off to the main synth and each of its voices
void BASICOSCAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    mainSynth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < mainSynth.getNumVoices(); i++)
    {
        //getVoice() returns a juce::SynthesiserVoice object.
        //need to cast that to a custom SynthVoice object before I can access any of my custom functions
        if (auto voice = dynamic_cast<SynthVoice*>(mainSynth.getVoice(i)))
        {
            voice->prepare(sampleRate);
        }
    }
}


//processBlock is called constantly, and allows reading & writing of midi messages and audio stream data 
void BASICOSCAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    //clears output buffers that may not have been written to
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());



    //this plugin only consists of the one synthesiser, so the custom processing code just needs to call renderNextBlock()
    //this function iterates through each SynthVoice attached to the mainSynth and calls their renderNextBlock() function in turn
    mainSynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}


//Creates a list of parameters to expose to the GUI for the user to manipulate
juce::AudioProcessorValueTreeState::ParameterLayout BASICOSCAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    //Main Gain Control
    params.push_back(std::make_unique <juce::AudioParameterFloat> ("GAIN", "Gain", 0.0f, 1.0f, 0.2f));
    
    //ADSR Controls
    params.push_back(std::make_unique <juce::AudioParameterFloat> ("ATTACK", "Attack", 0.01f, 2.0f, 0.01f));
    params.push_back(std::make_unique <juce::AudioParameterFloat> ("DECAY", "Decay", 0.01f, 2.0f, 0.01f));
    params.push_back(std::make_unique <juce::AudioParameterFloat> ("SUSTAIN", "Sustain", 0.0f, 1.0f, 1.0f));
    params.push_back(std::make_unique <juce::AudioParameterFloat> ("RELEASE", "Release", 0.01f, 5.0f, 0.01f));

    //Square Wave Pulse Width Control
    params.push_back(std::make_unique <juce::AudioParameterFloat>("PULSEWIDTH", "Pulse Width", 0, 1, 0.0f));

    //Oscilator Selector
    params.push_back(std::make_unique <juce::AudioParameterChoice>("OSC", "Osc", juce::StringArray{ "Sine", "Square", "Saw", "Triangle" }, 0));

    //Width and Thickness
    params.push_back(std::make_unique <juce::AudioParameterFloat> ("THICK", "Thickness", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique <juce::AudioParameterFloat>("WIDTH", "Width", 0.0f, 1.0f, 0.0f));

    return { params.begin(), params.end() };
}


//Saves and loads plugin state when saving/loading project in host DAW
void BASICOSCAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    if (auto xml = vTreeState.state.createXml())
        copyXmlToBinary(*xml, destData);

}

void BASICOSCAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        vTreeState.state = juce::ValueTree::fromXml(*xml);
}



//The rest of these are generated by JUCE and are unmodified by me.

const juce::String BASICOSCAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BASICOSCAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool BASICOSCAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool BASICOSCAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double BASICOSCAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BASICOSCAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BASICOSCAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BASICOSCAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String BASICOSCAudioProcessor::getProgramName(int index)
{
    return {};
}

void BASICOSCAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

void BASICOSCAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BASICOSCAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

//==============================================================================
bool BASICOSCAudioProcessor::hasEditor() const
{
    return true;
}


juce::AudioProcessorEditor* BASICOSCAudioProcessor::createEditor()
{
    return new BASICOSCAudioProcessorEditor(*this);
}


juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BASICOSCAudioProcessor();
}