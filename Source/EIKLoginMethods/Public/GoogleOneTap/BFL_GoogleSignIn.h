// Copyright (C) 2024 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_GoogleSignIn.generated.h"

UCLASS()
class EIKLOGINMETHODS_API UBFL_GoogleSignIn : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UBFL_GoogleSignIn();

	UFUNCTION(BlueprintCallable, meta = (DisplayName="GoogleSubsystem_GetUserDisplayName"), Category = "EOS Integration Kit|GoogleSubsystem")
	static FString GetUserDisplayName();

	UFUNCTION(BlueprintCallable, meta = (DisplayName="GoogleSubsystem_GetUserFirstName"), Category = "EOS Integration Kit|GoogleSubsystem")
	static FString GetUserFirstName();

	UFUNCTION(BlueprintCallable, meta = (DisplayName="GoogleSubsystem_GetUserLastName"), Category = "EOS Integration Kit|GoogleSubsystem")
	static FString GetUserLastName();

	UFUNCTION(BlueprintCallable, meta = (DisplayName="GoogleSubsystem_GetUserProfilePictureUrl"), Category = "EOS Integration Kit|GoogleSubsystem")
	static FString GetUserProfilePictureUrl();

	UFUNCTION(BlueprintCallable, meta = (DisplayName="GoogleSubsystem_IsUserLoggedIn"), Category = "EOS Integration Kit|GoogleSubsystem")
	static bool IsUserLoggedIn();
	
};
