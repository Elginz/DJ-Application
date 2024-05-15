/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 15 Jan 2023 4:50:17pm
    Author:  Elgin Foo

  ==============================================================================
*/
#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================

PlaylistComponent::PlaylistComponent(AudioFormatManager& _formatManager)
    : formatManager(_formatManager)

{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    tableComponent.getHeader().addColumn("Track Title",1, 250);
    tableComponent.getHeader().addColumn("Duration", 2, 100);
    tableComponent.getHeader().addColumn("Add", 3, 100);
    tableComponent.getHeader().addColumn("Add", 4, 100);
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);
}

PlaylistComponent::~PlaylistComponent()
{
}

//Colour and texts
void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (Colours::darkslategrey);
    g.setColour (juce::Colours::grey);
    // draw an outline around the componen
    g.drawRect (getLocalBounds(), 1);
    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);
}

void PlaylistComponent::resized()
{
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows(){
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected){
    if (rowIsSelected) {
        g.fillAll(Colours::orange);
    }else{
        g.fillAll(Colours::grey);
    }
}

void PlaylistComponent::paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected){
    
    // Draw Track Title Name to first column
    if (columnId == 1)
    {
        g.drawText(selectedTitle[rowNumber],
            1, rowNumber,
            width - 4, height,
            Justification::centredLeft,
            true);
    }
    // Draw duration in seconds to second column
    if (columnId == 2)
    {
        g.drawText(std::to_string(selectedDuration[rowNumber]) + " seconds",
            1, rowNumber,
            width - 4, height,
            Justification::centred,
            true);
    }
}


Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate)
{
    if (columnId == 3)
    {
        TextButton* btn = dynamic_cast<TextButton*>(existingComponentToUpdate);
        if (!btn)
        {
            btn = new TextButton{ "To Deck 1" };
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
        //locating row number
        btn->setComponentID(String(rowNumber));
    }
    else if (columnId == 4)
    {
        TextButton* btn = dynamic_cast<TextButton*>(existingComponentToUpdate);

        if (!btn)
        {
            btn = new TextButton{ "To Deck 2" };
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
        //different row number for other deck channel
        btn->setComponentID(String(rowNumber + 1000));
    }

    return existingComponentToUpdate;
}


//==============================================================================
//AudioSource pure virtual functions
void PlaylistComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate){}
void PlaylistComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill){}
void PlaylistComponent::releaseResources(){}

void PlaylistComponent::buttonClicked(Button* button)
{
    int id = std::stoi(button->getComponentID().toStdString());
    if (id < 1000)
    {
        addToChannelList(selectedFiles[id], 0);
        juce::URL trackURL("file://" + selectedFiles[id]);
        listeners.call([trackURL](Listener& l) { l.trackSelectedForDeck(trackURL, 0); });
    }
    else
    {
        addToChannelList(selectedFiles[id - 1000], 1);
        juce::URL trackURL("file://" + selectedFiles[id - 1000]);
        listeners.call([trackURL](Listener& l) { l.trackSelectedForDeck(trackURL, 1); });
    }
    rowclicked = button->getComponentID().getDoubleValue();
}

//Files to be dragged and dropped
bool PlaylistComponent::isInterestedInFileDrag(const StringArray& files)
{
    return true;
}

void PlaylistComponent::filesDropped(const StringArray& files, int x, int y)
{
    //Perform if files have been dropped (mouse released with files)
    for (String filename : files)
    {
        //For each file URL, get filepath and file name
        std::string filepath = String(filename).toStdString();
        std::size_t startFilePos = filepath.find_last_of("\\");
        std::size_t startExtPos = filepath.find_last_of(".");
        std::string extn = filepath.substr(startExtPos + 1, filepath.length() - startExtPos);
        std::string file = filepath.substr(startFilePos + 1, filepath.length() - startFilePos - extn.size() - 2);

        //Update vectors for file details
        chooseFiles.push_back(filepath);
        trackTitles.push_back(file);

        //Determine audio length of the file
        getAudioLength(URL{ File{filepath} });

    }
    selectedTitle = trackTitles;
    selectedFiles = chooseFiles;

    //To include selected files
    tableComponent.updateContent();
}


//==============================================================================
// To determine length of audio
void PlaylistComponent::getAudioLength(URL audioURL)
{

    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));

    if (reader != nullptr)
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader,
            true));
        //Determine the length of audio
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
        double trackDuration = transportSource.getLengthInSeconds();
        trackDurations.push_back(trackDuration); // add audio length to vector
    }
    //Initialise durations of the audio.
    selectedDuration = trackDurations;
}

//==============================================================================
//Adding music to deck 1 and deck 2 respectively
void PlaylistComponent::addToChannelList(std::string filepath, int chooseDeck)
{
    //Deck 1
    if (chooseDeck == 0)
    {
        playList1.push_back(filepath);
    }
    //Deck 2
    if (chooseDeck == 1)
    {
        playList2.push_back(filepath);
    }
}
