/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================

DeckGUI::DeckGUI(DJAudioPlayer* _player,
                PlaylistComponent* _playlistComponent,
                AudioFormatManager& formatManagerToUse,
                AudioThumbnailCache& cacheToUse,
                int channelToUse
                ) : player(_player),
                    playlistComponent(_playlistComponent),
                    waveformDisplay(formatManagerToUse,cacheToUse)


{

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    loopButton.addListener(this);
    
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.1, 5.0);
    posSlider.setRange(0.0, 1.0);

    // Set default values for the sliders
    volSlider.setValue(0.5);
    speedSlider.setValue(1.0);
    
    speedSlider.setMouseDragSensitivity(80);
    addAndMakeVisible(titleLabel);
    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    //Background
    g.fillAll (Colours::darkslategrey);
    g.setColour (Colours::black);
    g.drawRect (getLocalBounds(), 2);
    g.setColour (Colours::white);
    g.setFont (14.0f);
    
    
    //Volume and texts
    double rowH = getHeight() / 9;
    double width = getWidth()/10;
    
    int textHeight = 20;
    g.drawText ("Volume", width * 0.5, rowH * 2.7, getWidth(), textHeight, false);
    g.drawText ("Speed", width * 2.1, rowH* 2.7, getWidth(), textHeight, false);
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 9;
    double width = getWidth()/10;
    waveformDisplay.setBounds(0, rowH , getWidth(), rowH * 1.5);
    
    volSlider.setBounds(width * 0.5, rowH * 3, width, rowH * 4.5 );
    speedSlider.setBounds(width * 2 , rowH * 3, width, rowH * 4.5);
    posSlider.setBounds(width * 5, rowH * 3, width * 3, rowH * 4.5);
    
    volSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    speedSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    posSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    
    volSlider.setTextBoxStyle(Slider::TextBoxBelow, false, width *2, 20);
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow, false, width * 2, 20);
    posSlider.setTextBoxStyle(Slider::NoTextBox, false, width * 2, 20);
    posSlider.setMouseDragSensitivity(80);
    
    addAndMakeVisible(posLabel);
    posLabel.setText("Position Slider", juce::dontSendNotification);
    posLabel.attachToComponent(&posSlider, false);
    posLabel.setJustificationType(juce::Justification::centred);
    posLabel.setBounds(width * 5.5 - 10, rowH, width * 2.5, rowH * 4.5);

    playButton.setBounds(0 + 5, rowH * 8, getWidth()/4 - 5, rowH);
    stopButton.setBounds(getWidth()/4 + 5, rowH * 8, getWidth()/4 -5, rowH);
    loadButton.setBounds(getWidth()/4 * 2 + 5, rowH * 8, getWidth()/4 -5, rowH);
    loopButton.setBounds(getWidth()/4 * 3 + 5, rowH * 8, getWidth()/4 -5, rowH);
    titleLabel.setBounds(5, 5, getWidth() - 10, 20);
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
     if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();

    }
       if (button == &loadButton)
    {
        auto fileChooserFlags =
        FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            player->loadURL(URL{chooser.getResult()});
            
            String songTitle = chooser.getResult().getFileNameWithoutExtension();
            std::cout << "Loaded song: " << songTitle << std::endl;
            titleLabel.setText(File{chooser.getResult()}.getFileName(), dontSendNotification);
            
            waveformDisplay.loadURL(URL{chooser.getResult()});
        });
    }
    
    if (button == &loopButton)
    {
        isLooping = !isLooping; // Toggle loop state

        // Update loop button appearance based on loop state
        if (isLooping){
            loopButton.setButtonText("UNLOOP");
        }
        else
            loopButton.setButtonText("LOOP");
        }

    }



void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true;
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());
    
    // Check if the loop is active and adjust the playback position
    if (isLooping)
    {
        bool songHasEnded = false;
        // If the song is at 99% complete or more, then reset position and restart song.
        if(player->getPositionRelative() > 0.99)
        {
            { songHasEnded = true; }
        }
        if(songHasEnded)
        {
            player->setPositionRelative(0);
            player->start();
        }
    }
    
}



void DeckGUI::loadTrack(const URL& trackURL)
{
    // The trackURL is assumed to be a proper file URL here.
    if (trackURL.isLocalFile())
    {
        juce::File audioFile = trackURL.getLocalFile();
        player->loadURL(trackURL);

        String songTitle = audioFile.getFileNameWithoutExtension();
        std::cout << "Loaded song: " << songTitle << std::endl;
        titleLabel.setText(songTitle, dontSendNotification);
        
        waveformDisplay.loadURL(trackURL);
    }
    else
    {
        // Handle the error for non-local files or unsupported URLs.
        std::cerr << "Error: Unsupported URL passed to DeckGUI::loadTrack." << std::endl;
    }

}

//Sound effects from https://www.fesliyanstudios.com/royalty-free-sound-effects-download/musical-172

