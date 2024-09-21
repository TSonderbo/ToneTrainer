/*
  ==============================================================================

    PitchDetector.cpp
    Created: 2 Jan 2024 9:47:29am
    Author:  Sonderbo

  ==============================================================================
*/

#include "PitchDetector.h"

PitchDetector::PitchDetector(AudioBufferQueue& queueToUse) : audioBufferQueue(queueToUse)
{

}


void PitchDetector::prepareToPlay(double sampleRate, double minFreq, double maxFreq) 
{
    this->sampleRate = sampleRate;
    yin.prepareToPlay(sampleRate, minFreq, maxFreq);
    buffer.resize(audioBufferQueue.bufferSize);
}

Estimate PitchDetector::estimatePitch()
{
    audioBufferQueue.pop(buffer.data());

    std::tuple<float, float> yin_estimate = yin.estimatePitch(buffer, 0.1f);

    juce::String note_estimate = calculateNote(std::get<0>(yin_estimate));

    Estimate estimate;
    estimate.pitch = std::get<0>(yin_estimate);
    estimate.confidence = std::get<1>(yin_estimate);
    estimate.note = note_estimate;

    return estimate;
}

juce::String PitchDetector::calculateNote(float freq)
{
    using namespace notes;
    
    for (int i = 0; i < numOctaves; i++)
    {
        if (isNote(freq, A[i]))
            return juce::String("A");
        if (isNote(freq, A_sharp[i]))
            return juce::String("A#");
        if (isNote(freq, B[i]))
            return juce::String("B");
        if (isNote(freq, C[i]))
            return juce::String("C");
        if (isNote(freq, C_sharp[i]))
            return juce::String("C#");
        if (isNote(freq, D[i]))
            return juce::String("D");
        if (isNote(freq, D_sharp[i]))
            return juce::String("D");
        if (isNote(freq, E[i]))
            return juce::String("E");
        if (isNote(freq, F[i]))
            return juce::String("F");
        if (isNote(freq, F_sharp[i]))
            return juce::String("F#");
        if (isNote(freq, G[i]))
            return juce::String("G");
        if (isNote(freq, G_sharp[i]))
            return juce::String("G#");
    }
    //In case no note is registered
    return juce::String("-");
}

bool PitchDetector::isNote(float input, float target)
{
    const float cents_tolerance = 10.0f;

    float min = target * powf(2, -cents_tolerance * 2 / 1200.0f);
    float max = target * powf(2, cents_tolerance * 2 / 1200.0f);

    return min < input && input < max;
}
