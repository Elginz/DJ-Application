/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "SoundLibrary.h"
#include "Visualiser.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent,
                        private PlaylistComponent::Listener

{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
     
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100};
    
    int ToDeck1 = 0;
    int ToDeck2 = 1;
    
    PlaylistComponent playlistComponent{formatManager};

    DJAudioPlayer player1{formatManager};
    DeckGUI deckGUI1{&player1,&playlistComponent, formatManager, thumbCache, ToDeck1};

    DJAudioPlayer player2{formatManager};
    DeckGUI deckGUI2{&player2,&playlistComponent, formatManager, thumbCache, ToDeck2};

    DJAudioPlayer player3{formatManager};
    SoundLibrary soundLibrary{&player3, formatManager,thumbCache};
    
    Visualiser audioVisualiser;
    
    MixerAudioSource mixerSource;

    void trackSelectedForDeck(const URL& trackURL, int deck);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
