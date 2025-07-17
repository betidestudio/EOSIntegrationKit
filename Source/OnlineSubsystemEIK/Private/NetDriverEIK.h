// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NetDriverEIKBase.h"
#include "NetDriverEIKBase.h"
#include "NetDriverEIK.generated.h"

class ISocketSubsystem;

UCLASS(Transient, Config=Engine)
class ONLINESUBSYSTEMEIK_API UNetDriverEIK
	: public UNetDriverEIKBase
{
	GENERATED_BODY()

public:
	UNetDriverEIK(const FObjectInitializer& ObjectInitializer);
};