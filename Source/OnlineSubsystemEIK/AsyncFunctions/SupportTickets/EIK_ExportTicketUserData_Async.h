// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "EIKSettings.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "EIK_SendSupportTicket_Async.h"

#include "EIK_ExportTicketUserData_Async.generated.h"

USTRUCT(BlueprintType)
struct FMessageData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Message Data")
    int32 TicketId = 0;

    UPROPERTY(BlueprintReadWrite, Category = "Message Data")
    FString Message = "";

    UPROPERTY(BlueprintReadWrite, Category = "Message Data")
    FString SenderName = "";

    UPROPERTY(BlueprintReadWrite, Category = "Message Data")
    FString SenderEmail = "";

    UPROPERTY(BlueprintReadWrite, Category = "Message Data")
    FString Timestamp = "";
};

USTRUCT(BlueprintType)
struct FConversationData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Data")
    FString Guid = "";

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Data")
    FString Subject = "";

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Data")
    FString Message = "";

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Data")
    FString SenderName = "";

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Data")
    FString SenderEmail = "";

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Data")
    FString Timestamp = "";

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Data")
    TArray<FMessageData> Messages;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTicketExportComplete, const FString&, ResponseStr, const TArray<FConversationData>&, Conversations, const int32&, HTTP_ResponseCode);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_ExportTicketUserData_Async : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    // Variables
    FString Var_Email;

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit || Support Tickets")
    FOnTicketExportComplete Success;

    UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit || Support Tickets")
    FOnTicketExportComplete Failure;

    // Functions
    void Activate() override;

    UFUNCTION(BlueprintCallable, DisplayName = "Export EIK Support Ticket Data", meta = (BlueprintInternalUseOnly = "true"), Category = "EOS Integration Kit | Support Tickets")
    static UEIK_ExportTicketUserData_Async* ExportEIKTicketDataAsyncFunction(FString Email);

    void ExportTicketData();
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};

