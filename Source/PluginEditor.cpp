/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FlexPluginAudioProcessorEditor::FlexPluginAudioProcessorEditor (FlexPluginAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p)
        , valueTreeState(vts)
        //, Button::Listener()
{
    //==========================================================================
    // Fill effects array
    // create standard group components for effects of the array
    //TODO link effects to group components through value tree

    //effectSelectBox.addItem("Item 1", 0);
    effectSelectBox.addItem("Item 1", 1);
    effectSelectBox.addItem("Item 2", 2);
    effectSelectBox.setText("Select Effect", dontSendNotification);
    addAndMakeVisible(effectSelectBox);

    addEffectButton.setButtonText("Add Effect");
    addAndMakeVisible(addEffectButton);
//    addEffectButton.onClick = [this]{
//    };
    addEffectButton.addListener(this);

    groupComponent.setText("Group 1");
    addAndMakeVisible(groupComponent);
    //groupComponent.addChildComponent(gainSlider);

    effectGroupComponentList.add(&groupComponent);

    GroupComponent* echoGroupComponent = new GroupComponent();
    echoGroupComponent->setText("Echo Effect");
    addAndMakeVisible(echoGroupComponent);
    effectGroupComponentList.add( echoGroupComponent);



    //==========================================================================

    gainLabel.setText("Gain", dontSendNotification);
    addAndMakeVisible(gainLabel);
    groupComponent.addAndMakeVisible(gainSlider);
    gainAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "gain", gainSlider));

    echoLabel.setText("Echo", dontSendNotification);
    addAndMakeVisible(echoLabel);
    echoGroupComponent->addAndMakeVisible(echoSlider);
    echoAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "echo", echoSlider));

    phaseButton.setButtonText("Invert Phase");
    addAndMakeVisible(phaseButton);
    phaseAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "invertPhase", phaseButton));


    std::cout << "Tests:" << newLine;
    std::cout << "NumProperties: " << vts.state.getNumProperties() << newLine;


    for (int i = 0; vts.state.getNumProperties(); i++){
        // No need for type checking?
        auto parameterName = vts.state.getPropertyName(i);
        std::cout << "Parameter " << i << " name: " << parameterName.toString() << newLine;
    }
    std::cout << "end results" << newLine;


    setSize (400, 300);
}

FlexPluginAudioProcessorEditor::~FlexPluginAudioProcessorEditor()
{
}

//==============================================================================
void FlexPluginAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));



    g.setColour (Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void FlexPluginAudioProcessorEditor::resized()
{
    auto leftMargin = 30;
    auto middleMargin = 10;
    auto rightMargin = 30;
    auto heightMargin = 10;
    auto splitWidth = getWidth() / 3;
    effectSelectBox.setBounds(leftMargin, 20, splitWidth - leftMargin, 20);
    addEffectButton.setBounds(splitWidth, 20, splitWidth - rightMargin, 20);

    int currentHeight = 50;
    for (auto component : effectGroupComponentList)
    {
        std::cout << component->getName();
        // default height //TODO must be customized for each group
        int componentHeight = 60;
        component->setBounds(leftMargin, currentHeight,
                getWidth() - rightMargin - leftMargin, componentHeight);
        currentHeight += componentHeight + heightMargin;

        auto childHeight = 20;
        for (auto child : component->getChildren()){
            child->setBounds(10, childHeight, component->getWidth() - 20, 20);
            childHeight += 30;
        }
    }


    auto sliderLeft = 120;

    //echoSlider.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
    //phaseButton.setBounds(sliderLeft, 40, getWidth() - sliderLeft - 10, 20);
}

void FlexPluginAudioProcessorEditor::buttonClicked(Button *button)
{
    if (button == &addEffectButton)
    {
        std::cout << valueTreeState.state.isValid() << newLine;
        std::cout << " add effect" << newLine;
        StringRef testRef = "test";
        Slider testSlider;
        std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> testAttachment;
        GroupComponent* testGroupComponent = new GroupComponent();
        testGroupComponent->setText("Test");
        addAndMakeVisible(testGroupComponent);
        testGroupComponent->addAndMakeVisible(testSlider);
        testAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, testRef, testSlider));
        valueTreeState.createAndAddParameter(std::make_unique<AudioParameterFloat>(testRef, testRef, 0.0f,1.0f,0.2f));
        effectGroupComponentList.add(testGroupComponent);

    }
}
