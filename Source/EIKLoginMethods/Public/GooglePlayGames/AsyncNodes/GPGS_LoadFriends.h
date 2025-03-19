// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GooglePlayGames/GooglePlayGamesStructures.h"
#include "GPGS_LoadFriends.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGPGS_LoadFriendsCallbackSignature, const TArray<FGPGS_Friend>&, Friends, const FString&, error);

/**
 * 
 */
UCLASS()
class EIKLOGINMETHODS_API UGPGS_LoadFriends : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="EOS Integration Kit|Google Play Games|Friends")
	static UGPGS_LoadFriends* LoadFriends(UObject* WorldContextObject, int Max = 10, bool bForceReload = false);

	virtual void Activate() override;
	virtual void BeginDestroy() override;

	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Google Play Games")
	FGPGS_LoadFriendsCallbackSignature Success;

	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Google Play Games")
	FGPGS_LoadFriendsCallbackSignature Failure;

	void HandleCallback(bool bSuccess, const FString& FriendsJson, const FString& Error);
	
	static TWeakObjectPtr<UGPGS_LoadFriends> StaticInstance;

private:
	int Var_Max;
	bool Var_bForceReload;
};
