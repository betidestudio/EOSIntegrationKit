// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleInterface.h"
#include "Templates/SharedPointer.h"

using IVoiceChatPtr = TSharedPtr<class IVoiceChat, ESPMode::ThreadSafe>;
typedef TSharedPtr<class FEOSVoiceChatFactory, ESPMode::ThreadSafe> FEOSVoiceChatFactoryPtr;

class FEIKVoiceChatModule : public IModuleInterface
{
public:
	FEIKVoiceChatModule() = default;
	~FEIKVoiceChatModule() = default;

private:
	// ~Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// ~End IModuleInterface

	/** Singleton EOS object */
	FEOSVoiceChatFactoryPtr EOSFactory;
	IVoiceChatPtr EOSObj;
};