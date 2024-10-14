// Fill out your copyright notice in the Description page of Project Settings.


#include "EIKVoiceChatSynthComponent.h"

bool UEIKVoiceChatSynthComponent::Init(int32& SampleRate)
{
	NumChannels = 1;
#if ENGINE_MAJOR_VERSION >= 5
	AudioBuffer = Audio::TCircularAudioBuffer<float>(SampleRate * NumChannels);
#else
	// Reset the buffer with the new size
	AudioBuffer.Reset(SampleRate * NumChannels);

	OutArray.Reserve(SampleRate / 10);
	OutArrayView = TArrayView<float>(OutArray.GetData(), SampleRate / 10);
#endif
	OutArray.Reserve(SampleRate / 10);
	OutArrayView = TArrayView<float>(OutArray.GetData(), SampleRate / 10);

	return true;
}

int32 UEIKVoiceChatSynthComponent::OnGenerateAudio(float* OutAudio, int32 NumSamples)
{
	//we perform some basic checks to ensure we have enough samples to stream, if not we return 0
	if (AudioBuffer.Num() >= uint32(NumSamples))
	{
		AudioBuffer.Pop(OutAudio, NumSamples);
		return NumSamples;
		
	}
	else
	{
		return 0;
	}
}

