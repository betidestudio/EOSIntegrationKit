// Copyright (c) 2024 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_CreateParty.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEIK_CreatePartyComplete);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_CreateParty : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Party", DisplayName="Create EIK Party")
	static UEIK_CreateParty* EIK_CreateParty(TEnumAsByte<EEIK_ELobbyPermissionLevel> PermissionLevel, int32 MaxPartyMembers);

	UPROPERTY(BlueprintAssignable)
	FEIK_CreatePartyComplete OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FEIK_CreatePartyComplete OnFailure;
	
private:
	int32 Var_MaxPartyMembers;
	FName Var_SessionName;
	bool Var_bUsePresence;
	bool Var_bUseVoiceChat;
	TEnumAsByte<EEIK_ELobbyPermissionLevel> Var_PermissionLevel;
	virtual void Activate() override;
	void OnCreatePartyCompleted(FName SessionName, bool bWasSuccessful);
};
