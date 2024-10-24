// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_CreateSanctions.generated.h"

USTRUCT(BlueprintType)
struct FEIK_SanctionPostPayload
{
	GENERATED_BODY()

	//Sanctioned user’s EOS ProductUserId
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit|Web")
	FString ProductUserId;

	//Action string associated with this sanction. Format: [a-zA-Z0-9_-]+. Min length: 1. Max length: 64.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit|Web")
	FString Action;

	//Justification string associated with this sanction. Min length: 1 Max length: 2048
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit|Web")
	FString Justification;

	//Source which created this sanction, e.g. developer-portal. Format: [a-zA-Z0-9_-]+. Min length: 2. Max length: 64.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit|Web")
	FString Source;

	//List of tags associated with this sanction. Items are case insensitive and unique. Item format: [a-zA-Z0-9_-]+. Item max length: 16
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit|Web")
	TArray<FString> Tags;

	//True if this sanction is currently pending
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit|Web")
	bool bPending;

	//Arbitrary metadata key/value pairs associated with this sanction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit|Web")
	TMap<FString, FString> Metadata;

	//Display name of sanctioned user. Max length: 64
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit|Web")
	FString DisplayName;

	//Identity provider that the sanctioned user authenticated with. Max length: 64
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit|Web")
	FString IdentityProvider;

	//Sanctioned user's account ID with the specified identityProvider. Max length: 64
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit|Web")
	FString AccountId;

	//The length of the sanction in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit|Web")
	int32 Duration = 0;

	FEIK_SanctionPostPayload()
	{
		ProductUserId = "";
		Action = "";
		Justification = "";
		Source = "";
		Tags = TArray<FString>();
		bPending = false;
		Metadata = TMap<FString, FString>();
		DisplayName = "";
		IdentityProvider = "";
		AccountId = "";
		Duration = 0;
	}
	
};
UCLASS()
class EIKWEB_API UEIK_API_CreateSanctions : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web")
	static UEIK_API_CreateSanctions* CreateSanctions(FString Authorization, FString DeploymentId, TArray<FEIK_SanctionPostPayload> Sanctions);

private:
	virtual void Activate() override;
	FString Var_Authorization;
	FString Var_DeploymentId;
	TArray<FEIK_SanctionPostPayload> Var_Sanctions;
};
