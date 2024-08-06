// Copyright (c) 2024 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_CreateParty.generated.h"

USTRUCT(BlueprintType)
struct FEIK_PartyExtraSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || Party")
	FName SessionName = "PartySession";

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || Party")
	int32 MaxPrivatePartyMembers = 0;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || Party")
	bool bUsePresence = true;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || Party")
	bool bUseVoiceChat = true;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEIK_CreatePartyComplete);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_CreateParty : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Party", DisplayName="Create EIK Party")
	static UEIK_CreateParty* EIK_CreateParty(int32 MaxPublicPartyMembers, FEIK_PartyExtraSettings ExtraPartySettings);

	UPROPERTY(BlueprintAssignable)
	FEIK_CreatePartyComplete OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FEIK_CreatePartyComplete OnFailure;
	
private:
	FEIK_PartyExtraSettings Var_CreatePartySettings;
	int32 Var_MaxPublicPartyMembers;
	virtual void Activate() override;
	void OnCreatePartyCompleted(FName SessionName, bool bWasSuccessful);
};
