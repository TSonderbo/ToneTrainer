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
	this->sampleRate = sampleRate;
	tau_min = static_cast<int>(sampleRate / maxFreq);
	tau_max = static_cast<int>(sampleRate / minFreq);
	isPrepared = true;
}

/*
	returns pitch estimate, confidence value
*/
std::tuple<float, float> Yin::estimatePitch(std::vector<float>& audioBuffer, float threshold)
{
	if (isPrepared == false)
	{
		return std::make_tuple(0,0);
	}

	buffer = audioBuffer;

	windowSize = static_cast<int>(buffer.size() / 2);

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

/*
	returns pitch estimate, confidence value
*/
std::tuple<float, float> Yin::CMNDF(float threshold)
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
			return std::make_tuple(sampleRate / tau, yinBuffer[tau - tau_min]);
		}
		if (yinBuffer[tau - tau_min] < min)
		{
			min = yinBuffer[tau - tau_min];
			minArg = tau;
		}
	}

	return std::make_tuple(sampleRate / minArg, min);
}
