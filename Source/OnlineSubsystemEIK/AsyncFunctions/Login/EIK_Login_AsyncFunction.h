// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineError.h"
#include "OnlineSubsystem.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Runtime/CoreOnline/Public/Online/CoreOnline.h"
#include "EIK_Login_AsyncFunction.generated.h"

UENUM(BlueprintType)
enum class ELoginTypes : uint8 {
	AccountPortal       UMETA(DisplayName="Account Portal"),
	PersistentAuth              UMETA(DisplayName="Persistent Auth"),
	EpicLauncher        UMETA(DisplayName="Epic Launcher"),
	DeviceID        UMETA(DisplayName="Device ID"),
	Google              UMETA(DisplayName="Google"),
	Steam              UMETA(DisplayName="Steam"),
	Apple              UMETA(DisplayName="Apple"),
	Discord              UMETA(DisplayName="Discord"),
	Oculus              UMETA(DisplayName="Oculus"),
	OpenID              UMETA(DisplayName="OpenID"),
	Developer           UMETA(DisplayName="Developer"),
	None           UMETA(DisplayName="None"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResponsDelegate, FString, ProductUserID, FString, Error);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Login_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintAssignable)
	FResponsDelegate OnSuccess;
	
	UPROPERTY(BlueprintAssignable)
	FResponsDelegate OnFail;
	
	ELoginTypes LoginMethod;
	FString Input1;
	FString Input2;
	IOnlineSubsystem* AppleSubsystem;

	bool bDelegateCalled = false;

	/*
	This C++ method logs in a user to an online subsystem using the selected method and sets up a callback function to handle the login response.
	Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/authentication/
	For Input Parameters, please refer to the documentation link above.
	
	Following Methods are coming with upcoming build - Apple, Discord, Oculus, OpenID
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Login")
	static UEIK_Login_AsyncFunction* LoginUsingEIK(ELoginTypes LoginMethod, FString DisplayName, FString Token);
	
	virtual void Activate() override;

	void Login();

	void LoginCallback(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error);

	void LoginWithAppleCallback(TSharedPtr<const FUniqueNetId> UniqueNetId, int I, const FOnlineError& OnlineError);
	void LoginWithApple();
	
};
