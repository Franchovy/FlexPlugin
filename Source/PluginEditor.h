/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomProcessors.h"

//==============================================================================

/**
*/
class FlexPluginAudioProcessorEditor  : public AudioProcessorEditor
        , public Button::Listener
{
public:
    FlexPluginAudioProcessorEditor (FlexPluginAudioProcessor&,  AudioProcessorValueTreeState&);
    ~FlexPluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FlexPluginAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;

    //==============================================================================

    // Combobox select effect type - create()
    ComboBox effectSelectBox;
    // Create effect button - on click()
    TextButton addEffectButton;

    void buttonClicked(Button* button) override;

    // Array of component groups - create()
    //OwnedArray<AudioProcessor> effects;
    OwnedArray<GroupComponent> effectGroupComponentList;

    // Parameter array
//    template <class Component>
//    struct Parameter {
//        Parameter(Component* c){
//
//        }
//    };
//    OwnedArray<Slider> sliders;
//    OwnedArray<Label> labels;
//    OwnedArray<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> attachments;

    GroupComponent groupComponent;

    //==============================================================================

    Label gainLabel;
    Slider gainSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    Label phaseLabel;
    ToggleButton phaseButton;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> phaseAttachment;

    Label echoLabel;
    Slider echoSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> echoAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlexPluginAudioProcessorEditor)
};
