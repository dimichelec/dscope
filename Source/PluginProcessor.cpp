/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DscopeAudioProcessor::DscopeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
        #if ! JucePlugin_IsMidiEffect
            #if ! JucePlugin_IsSynth
                .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
            #endif
            .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
        #endif
       )
#endif
{
}

DscopeAudioProcessor::~DscopeAudioProcessor()
{
}

//==============================================================================
const juce::String DscopeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DscopeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DscopeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DscopeAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DscopeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DscopeAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DscopeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DscopeAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DscopeAudioProcessor::getProgramName (int index)
{
    return {};
}

void DscopeAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DscopeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    vBuffer.setSize(getNumInputChannels(), samplesPerBlock * vBufferBlocks);
}

void DscopeAudioProcessor::releaseResources()
{
    vBuffer.clear();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DscopeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
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

void DscopeAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
        vBuffer.copyFrom(channel, vBufferBlock * buffer.getNumSamples(), buffer, channel, 0, buffer.getNumSamples());
    
    if (++vBufferBlock >= vBufferBlocks) vBufferBlock = 0;
}

//==============================================================================
bool DscopeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DscopeAudioProcessor::createEditor()
{
    return new DscopeAudioProcessorEditor (*this);
}

//==============================================================================
void DscopeAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DscopeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DscopeAudioProcessor();
}
