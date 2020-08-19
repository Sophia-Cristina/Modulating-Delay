/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SlidersHeader.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged(juce::Slider* MySlider) override;

    // COMPONENTS:
    SlidersHeader MainSliders;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NewProjectAudioProcessor& audioProcessor;

    //juce::AudioProcessorValueTreeState State;
    
    // This is used to make presets and stuffs:
    //juce::AudioProcessorValueTreeState::SliderAttachment MSSliderAttach(State, "MSID", MSSlider);
    //juce::AudioProcessorValueTreeState::SliderAttachment MSSliderAttach;

    // DESIGN:


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};
