/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DscopeAudioProcessorEditor::DscopeAudioProcessorEditor(DscopeAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), Timer()
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 252);

    // scope rectangle
    rectScope = Rectangle(2, 0, 300, 250);

    int x = rectScope.getWidth() + 4;
    int width = getWidth() - rectScope.getRight() - 2;

    // setup L/R channel toggle buttons
    auto* b = rbChannel.add(new juce::TextButton("L"));
    addAndMakeVisible(b);
    b->setRadioGroupId(1);
    b->setClickingTogglesState(true);
    b->setConnectedEdges(juce::Button::ConnectedOnRight);
    rectChannel.add(new Rectangle(x, 20, width / 2, 20));

    b = rbChannel.add(new juce::TextButton("R"));
    addAndMakeVisible(b);
    b->setRadioGroupId(1);
    b->setClickingTogglesState(true);
    b->setConnectedEdges(juce::Button::ConnectedOnLeft);
    rectChannel.add(new Rectangle(rectChannel[0]->translated(rectChannel[0]->getWidth() - 1, 0)));

    rbChannel[1]->setToggleState(true, juce::NotificationType::dontSendNotification);

    // setup amplitude knob
    rectAmplitude = Rectangle(x - 2, rectChannel[0]->getBottom() + 30, width + 2, 100);
    addAndMakeVisible(slAmplitude);
    slAmplitude.setRange(1, 100, 1);
    slAmplitude.setValue(10);
    slAmplitude.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slAmplitude.setTextBoxStyle(juce::Slider::TextBoxBelow, true, rectAmplitude.getWidth(), 20);
    slAmplitude.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0));

    startTimer(1000 / REFRESH_RATE);
}

DscopeAudioProcessorEditor::~DscopeAudioProcessorEditor()
{
}

//==============================================================================
void DscopeAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::darkorange);
    g.drawLine(rectScope.getX(), rectScope.getCentreY(), rectScope.getRight(), rectScope.getCentreY(), 0.2);

    g.setColour(juce::Colours::white);
    float v_mid = rectScope.getHeight() / 2;
    int channel = (rbChannel[0]->getToggleState()) ? 0 : 1;

    float x0 = rectScope.getX() + 1, y0 = v_mid, y1 = rectScope.getY();
    for (int x = rectScope.getX() + 1; x < rectScope.getRight(); x++)
    {
        y1 = juce::jlimit<float>(
            0.0f,
            rectScope.getHeight(),
            v_mid + (audioProcessor.vBuffer.getSample(channel, vBufferIndex) * slAmplitude.getValue() * 100.0f)
        );
        g.drawLine(x0, y0, x, y1, 0.5f);
        x0 = x;
        y0 = y1;
        vBufferIndex = (vBufferIndex + vBufferIncrement) % audioProcessor.vBuffer.getNumSamples();
    }
    g.setColour(juce::Colours::darkorange);
    g.drawRoundedRectangle(rectScope, 10, 0.5);

    Rectangle* r = rectChannel[0];
    g.drawText("CHANNEL", r->getX(), r->getY() - 20, r->getWidth() * 2, 20, juce::Justification::centred, false);
    rbChannel[0]->setBounds(rectChannel[0]->toNearestInt());
    rbChannel[1]->setBounds(rectChannel[1]->toNearestInt());

    r = &rectAmplitude;
    g.drawText("AMPLITUDE", r->getX(), r->getY() - 16, r->getWidth(), 20, juce::Justification::centred, false);
    slAmplitude.setBounds(rectAmplitude.toNearestInt());
}

void DscopeAudioProcessorEditor::resized()
{

}

void DscopeAudioProcessorEditor::timerCallback()
{
    repaint();
}

void sliderValueChanged(juce::Slider* slider)
{

}