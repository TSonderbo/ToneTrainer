/*
  ==============================================================================

    PitchDetector.h
    Created: 2 Jan 2024 9:47:29am
    Author:  Sonderbo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "Yin.h"

class PitchDetector
{
public:

    void prepareToPlay(double sampleRate, double minFreq, double maxFreq, double bufferSize);
    void estimatePitch();
    juce::String estimateNote();

    double getPitch();

private:


    juce::AudioBuffer<float> buffer;
    Yin yin;
};