// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "eos_connect.h"
#include "eos_connect_types.h"

#include "EIK_GetExternalAccountsFromPUID.generated.h"

UENUM(BlueprintType)
enum class ExternalAccountTypes : uint8
{
	EOS_EAT_EPIC UMETA(DisplayName = "Epic Games"),
	EOS_EAT_STEAM UMETA(DisplayName = "Steam"),
	EOS_EAT_PSN UMETA(DisplayName = "PlayStation Network"),
	EOS_EAT_XBL UMETA(DisplayName = "Xbox Live"),
	EOS_EAT_DISCORD UMETA(DisplayName = "Discord"),
	EOS_EAT_GOG UMETA(DisplayName = "GOG"),
	EOS_EAT_NINTENDO UMETA(DisplayName = "Nintendo"),
	EOS_EAT_UPLAY UMETA(DisplayName = "Uplay"),
	EOS_EAT_OPENID UMETA(DisplayName = "OpenID Provider"),
	EOS_EAT_APPLE UMETA(DisplayName = "Apple"),
	EOS_EAT_GOOGLE UMETA(DisplayName = "Google"),
	EOS_EAT_OCULUS UMETA(DisplayName = "Oculus"),
	EOS_EAT_ITCHIO UMETA(DisplayName = "itch.io"),
	EOS_EAT_AMAZON UMETA(DisplayName = "Amazon")
};

USTRUCT(BlueprintType)
struct FExternalAccountIdAndType
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || UserInfo")
	ExternalAccountTypes ExternalAccountType = ExternalAccountTypes::EOS_EAT_EPIC;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || UserInfo")
	FString AccountId = "";

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || UserInfo")
	FString DisplayName = "";

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || UserInfo")
	FDateTime LastLogin;

};

USTRUCT(BlueprintType)
struct FProductUserIdAndExternalAccountIds
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || UserInfo")
	TArray<FExternalAccountIdAndType> ExternalAccountIds;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || UserInfo")
	FString ProductUserId = "";

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetExternalAccountsFromPUIDDelegate, const TArray<FProductUserIdAndExternalAccountIds>&, UserInfo);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_GetExternalAccountsFromPUID : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, DisplayName = "Get External Accounts From Product User Ids", meta = (BlueprintInternalUseOnly = "true"), Category = "EOS Integration Kit || UserInfo")
	static UEIK_GetExternalAccountsFromPUID* EIKGetExternalAccountsFromPuid(TArray<FString> TargetProductUserIds, FString LocalProductUserId);

	TArray<FString> Var_TargetUserIds;

	FString Var_LocalProductUserId;

	void GetExternalAccountsFromPuid();

	static void OnQueryProductUserIdMappingsComplete(const EOS_Connect_QueryProductUserIdMappingsCallbackInfo* Data);

	void QueryProductUserIdMappingsSuccess();

	void QueryProductUserIdMappingsFailure();

	virtual void Activate() override;


	UPROPERTY(BlueprintAssignable)
	FGetExternalAccountsFromPUIDDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FGetExternalAccountsFromPUIDDelegate Failure;
};
