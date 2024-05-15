/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
: soundLibrary(&player3, formatManager, thumbCache)

{
    setSize (1000, 500);

    // Some platforms require permissions to open input channels so requesting that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }  

    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);
    
    //Soundboard
    addAndMakeVisible(soundLibrary);
    formatManager.registerBasicFormats();
    
    //Playlist
    playlistComponent.addListener(this);
    
    //visualiser
    addAndMakeVisible(audioVisualiser);
    
    // Connect the mixer to the audio transport sources
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    //For Visualiser to have it's own mixer
    mixerSource.addInputSource(&player3, false);

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay  (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

 }
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
    audioVisualiser.pushAudioData(*bufferToFill.buffer);

}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    playlistComponent.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour(Colours::white);
    g.setFont(14.0f);
}

void MainComponent::resized()
{
    deckGUI1.setBounds(0, 0, getWidth()/7 * 3, getHeight() /5 * 3);
    deckGUI2.setBounds(getWidth()/7 * 4 + 5, 0, getWidth()/7 * 3, getHeight() / 5 * 3);
    playlistComponent.setBounds(0, getHeight()/5 * 3, getWidth()/3 * 2, getHeight()/5 * 2);
    soundLibrary.setBounds(getWidth()/3* 2, getHeight()/5 * 3, getWidth()/3, getHeight()/5 * 2);
    audioVisualiser.setBounds(getWidth()/7 * 3, 0, getWidth()/7 + 5, getHeight() /5 * 3);

}

void MainComponent::trackSelectedForDeck(const URL &trackURL, int deck)
{
    if(deck == 0)
        deckGUI1.loadTrack(trackURL);
    else if (deck == 1)
        deckGUI2.loadTrack(trackURL);
}
