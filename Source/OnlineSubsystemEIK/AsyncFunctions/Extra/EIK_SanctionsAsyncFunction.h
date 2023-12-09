//Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "eos_sanctions.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "EIK_SanctionsAsyncFunction.generated.h"

USTRUCT(BlueprintType)
struct FSanctionsStruct
{
	GENERATED_BODY()
public:
	/** API Version: This will be set to EOS_SANCTIONS_PLAYERSANCTION_API_LATEST. */
	UPROPERTY(BlueprintReadOnly, Category = "EOS Integration Kit | Sanctions")
	int32 ApiVersion = EOS_SANCTIONS_PLAYERSANCTION_API_LATEST;
	/** The POSIX timestamp when the sanction was placed */
	UPROPERTY(BlueprintReadOnly, Category = "EOS Integration Kit | Sanctions")
	int64 TimePlaced = 0;
	/** The action associated with this sanction */
	UPROPERTY(BlueprintReadOnly, Category = "EOS Integration Kit | Sanctions")
	FString Action = "";
	/** The POSIX timestamp when the sanction will expire. If the sanction is permanent, this will be 0. */
	UPROPERTY(BlueprintReadOnly, Category = "EOS Integration Kit | Sanctions")
	int64 TimeExpires = 0;
	/** A unique identifier for this specific sanction */
	UPROPERTY(BlueprintReadOnly, Category = "EOS Integration Kit | Sanctions")
	FString ReferenceId = "";
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSanctionsDelegate, const TArray<FSanctionsStruct>&, Sanctions);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_SanctionsAsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
		/*
	This C++ method gets the logged in user's achievements from the EOS backend.
	Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/authentication/
	*/
	UFUNCTION(BlueprintCallable, DisplayName="Get EIK Player Sanctions",meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Others")
	static UEIK_SanctionsAsyncFunction* GetEikPlayerSanctionsAsyncFunction(FString LocalProductUserID, FString TargetProductUserID);

	FString Var_LocalProductUserID;
	FString Var_TargetProductUserID;

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | Sanctions")
	FSanctionsDelegate Success;

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | Sanctions")
	FSanctionsDelegate Failure;
	
	void Func_GetSanctions();

	static void EOS_CALL ReturnFunc(const EOS_Sanctions_QueryActivePlayerSanctionsCallbackInfo* Data);

	void GetFinalValues();
	void FireFailure();
	void Activate() override;
};
