// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "eos_userinfo.h"
#include "eos_userinfo_types.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EIK_FindUserByDisplayName_Async.generated.h"

/**
 *
 */

USTRUCT(BlueprintType, Category = "EOS Integration Kit|UserInfo")
struct FEIKUserInfo
{
    GENERATED_BODY()

    /** The Epic Account ID in string format of the found user */
    UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit|UserInfo")
    FString EpicAccountID;
    /** The name of the found users country. This may be null */
    UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit|UserInfo")
    FString Country;
    /** The display name (un-sanitized). This may be null */
    UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit|UserInfo")
    FString DisplayName;
    /** The ISO 639 language code for the user's preferred language. This may be null */
    UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit|UserInfo")
    FString PreferredLanguage;
    /** A nickname/alias for the target user assigned by the local user. This may be null */
    UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit|UserInfo")
    FString Nickname;
    /** The raw display name (sanitized). This may be null */
    UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit|UserInfo")
    FString DisplayNameSanitized;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFindUserByDisplayNameDelegate, const FEIKUserInfo, EIKUserInfo);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_FindUserByDisplayName_Async : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, DisplayName = "Find EIK Player By Display Name", meta = (BlueprintInternalUseOnly = "true"), Category = "EOS Integration Kit || UserInfo")
	static UEIK_FindUserByDisplayName_Async* FindEIkUserByDisplayName(FString TargetDisplayName, FString LocalEpicID);

	FString TargetDisplayName;

	FString LocalEpicID;

	void FindUserByDisplayName();

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit || UserInfo")
    FFindUserByDisplayNameDelegate Success;

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit || UserInfo")
    FFindUserByDisplayNameDelegate Failure;

	static void EOS_CALL FindUserByDisplayNameCallback(const EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo* Data);

	void ResultFaliure();

	void ResultSuccess(const FEIKUserInfo UserInfoStruct);

	void Activate() override;
};
