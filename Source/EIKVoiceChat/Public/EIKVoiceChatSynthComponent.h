// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SynthComponent.h"
#include "DSP/Osc.h"
#include "DSP/Dsp.h"
#include "DSP/FloatArrayMath.h"
#include "Containers/CircularBuffer.h"
#include "Runtime/Launch/Resources/Version.h"
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

	Audio::TCircularAudioBuffer<float> AudioBuffer;

	bool bIsOutArrayInitialized = false;
	TArray<float> OutArray;
	TArrayView<float> OutArrayView;


	// Called when synth is created
	virtual bool Init(int32& SampleRate) override;

	// Called to generate more audio
	virtual int32 OnGenerateAudio(float* OutAudio, int32 NumSamples) override;


protected:

	friend class FEOSVoiceChatUser;

	// called by EOSVoiceChatUser to fill the buffer, as long as these are samples available these should be consumed via OnGenerateAudio
	void WriteSamples(TArrayView<int16> Samples)
	{
		// Ensure the OutArray has enough space to hold the converted float data
		if (OutArray.Num() < Samples.Num())
		{
			OutArray.SetNum(Samples.Num());  // Resize OutArray to match the size of Samples
			OutArrayView = TArrayView<float>(OutArray.GetData(), Samples.Num());  // Recreate OutArrayView with the new size
		}
#if ENGINE_MAJOR_VERSION >= 5
		// Convert PCM16 to float and write to OutArrayView
		Audio::ArrayPcm16ToFloat(Samples, OutArrayView);
#else
		// Convert each PCM16 sample to a float in the range [-1.0, 1.0]
		for (int32 Index = 0; Index < Samples.Num(); ++Index)
		{
			OutArrayView[Index] = static_cast<float>(Samples[Index]) / 32768.0f;
		}
#endif
		// Push the float data to the circular buffer
		AudioBuffer.Push(OutArrayView.GetData(), Samples.Num());
	}

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EOS Integration Kit|Voice Settings")
	TArray<FString> SupportedRooms;

	// This component will be used for ALL rooms, regardless of added values in SupportedRooms
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EOS Integration Kit|Voice Settings")
	bool bUseGlobalRoom = false;

};