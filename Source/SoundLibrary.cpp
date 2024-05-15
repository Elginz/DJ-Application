/*
  ==============================================================================

    SoundLibrary.cpp
    Created: 16 Feb 2024 12:07:50am
    Author:  Elgin Foo

  ==============================================================================
*/

#include "SoundLibrary.h"
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
SoundLibrary::SoundLibrary(DJAudioPlayer* _player,
                           AudioFormatManager& formatManagerToUse,
                           AudioThumbnailCache& cacheToUse)
                    : player(_player),
                    formatManager(formatManagerToUse),
                    thumbCache(cacheToUse)  // Initialize thumbCache

{
    
    addAndMakeVisible(bassDrumButton);
    addAndMakeVisible(bassOpenButton);
    addAndMakeVisible(floorDrumButton);
    addAndMakeVisible(drumSticksButton);
    addAndMakeVisible(CymbalButton);
    addAndMakeVisible(guitarButton);
    addAndMakeVisible(hiHatButton);
    addAndMakeVisible(slapBassButton);
    addAndMakeVisible(snareButton);

    bassDrumButton.addListener(this);
    bassOpenButton.addListener(this);
    floorDrumButton.addListener(this);
    drumSticksButton.addListener(this);
    CymbalButton.addListener(this);
    guitarButton.addListener(this);
    hiHatButton.addListener(this);
    slapBassButton.addListener(this);
    snareButton.addListener(this);

//FOR TESTING: AN ABSOLUTE FILE IN CASE SOUNDBOARD DOESNT WORKS
//    audioDirectory = File::getSpecialLocation(File::userHomeDirectory).getChildFile("/Users/elginfoo/Downloads/OtodecksFinals/SoundBoard");
        
    
    //Get files from     //Otodecks.app/Contents/Resources/

//    audioDirectory = File::getSpecialLocation(File::currentExecutableFile).getChildFile("../../Resources");
    
    audioDirectory = File::getSpecialLocation(File::currentExecutableFile).getChildFile("../../../../../../../../Source/SoundBoard");

    
    DBG("Audio directory path: " + audioDirectory.getFullPathName());
    
    buttonToSoundMap[&bassDrumButton] = URL{audioDirectory.getChildFile("bassDrum.mp3")};
    buttonToSoundMap[&bassOpenButton] = URL{audioDirectory.getChildFile("bassOpen.mp3")};
    buttonToSoundMap[&floorDrumButton] = URL{audioDirectory.getChildFile("floorDrum.mp3")};
    buttonToSoundMap[& drumSticksButton] = URL{audioDirectory.getChildFile("drumSticks.mp3")};
    buttonToSoundMap[&CymbalButton] = URL{audioDirectory.getChildFile("cymbal.mp3")};
    buttonToSoundMap[&guitarButton] = URL{audioDirectory.getChildFile("guitar.mp3")};
    buttonToSoundMap[&hiHatButton] = URL{audioDirectory.getChildFile("hiHat.mp3")};
    buttonToSoundMap[&slapBassButton] = URL{audioDirectory.getChildFile("slapBass.mp3")};
    buttonToSoundMap[&snareButton] = URL{audioDirectory.getChildFile("snare.mp3")};

}

SoundLibrary::~SoundLibrary()
{
}

void SoundLibrary::paint (Graphics& g)
{

    g.fillAll (Colours::darkslateblue);   // clear the background

    g.setColour (Colours::grey);

    g.drawRect (getLocalBounds(), 2);   // draw an outline around the component

    g.setColour (Colours::white);
}


void SoundLibrary::resized()
{
    double rowH = getHeight() /3;
    double colW = getWidth() /3;
    
    
    snareButton.setBounds(5,10, colW - 10 , rowH -20);
    CymbalButton.setBounds(colW + 5,10, colW - 10, rowH -20);
    hiHatButton.setBounds(colW * 2 +5, 10, colW - 10, rowH -20);
    
    bassDrumButton.setBounds(5, rowH + 10, colW - 10, rowH -20);
    bassOpenButton.setBounds(colW + 5, rowH + 10, colW - 10, rowH -20);
    floorDrumButton.setBounds(colW * 2 +5, rowH + 10, colW - 10, rowH -20);
    
    drumSticksButton.setBounds(5, rowH * 2 + 10, colW - 10, rowH -20);
    guitarButton.setBounds(colW + 5, rowH * 2 +10, colW - 10, rowH -20);
    slapBassButton.setBounds(colW *2 +5, rowH * 2 +10, colW - 10, rowH -20);
}

void SoundLibrary::buttonClicked(Button* button)
{
    auto it = buttonToSoundMap.find(static_cast<TextButton*>(button));

    if (it != buttonToSoundMap.end())
    {
        URL soundURL = it->second;

        //EXPERIMENT
        // Check if the file exists
        if (!soundURL.isLocalFile())
        {
            DBG("URL is not a local file");
            return;
        }

        File audioFile(soundURL.getLocalFile());
        if (!audioFile.exists())
        {
            DBG("File does not exist: " + audioFile.getFullPathName());
            return;
        }

        DBG("Checking file existence: " + audioFile.getFullPathName());

        //  load and play the sound associated with the button
        player->loadURL(soundURL);
        player->start();

    }
    else
    {
        std::cerr << "Error: Button not found in the map!" << std::endl;
    }
        
}
