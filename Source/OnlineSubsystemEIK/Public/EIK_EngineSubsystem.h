// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "EIK_EngineSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_EngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;

	void BeginPIE( const bool bIsSimulating );
	
};
