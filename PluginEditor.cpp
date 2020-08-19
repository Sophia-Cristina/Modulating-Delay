/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

// MEU:
#include "ysxCImg.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor(NewProjectAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    // #################################################
    // #################################################
    // #################################################
    // ADD COMPONENTS HERE:

    addAndMakeVisible(MainSliders);
    MainSliders.MSSlider.addListener(this);
    MainSliders.FeedBack.addListener(this);
    MainSliders.Floor.addListener(this);
    MainSliders.Multiplier.addListener(this);
    MainSliders.Omega.addListener(this);

    // AudioProssTree:

    // #################################################
    // #################################################
    // #################################################

    // SET SIZE:
    setSize(300, 600);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

void NewProjectAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    //processor.noteOnVel = midiVolume.getValue();
    audioProcessor.Delay.MSNum = MainSliders.MSSlider.getValue();
    audioProcessor.Delay.FeedBNum = MainSliders.FeedBack.getValue() / 100.0;
    audioProcessor.Delay.FloorNum = MainSliders.Floor.getValue();
    audioProcessor.Delay.MultiNum = MainSliders.Multiplier.getValue();
    audioProcessor.Delay.Omega = MainSliders.Omega.getValue();
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
   // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::white);
    juce::Colour Pink(255, 0, 127);
    g.setColour(Pink);
    g.setFont(juce::Font(16.0f)); g.drawText("ChanDataVal: " + to_string(audioProcessor.ChanDataVal), getLocalBounds(), juce::Justification::centredBottom, true);
    g.drawText("DelayBFSize: " + to_string(audioProcessor.Delay.DelayBFSize), 10, 550, getWidth(), 30, juce::Justification::centredBottom, true);
    //g.drawText("SampleRate: " + to_string(audioProcessor.Delay.SampleRate), 10, 520, getWidth(), 30, juce::Justification::centredBottom, true);
    g.drawText("MSNum: " + to_string(audioProcessor.Delay.MSNum), 10, 520, getWidth(), 30, juce::Justification::centredBottom, true);
    g.drawText("DelayBuffer Num. Samples: " + to_string(audioProcessor.Delay.DelayBuffer.getNumSamples()), 10, 490, getWidth(), 30, juce::Justification::centredBottom, true);
    g.drawText("Delay Buffer Size: " + to_string(audioProcessor.Delay.DelayBFDataSizeView), 10, 460, getWidth(), 30, juce::Justification::centredBottom, true);
    g.setColour({ 255, 0, 0 });
    g.drawText("WRITE POS: " + to_string(audioProcessor.Delay.WritePos), 10, 430, getWidth(), 30, juce::Justification::centredBottom, true);
    g.setOpacity(1.0f);
}

void NewProjectAudioProcessorEditor::resized()
{
    //MainSliders.setBounds(10, 200, SldX.getX(), 50);
    MainSliders.setBounds(10, 10, 280, 360);

    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
