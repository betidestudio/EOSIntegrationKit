// Copyright (c) 2024 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_UpdatingSanctions.generated.h"

USTRUCT(BlueprintType)
struct FEIK_UpdatableFields
{
	GENERATED_BODY()

	//List of tags associated with this sanction. Items are case insensitive and unique. Item format: [a-zA-Z0-9_-]+. Item max length: 16
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit|Web")
	TArray<FString> Tags;

	//Arbitrary metadata key/value pairs associated with this sanction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit|Web")
	TMap<FString, FString> Metadata;

	//Justification string associated with this sanction. Min length: 1 Max length: 2048
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit|Web")
	FString Justification;
	
};

USTRUCT(BlueprintType)
struct FEIK_SanctionPatchPayload
{
	GENERATED_BODY()

	//Unique identifier for this sanction	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit|Web")
	FString ReferenceId;

	//Fields to be updated and their new values.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit|Web")
	TArray<FEIK_UpdatableFields> Updates;
};
UCLASS()
class EIKWEB_API UEIK_API_UpdatingSanctions : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web")
	static UEIK_API_UpdatingSanctions* UpdatingSanctions(FString Authorization, FString DeploymentId, TArray<FEIK_SanctionPatchPayload> SanctionPatchPayload);

private:
	virtual void Activate() override;
	FString Var_Authorization;
	FString Var_DeploymentId;
	TArray<FEIK_SanctionPatchPayload> Var_SanctionPatchPayload;
};
