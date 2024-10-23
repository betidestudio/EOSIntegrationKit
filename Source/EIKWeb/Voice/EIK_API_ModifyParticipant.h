// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_ModifyParticipant.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_ModifyParticipant : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:
	
	//Room members can be remote-muted, i.e. be muted for everyone, independent of their local mute status.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web")
	static UEIK_API_ModifyParticipant* ModifyParticipant(FString Authorization, FString DeploymentId, FString RoomId, FString ProductUserId, bool bHardMuted);

private:
	virtual void Activate() override;
	FString Var_Authorization;
	FString Var_DeploymentId;
	FString Var_RoomId;
	FString Var_ProductUserId;
	bool Var_bHardMuted;
};
