// Fill out your copyright notice in the Description page of Project Settings.


#include "EIKVoiceChatSynthComponent.h"

bool UEIKVoiceChatSynthComponent::Init(int32& SampleRate)
{
	NumChannels = 1;

#if SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
	// Initialize the DSP objects
	Osc.Init(SampleRate);
	Osc.SetFrequency(440.0f);
	Osc.Start();
#endif // SYNTHCOMPONENT_EX_OSCILATOR_ENABLED

	return true;
}

int32 UEIKVoiceChatSynthComponent::OnGenerateAudio(float* OutAudio, int32 NumSamples)
{
	if (bIsReadyToStream)
	{
		for (int32 Sample = 0; Sample < NumSamples; ++Sample)
		{
			OutAudio[Sample] = Buffer[ReadIndex];
			ReadIndex = Buffer.GetNextIndex(ReadIndex);
		}
	}
	else
	{
		//VERY HACKY! This is NOT how we should actually be doing this. This is just a quick and dirty way to get the audio to play.
		if (WriteIndex > MinLatencySamples)
		{
			bIsReadyToStream = true;
		}

		return 0;

	}


	return NumSamples;
}

void UEIKVoiceChatSynthComponent::SetFrequency(const float InFrequencyHz)
{
#if SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
	// Use this protected base class method to push a lambda function which will safely execute in the audio render thread.
	SynthCommand([this, InFrequencyHz]()
	{
		Osc.SetFrequency(InFrequencyHz);
		Osc.Update();
	});
#endif // SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
}