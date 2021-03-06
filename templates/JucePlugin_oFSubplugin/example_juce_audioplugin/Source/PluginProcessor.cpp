/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <chrono>
#include <thread>

//==============================================================================
Juceglvst_audioProcessor::Juceglvst_audioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{

#if defined (_WIN32)
	HotJuicePluginProcessor::setup(
		{ "MyPlugin" }, "example_oF_subplugin",
		juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getChildFile("MyCompany/Hotjuice oF and JUCE example").getFullPathName().toStdString(),
		{ "FreeImage.dll", "fmodex64.dll","fmodexL64.dll" }
	);
#elif defined (__APPLE__)
	HotJuicePluginProcessor::setup(
		{ "MyPlugin" }, "example_oF_subplugin",
		juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getChildFile("Application Support").getChildFile("MyCompany/com.company.applicatione").getFullPathName().toStdString(),
		{ "libfmodex.dylib" }
	);
#endif

	startTimer(50);
}

void Juceglvst_audioProcessor::timerCallback()
{
	pluginManager->tryToLoadIfUpdated();

	if (!isReloading && plugins[0]) {
		float in[2] = { 0, 0 };
		float out = 0;

		plugins[0]->update(&in, &out);
	}
}

Juceglvst_audioProcessor::~Juceglvst_audioProcessor()
{
}


//==============================================================================
const String Juceglvst_audioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Juceglvst_audioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Juceglvst_audioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Juceglvst_audioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Juceglvst_audioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Juceglvst_audioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Juceglvst_audioProcessor::getCurrentProgram()
{
    return 0;
}

void Juceglvst_audioProcessor::setCurrentProgram (int index)
{
}

const String Juceglvst_audioProcessor::getProgramName (int index)
{
    return {};
}

void Juceglvst_audioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Juceglvst_audioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Juceglvst_audioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Juceglvst_audioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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


void Juceglvst_audioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

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
	
	HotJuicePluginProcessor::processBlock(buffer, midiMessages, totalNumInputChannels, totalNumOutputChannels, getSampleRate());

	/*
	for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
    }
	*/
}

//==============================================================================
bool Juceglvst_audioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Juceglvst_audioProcessor::createEditor()
{
    return new Juceglvst_audioProcessorEditor (*this);
}

//==============================================================================
void Juceglvst_audioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Juceglvst_audioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Juceglvst_audioProcessor();
}
