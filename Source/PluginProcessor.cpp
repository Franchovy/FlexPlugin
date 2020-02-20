/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FlexPluginAudioProcessor::FlexPluginAudioProcessor() :
        parameters(*this, nullptr, Identifier("parameterTree"), createParameterLayout()
                /*{


        }*/)
#ifndef JucePlugin_PreferredChannelConfigurations
     , AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    echoBuffer = AudioBuffer<float>();
    gainParameter = parameters.getRawParameterValue("gain");
    phaseParameter = parameters.getRawParameterValue("invertPhase");
    echoParameter = parameters.getRawParameterValue("echo");


}

FlexPluginAudioProcessor::~FlexPluginAudioProcessor()
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
}

//==============================================================================
const String FlexPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FlexPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FlexPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FlexPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FlexPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FlexPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FlexPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FlexPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String FlexPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void FlexPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void FlexPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    auto phase = *phaseParameter < 0.5 ? 1.0f : -1.0f;
    previousGain = *gainParameter * phase;

    float echoLengthInSeconds = 0.5;
    echoBufferSize = echoLengthInSeconds * sampleRate;
    echoBufferPt = 0;

    echoBuffer = AudioBuffer<float>(getTotalNumOutputChannels(), echoBufferSize);
    echoBuffer.clear();

    /* // Sine wave test
    for (int i = 0; i < echoBufferSize; i++) {
        for (int c = 0; c < getTotalNumOutputChannels(); c++) {
            echoBuffer.setSample(c,i, std::sin(i/0.01f));
        }
    } */
}

void FlexPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FlexPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void FlexPluginAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    auto phase = *phaseParameter < 0.5f ? 1.0f : -1.0f;
    auto currentGain = *gainParameter * phase;
/*
    // Optional?
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
*/
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto *channelData = buffer.getWritePointer(channel);
        bool resetState = false;
        if (channel == totalNumInputChannels - 1) {
            resetState = true;
        }

        if (echoBufferPt + numSamples > echoBufferSize) {
            auto firstHalfSize = echoBufferSize - echoBufferPt;
            auto secondHalfSize = numSamples - firstHalfSize;
            // Split add to output
            buffer.addFrom(channel, 0, echoBuffer, channel, echoBufferPt, firstHalfSize);
            buffer.addFrom(channel, firstHalfSize, echoBuffer, channel, 0, secondHalfSize);
            // Copy over to echoBuffer
            echoBuffer.copyFrom(channel, echoBufferPt, buffer, channel, 0, firstHalfSize);
            echoBuffer.copyFrom(channel, 0, buffer, channel, firstHalfSize, secondHalfSize);
            // Apply gain to echo buffer
            echoBuffer.applyGain(echoBufferPt, firstHalfSize, *echoParameter);
            echoBuffer.applyGain(channel, 0, secondHalfSize, *echoParameter);
            // Set new echoBufferPt if this is the last channel
            if (resetState)
                echoBufferPt = numSamples - firstHalfSize;
        } else {
            // Add to output
            buffer.addFrom(channel, 0, echoBuffer, channel, echoBufferPt, numSamples);
            // Copy to echo buffer
            echoBuffer.copyFrom(channel, echoBufferPt, buffer, channel, 0, numSamples);
            // Apply gain to echo buffer
            echoBuffer.applyGain(channel, echoBufferPt, numSamples, *echoParameter);
            // Set new echoBufferPt if this is the last channel
            if (resetState)
                echoBufferPt += numSamples;
        }
    }

    //echoBuffer.applyGain(*echoParameter);

    if (currentGain == previousGain)
    {
        buffer.applyGain(currentGain);
    } else {
        buffer.applyGainRamp(0, buffer.getNumSamples(), previousGain, currentGain);
        previousGain = currentGain;
    }

}

//==============================================================================
bool FlexPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* FlexPluginAudioProcessor::createEditor()
{
    return new FlexPluginAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void FlexPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary(*xml, destData);
}

void FlexPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr){
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(ValueTree::fromXml(*xmlState));
    }
}

AudioProcessorValueTreeState::ParameterLayout FlexPluginAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<AudioParameterFloat>> params;



    params.push_back(std::make_unique<AudioParameterFloat>("gain", "Gain", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<AudioParameterFloat>("invertPhase", "InvertPhase", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<AudioParameterFloat>("echo", "Echo", 0.0f, 1.0f, 0.2f));
    for (int i = 4; i < 9; ++i)
        params.push_back(std::make_unique<AudioParameterFloat> (String (i), String(i), 0, 1.0f, 0.5f));

    return {params.begin(), params.end()};

}

void FlexPluginAudioProcessor::addParameter(StringRef name) {
    parameters.createAndAddParameter(std::make_unique<AudioParameterFloat>(name, name, 0.0f,1.0f,0.2f));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FlexPluginAudioProcessor();
}
