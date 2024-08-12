/*
  ==============================================================================

	Yin.cpp
	Created: 3 May 2023 10:41:23am
	Author:  Sonderbo

  ==============================================================================
*/

#include "Yin.h"

void Yin::prepareToPlay(double sampleRate, double minFreq, double maxFreq)
{
	tau_min = static_cast<int>(sampleRate / maxFreq);
	tau_max = static_cast<int>(sampleRate / minFreq);

	this->sampleRate = sampleRate;
	isPrepared = true;
}

float Yin::estimatePitch(juce::AudioBuffer<float>& audioBuffer, float threshold)
{
	if (isPrepared == false)
	{
		return 0;
	}

	const float* start = audioBuffer.getReadPointer(0);
	int size = audioBuffer.getNumSamples();
	buffer = std::vector<float>(start, start + size);

	windowSize = static_cast<int>(audioBuffer.getNumSamples() / 2);

	if (tau_max > windowSize)
	{
		tau_max = windowSize;
	}

	size_t yinsize = tau_max - tau_min;

	yinBuffer = std::vector<float>(yinsize, 0);

	return CMNDF(threshold);
}

void Yin::DF()
{
	float difference = 0.0f;

	for (int tau = tau_min; tau < tau_max; tau++)
	{
		for (int i = 0; i < windowSize; i++)
		{
			difference = buffer[i] - buffer[i + tau];
			yinBuffer[tau-tau_min] += difference * difference;
		}
	}
}

float Yin::CMNDF(float threshold)
{
	DF();

	float runningSum = 0.0f;
	float min = std::numeric_limits<float>::max();
	int minArg = 1;

	for (int tau = tau_min; tau < tau_max; tau++)
	{
		runningSum += yinBuffer[tau - tau_min];
		yinBuffer[tau - tau_min] *= tau / runningSum;

		if (yinBuffer[tau - tau_min] < threshold)
		{
			return sampleRate / tau;
		}
		if (yinBuffer[tau - tau_min] < min)
		{
			min = yinBuffer[tau - tau_min];
			minArg = tau;
		}
	}

	return sampleRate / minArg;
}
