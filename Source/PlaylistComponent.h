/*
  ==============================================================================

    PlaylistComponent.h
    Created: 15 Jan 2023 4:50:17pm
    Author:  Elgin Foo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>


//==============================================================================
class PlaylistComponent  : public juce::Component,
                            public TableListBoxModel,
                            public Button::Listener,
                            public FileDragAndDropTarget
{
public:
    PlaylistComponent(AudioFormatManager& formatManager);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    int getNumRows() override;
    void paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;
    void buttonClicked(Button * button) override;
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

        
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;
    
//Loading of playlists
    //songs that will be added to the deckGUI1
    std::vector<std::string> playList1;
    //songs that wil be added to deckGUI2
    std::vector<std::string> playList2;

//listener to register classes as listeners inside the component
    struct Listener{
        virtual ~ Listener() = default;
        virtual void trackSelectedForDeck(const juce::URL& trackURL, int deck) = 0;
    };
    void addListener(Listener* newListener) { listeners.add(newListener); }
    void removeListener(Listener* listener) { listeners.remove(listener); }

//==============================================================================

private:
    
    TableListBox tableComponent;

    std::vector<std::string> chooseFiles;
    std::vector<std::string> selectedFiles;
    std::vector<std::string> trackTitles;
    std::vector<std::string> selectedTitle;
    std::vector<int> trackDurations;
    std::vector<int> selectedDuration;
    
    void getAudioLength(URL audioURL);
    void addToChannelList(std::string filepath, int chooseDeck);

    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    
    Array<juce::File> myFiles;
    double rowclicked;
    juce::ListenerList<Listener> listeners;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
