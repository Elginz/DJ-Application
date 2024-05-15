///*
//  ==============================================================================
//
//    Visualiser.h
//    Created: 25 Feb 2024 3:34:38am
//    Author:  Elgin Foo
//
//  ==============================================================================
//
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================



class Visualiser    :   public ChangeListener,
                        public juce::AudioVisualiserComponent
{
public:
    Visualiser() : juce::AudioVisualiserComponent(2)
    {
        setBufferSize(512);
        setSamplesPerBlock(256);
        setColours(juce::Colours::black, juce::Colours::darkgreen);
    }

    ~Visualiser() override = default;

    void pushAudioData(const juce::AudioBuffer<float>& buffer)
    {
        pushBuffer(buffer);
    }

    void changeListenerCallback(ChangeBroadcaster* source) override
    {
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Visualiser)
};
