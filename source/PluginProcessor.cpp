#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginProcessor::PluginProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ) 
{


  parameters.addParameterListener("param1", this);
  parameters.addParameterListener("gain", this);
  parameters.addParameterListener("param3", this);
  parameters.addParameterListener("allpass_gain", this);
  parameters.addParameterListener("allpass_frequency", this);


#if USING_RUST
  auto result_from_rust = pawrprustlib::my_rust_function(3, 9);
  juce::Logger::writeToLog("Rust result: " + juce::String(result_from_rust));
#endif

}

PluginProcessor::~PluginProcessor()
{


  parameters.removeParameterListener("param1", this);
  parameters.removeParameterListener("gain", this);
  parameters.removeParameterListener("param3", this);
  parameters.removeParameterListener("allpass_gain", this);
  parameters.removeParameterListener("allpass_delay", this);

}

//==============================================================================
const juce::String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String PluginProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void PluginProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    allpass = std::make_unique<AllpassFilter>(1, int(sampleRate), getNumOutputChannels());
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void PluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    if (parametersNeedUpdating)
{
        updateParameters();
}

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        auto numSamples = buffer.getNumSamples();
         for (int n = 0; n < numSamples; n++)
        {
            auto x = buffer.getSample(channel, n);
            auto y = allpass->process(x, channel);
            y *= volume;
            buffer.setSample(channel, n, y);
        }
    }
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{// Use generic gui for editor for now
    // return new PluginEditor (*this);
    return new juce::GenericAudioProcessorEditor (*this);


}

//==============================================================================
void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}


//==============================================================================
// Define parameters for the plugin

void PluginProcessor::parameterChanged (const juce::String& parameterID, float newValue)
{
    // Print out change
    juce::String message = "Parameter changed: " + parameterID + " = " + juce::String (newValue);
    juce::Logger::writeToLog (message);

    parametersNeedUpdating = true;
}

void PluginProcessor::updateParameters()
{
    // Get parameters
    auto defaultParam1 = parameters.getParameter ("param1");
    auto gainParam = parameters.getParameter ("gain");
    auto defaultParam3 = parameters.getParameter ("param3");
    auto allpassGainParam = parameters.getParameter ("allpass_gain");
    auto allpassDelayParam = parameters.getParameter ("allpass_delay");

    // Get values
    auto defaultVal1 = static_cast<juce::AudioParameterInt*> (defaultParam1)->get();
    auto gainVal = static_cast<juce::AudioParameterFloat*> (gainParam)->get();
    auto defaultVal3 = static_cast<juce::AudioParameterChoice*> (defaultParam3)->getCurrentChoiceName();
    auto allpassGainVal = static_cast<juce::AudioParameterFloat*> (allpassGainParam)->get();
    auto allpassDelayVal = static_cast<juce::AudioParameterInt*> (allpassDelayParam)->get();

    allpass->setGain(allpassGainVal);
    allpass->setDelayLength(allpassDelayVal);
    volume = gainVal;

    // Do something with the parameter values
    juce::ignoreUnused(defaultVal1, gainVal, defaultVal3, allpassDelayVal, allpassGainVal);

    // Reset flag
    parametersNeedUpdating = false;
}


