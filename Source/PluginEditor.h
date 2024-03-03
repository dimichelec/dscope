/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

using Rectangle = juce::Rectangle<float>;

//==============================================================================
/**
*/
class DscopeAudioProcessorEditor : public juce::AudioProcessorEditor,
    public juce::Timer
{
public:
    DscopeAudioProcessorEditor(DscopeAudioProcessor&);
    ~DscopeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    DscopeAudioProcessor& audioProcessor;

    int const REFRESH_RATE{ 30 };

    juce::OwnedArray<juce::TextButton> rbChannel;
    juce::OwnedArray<Rectangle> rectChannel;

    juce::Slider slAmplitude;
    Rectangle rectAmplitude;
    Rectangle rectScope;

    int vBufferIndex{ 0 };
    int vBufferIncrement{ 10 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DscopeAudioProcessorEditor)
};
