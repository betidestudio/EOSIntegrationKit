// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GPGS_ShowSavedGamesUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGPGS_ShowSavedGamesUIOutputSignature, const FString&, ID, const FString&, Error);

UCLASS()
class EIKLOGINMETHODS_API UGPGS_ShowSavedGamesUI : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="EOS Integration Kit|Google Play Games|Saved Games")
	static UGPGS_ShowSavedGamesUI* ShowSavedGamesUI(UObject* WorldContextObject, int Max = 5);

	virtual void Activate() override;
	virtual void BeginDestroy() override;

	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Google Play Games")
	FGPGS_ShowSavedGamesUIOutputSignature Selected;

	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Google Play Games")
	FGPGS_ShowSavedGamesUIOutputSignature CreateNew;
	
	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Google Play Games")
	FGPGS_ShowSavedGamesUIOutputSignature ClickedBack;

	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Google Play Games")
	FGPGS_ShowSavedGamesUIOutputSignature Failure;
	
	static TWeakObjectPtr<UGPGS_ShowSavedGamesUI> StaticInstance;

private:
	int32 Var_Max;
};
