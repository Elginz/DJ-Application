/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"

//==============================================================================

class DeckGUI    : public Component,
                   public Button::Listener,
                   public Slider::Listener,
                   public FileDragAndDropTarget,
                   public Timer
{
public:
    
    DeckGUI(DJAudioPlayer* player,
        PlaylistComponent* playlistComponent,
        AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& cacheToUse,
        int channelToUse);
    ~DeckGUI();

    
    void paint (Graphics&) override;
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (Button *) override;

    /** implement Slider::Listener */
    void sliderValueChanged (Slider *slider) override;

    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override;
    
    void timerCallback() override;
    
    void loadSong(const String& filePath)
    {
        URL fileURL = URL{ File{filePath} };
        player->loadURL(fileURL);
        waveformDisplay.loadURL(fileURL);
    }

    void loadTrack(const URL& trackURL);

private:
    //Declaring Buttons
    PlaylistComponent* playlistComponent;

    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
    TextButton loadButton{"LOAD"};
    TextButton loopButton{"LOOP"};
    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;
    Label posLabel;
    FileChooser fChooser{"Select a file..."};

    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player;
    
    //For playlist component
    int channel;
    Label titleLabel;
    //For Loop Button
    bool isLooping = false;
    double loopStart = 0.0;
    double loopEnd = 0.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
