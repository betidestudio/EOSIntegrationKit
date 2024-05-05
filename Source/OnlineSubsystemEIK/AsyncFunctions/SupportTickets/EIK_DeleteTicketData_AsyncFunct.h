// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "EIKSettings.h"
#include "Runtime/Online/HTTP/Public/Http.h"

#include "EIK_DeleteTicketData_AsyncFunct.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTicketDeleteComplete, const FString&, ResponseStr, const int32&, HTTP_ResponseCode, bool, bWasSuccessful);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_DeleteTicketData_AsyncFunct : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    // Variables
    FString Var_Email;

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit || Support Tickets")
    FOnTicketDeleteComplete Success;

    UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit || Support Tickets")
    FOnTicketDeleteComplete Failure;

    // Functions
    void Activate() override;

    UFUNCTION(BlueprintCallable, DisplayName = "Delete EIK Support Ticket Data", meta = (BlueprintInternalUseOnly = "true"), Category = "EOS Integration Kit | Support Tickets ")
    static UEIK_DeleteTicketData_AsyncFunct* DeleteEIKTicketDataAsyncFunction(FString Email);

    void DeleteTicketData();
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};

