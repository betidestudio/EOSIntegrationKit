// Fill out your copyright notice in the Description page of Project Settings.


#include "EIKVoiceChatSynthComponent.h"

bool UEIKVoiceChatSynthComponent::Init(int32& SampleRate)
{
	NumChannels = 1;
	return true;
}

int32 UEIKVoiceChatSynthComponent::OnGenerateAudio(float* OutAudio, int32 NumSamples)
{
	//we perform some basic checks to ensure we have enough samples to stream, if not we return 0
	if (bIsReadyToStream)
	{
		if (AvailableSamples < NumSamples)
		{
			bIsReadyToStream = false;
			return 0;
		}

		for (int32 Sample = 0; Sample < NumSamples; ++Sample)
		{
			OutAudio[Sample] = Buffer[ReadIndex];
			ReadIndex = Buffer.GetNextIndex(ReadIndex);
		}

		AvailableSamples -= NumSamples;

	}
	else
	{

		if (AvailableSamples > MinLatencySamples)
		{
			bIsReadyToStream = true;
		}

		return 0;

	}

	return NumSamples;
}

