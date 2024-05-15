/*
  ==============================================================================

    SoundLibrary.h
    Created: 16 Feb 2024 12:07:50am
    Author:  Elgin Foo

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include <map>

//==============================================================================
/*
*/

class SoundLibrary    : public Component,
                        public Button::Listener
{
public:
    SoundLibrary(DJAudioPlayer* player,
           AudioFormatManager &     formatManagerToUse,
           AudioThumbnailCache &     cacheToUse );
    ~SoundLibrary();
    
    
    void paint (Graphics&) override;
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (Button *) override;

private:
    DJAudioPlayer* player;
    
    AudioFormatManager& formatManager;
    AudioBuffer<float> bassDrumBuffer;
    
    AudioThumbnailCache& thumbCache;
    std::map<TextButton*, URL> buttonToSoundMap;
    File audioDirectory;

    
    //ADDED COLLECTION
    TextButton bassDrumButton{"Bass Drum"};
    TextButton bassOpenButton{"Bass Open"};
    TextButton floorDrumButton{"Floor Drum"};
    TextButton drumSticksButton{"Drum Sticks"};
    TextButton CymbalButton{"Cymbal"};
    TextButton guitarButton{"Guitar"};
    TextButton hiHatButton{"Hi Hat"};
    TextButton slapBassButton{"Slap Bass"};
    TextButton snareButton{"Snare"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundLibrary)
};
