#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "DSP/Allpass.h"

#if (MSVC)
#include "ipps.h"
#endif

#if USING_RUST
#include "pawrprustlib/lib.h"
#endif

class PluginProcessor : public juce::AudioProcessor

  , public juce::AudioProcessorValueTreeState::Listener

{
public:
    PluginProcessor();
    ~PluginProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

  
  // Parameters
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        juce::AudioProcessorValueTreeState::ParameterLayout layout;

        layout.add (std::make_unique<juce::AudioParameterInt> ("param1", "Default Param 1", 0, 12, 0));

        layout.add (std::make_unique<juce::AudioParameterFloat> ("gain", "Gain", 0.0f, 1.1f, 0.5f));

        layout.add (std::make_unique<juce::AudioParameterChoice> ("param3", "Default Param 3", juce::StringArray { "Option 1", "Option 2", "Option 3" }, 0));

        layout.add (std::make_unique<juce::AudioParameterFloat> ("allpass_gain", "Allpass Gain", -1.0f, 1.0f, 0.5f));

        layout.add (std::make_unique<juce::AudioParameterFloat> ("allpass_frequency", "Allpass Frequency", -22000.0f, 22000.0f, 10.0f));



        return layout;
    }

    // Parameter stuff
    std::atomic<bool> parametersNeedUpdating;
    juce::AudioProcessorValueTreeState parameters { *this, nullptr, "Parameters", createParameterLayout() };
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    void updateParameters();

    AllpassFilter allpass;

    juce::AudioProcessorValueTreeState& getValueTreeState() { return parameters; }


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
