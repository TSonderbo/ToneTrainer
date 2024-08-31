#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : deviceSelector(deviceManager, 1, 256, 0, 256, false, false, false, false)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    deviceSelector.setSize(600, 400);
    settingsButton.setButtonText(juce::String("Settings"));
    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    setupDeviceSettings(&deviceSelector);
    addAndMakeVisible(settingsButton);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    /*auto* device = deviceManager.getCurrentAudioDevice();
    auto activeInputChannels = device->getActiveInputChannels();
    auto activeOutputChannels = device->getActiveOutputChannels();*/



    scopeDataCollector.process(bufferToFill.buffer->getReadPointer(0), (size_t)bufferToFill.numSamples);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    auto bounds = getLocalBounds();

    settingsButton.setBounds(10, bounds.getBottom() - 60, 100, 50);

}

void MainComponent::setupDeviceSettings(juce::AudioDeviceSelectorComponent* deviceSelectorPtr)
{
    auto wrapper = new Component();
    wrapper->setSize(600, 400);

    settingsButton.onClick = [this, deviceSelectorPtr, wrapper] {
        juce::DialogWindow::showDialog(
            "Audio Device Settings",
            deviceSelectorPtr,
            wrapper,
            getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId),
            true
        );
        };
}