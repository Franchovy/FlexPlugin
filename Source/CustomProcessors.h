/*
  ==============================================================================

    CustomPlugin.h
    Created: 14 Feb 2020 9:07:58am
    Author:  maxime

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CustomProcessor : public AudioProcessor
{
public:
    explicit CustomProcessor(String name, bool midiEffect);

    AudioProcessorEditor* createEditor() override {};
    bool hasEditor() const override {return false;}

    bool producesMidi() const override {return false;}
    bool isMidiEffect() const override {return midiEffect;}
    bool acceptsMidi() const override {return midiEffect;}

    virtual const String getName() const override {return effectName;}

    //TODO set/getStateInformation() override

protected:

private:
    String effectName;
    bool midiEffect; // MidiEffect -> input is MIDI, output is audio.

};

class EchoProcessor : public CustomProcessor
{
public:
    EchoProcessor(String name, bool midiEffect) : CustomProcessor(name, midiEffect) {

    }
    //==============================================================================

    void prepareToPlay (double sampleRate, int samplesPerBlock) override {

    }
    void releaseResources() override {

    }

    //TODO typedef these into one (in parent class) - example in overtonix
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override {

    }
    void processBlock (AudioBuffer<double>&, MidiBuffer&) override {

    }


    //==============================================================================

    double getTailLengthSeconds() const override {return 0.0;}

    int getNumPrograms() override {return 1;}
    int getCurrentProgram() override {return 0;}
    void setCurrentProgram (int index) override {}
    const String getProgramName (int index) override {return {};}
    void changeProgramName (int index, const String& newName) override {}
};
