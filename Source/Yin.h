/*
  ==============================================================================

    Yin.h
    Created: 3 May 2023 10:41:23am
    Author:  Sonderbo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class Yin
{
public:
    void prepareToPlay(double sampleRate, double minFreq, double maxFreq);
    float estimatePitch(juce::AudioBuffer<float>& audioBuffer, float threshold);
private:

    bool isPrepared = false;
    std::vector<float> buffer;
    std::vector<float> yinBuffer;

    int tau_min;
    int tau_max;
    int windowSize;
    double sampleRate;

    void DF();
    float CMNDF(float threshold);
};