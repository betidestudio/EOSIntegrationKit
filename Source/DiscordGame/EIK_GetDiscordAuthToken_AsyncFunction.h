// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Runtime/Launch/Resources/Version.h"
#include "EIK_GetDiscordAuthToken_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_OnGetDiscordAuthTokenComplete, const FString&, AuthToken, const FString&, Error);

UCLASS()
class DISCORDGAME_API UEIK_GetDiscordAuthToken_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), DisplayName="Get Discord Auth Token", Category = "EOS Integration Kit|Extra")
	static UEIK_GetDiscordAuthToken_AsyncFunction* GetDiscordAuthToken();

	UPROPERTY(BlueprintAssignable)
	FEIK_OnGetDiscordAuthTokenComplete OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FEIK_OnGetDiscordAuthTokenComplete OnFailure;

private:
	virtual void Activate() override;
	
};
