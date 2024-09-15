// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SynthComponent.h"
#include "DSP/Osc.h"
#include "Containers/CircularBuffer.h"
#include "EIKVoiceChatSynthComponent.generated.h"

// ========================================================================
// UEIKVoiceChatSynthComponent
// Synth component class which implements USynthComponent
// This is a simple hello-world type example which generates a sine-wave
// tone using a DSP oscillator class and implements a single function to set
// the frequency. To enable example:
// 1. Ensure "SignalProcessing" is added to project's .Build.cs in PrivateDependencyModuleNames
// 2. Enable macro below that includes code utilizing SignalProcessing Oscilator
// ========================================================================

#define SYNTHCOMPONENT_EX_OSCILATOR_ENABLED 1

UCLASS(ClassGroup = Synth, meta = (BlueprintSpawnableComponent))
class EIKVOICECHAT_API UEIKVoiceChatSynthComponent : public USynthComponent
{
	GENERATED_BODY()

	constexpr static int32 MinLatencySamples = 2048;
	bool bIsReadyToStream = false;

	TCircularBuffer<float> Buffer = TCircularBuffer<float>(44100);
	uint32 WriteIndex = 0;
	uint32 ReadIndex = 0;


	
	// Called when synth is created
	virtual bool Init(int32& SampleRate) override;

	// Called to generate more audio
	virtual int32 OnGenerateAudio(float* OutAudio, int32 NumSamples) override;

	// Sets the oscillator's frequency
	UFUNCTION(BlueprintCallable, Category = "Synth|Components|Audio")
	void SetFrequency(const float FrequencyHz = 440.0f);

public:

	void WriteSamples(TArrayView<int16> Samples)
	{
		for (int16 Sample : Samples)
		{
			Buffer[WriteIndex] = Sample / 32768.0f;
			WriteIndex = Buffer.GetNextIndex(WriteIndex);
		}
	}

	void WriteSamples(float* Samples, uint32 NumSamples)
	{
		for (uint32 i = 0; i < NumSamples; i++)
		{
			Buffer[WriteIndex] = Samples[i];
			WriteIndex = Buffer.GetNextIndex(WriteIndex);
		}
	}


protected:
#if SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
	// A simple oscillator class. Can also generate Saw/Square/Tri/Noise.
	Audio::FOsc Osc;
#endif // SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
};