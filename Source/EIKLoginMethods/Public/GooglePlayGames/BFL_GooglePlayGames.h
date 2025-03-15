// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_GooglePlayGames.generated.h"

UCLASS()
class EIKLOGINMETHODS_API UBFL_GooglePlayGames : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UBFL_GooglePlayGames();

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games")
	static void ManualSignIn();

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games")
	static FString GetUsername();

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games")
	static FString GetPlayerID();
	
};
