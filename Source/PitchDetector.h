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
#include "AudioBufferQueue.h"
#include "NoteFrequencies.h"

struct Estimate
{
    float pitch;
    juce::String note;
    float confidence;
};

class PitchDetector
{
public:
    PitchDetector(AudioBufferQueue& queueToUse);
    void prepareToPlay(double sampleRate, double minFreq, double maxFreq);
    Estimate estimatePitch();

private:
    std::vector<float> buffer;
    double sampleRate;
    AudioBufferQueue& audioBufferQueue;
    Yin yin;
    std::vector<float> sampleData;

    juce::String calculateNote(float freq);
    bool isNote(float input, float target);
};