// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SynthComponent.h"
#include "DSP/Osc.h"
#include "Containers/CircularBuffer.h"
#include "EIKVoiceChatSynthComponent.generated.h"

/**
 * EIK Voice Chat synth Component is used by the EOSVoiceChat System, when attached to a player pawn it will stream audio from the EOSVoiceChatUser allowing the use of spatialization and other audio features
 */

UCLASS(ClassGroup = Synth, meta = (BlueprintSpawnableComponent))
class EIKVOICECHAT_API UEIKVoiceChatSynthComponent : public USynthComponent
{
	GENERATED_BODY()

	//the minimum amount of samples we accumulate before we start streaming, to ensure there is no starvation on the audio thread, if there's too much latency this can be reduced
	constexpr static int32 MinLatencySamples = 2048;
	bool bIsReadyToStream = false;

	//The circular buffer and its read/write indices, 44100 is really largely an arbitrary number here, a rather larger buffer
	TCircularBuffer<float> Buffer = TCircularBuffer<float>(44100);
	uint32 WriteIndex = 0;
	uint32 ReadIndex = 0;


	// Called when synth is created
	virtual bool Init(int32& SampleRate) override;

	// Called to generate more audio
	virtual int32 OnGenerateAudio(float* OutAudio, int32 NumSamples) override;


protected:

	friend class FEOSVoiceChatUser;

	// called by EOSVoiceChatUser to fill the buffer, as long as these are samples available these should be consumed via OnGenerateAudio
	void WriteSamples(TArrayView<int16> Samples)
	{
		for (int16 Sample : Samples)
		{
			Buffer[WriteIndex] = Sample / 32768.0f;
			WriteIndex = Buffer.GetNextIndex(WriteIndex);
		}

		AvailableSamples += Samples.Num();
	}

	int32 AvailableSamples = 0;

};