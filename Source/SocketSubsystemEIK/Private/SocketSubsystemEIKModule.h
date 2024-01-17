// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleInterface.h"

class FSocketSubsystemEIKModule: public IModuleInterface
{
public:
	FSocketSubsystemEIKModule() = default;
	~FSocketSubsystemEIKModule() = default;

private:
	// ~Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// ~End IModuleInterface
};