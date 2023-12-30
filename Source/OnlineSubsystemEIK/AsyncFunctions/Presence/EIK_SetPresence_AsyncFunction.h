// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"
#include "IOnlineSubsystemEOS.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "EIK_SetPresence_AsyncFunction.generated.h"

/**
 * 
 */
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_SetPresence_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	FString RichPresence;
	
	//EPresenceStatus PresenceStatus;

	virtual void Activate() override;

	void SetPresence();

	UFUNCTION(BlueprintCallable, DisplayName = "Set EOS Presence", meta = (BlueprintInternalUseOnly = "true"), Category = "EOS Integration Kit || Presence")
	static UEIK_SetPresence_AsyncFunction* SetEOSPresence(FString RichPresense);//, EPresenceStatus PresenceStatus);

};
