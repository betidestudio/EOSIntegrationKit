// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "Runtime/Online/HTTP/Public/Http.h"
#include "EIKSettings.h"
#include "EIK_SendSupportTicket_Async.generated.h"

UENUM(BlueprintType)
enum class ESupportTicketSubject : uint8
{
    ST_open_question UMETA(DisplayName = "Open Question"),
    ST_technical_support UMETA(DisplayName = "Technical Support"),
    ST_ban_appeal UMETA(DisplayName = "Ban Appeal")
};



USTRUCT(BlueprintType)
struct FSupportTicketData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets", meta = (ToolTip = "Subject of the support ticket. This is a required field."))
    ESupportTicketSubject Subject = ESupportTicketSubject::ST_open_question;

    UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets", meta = (ToolTip = "Message or description for the support ticket. This is a required field."))
    FString Message = "";

    UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets", meta = (ToolTip = "Email address of the sender. This is a required field."))
    FString SenderEmail = "";

    UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets", meta = (ToolTip = "Name of the sender."))
    FString SenderName = "";

    UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets", meta = (ToolTip = "Unique identifier for the end-user."))
    FString Guid = "";

    UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets", meta = (ToolTip = "Error code for technical support emails."))
    FString ErrorCode = "";

    UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets", meta = (ToolTip = "Operating system of the end-user."))
    FString SystemOS = "";

    UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets", meta = (ToolTip = "Anti-malware system of the end-user."))
    FString SystemAntiMalware = "";

    UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets", meta = (ToolTip = "Other information about the end-user's system."))
    FString SystemOther = "";

};

USTRUCT(BlueprintType)
struct FSupportTicketResponseData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets")
    FString prod_name = "";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets")
    FString prod_slug = "";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets")
    FString guid = "";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets")
    FString sender_name = "";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets")
    FString sender_email = "";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets")
    FString subject = "";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets")
    FString message = "";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets")
    FString error_code = "";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets")
    FString system_os = "";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets")
    FString system_antimalware = "";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets")
    FString system_other = "";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit || Support Tickets")
    FString timestamp = "";
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTicketRequestComplete, const FString&, ResponseStr, const FSupportTicketResponseData&, ResponseData, const int32&, HTTP_ResponseCode);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_SendSupportTicket_Async : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
    // Variables
    FSupportTicketData Var_TicketData;

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit || Support Tickets")
    FOnTicketRequestComplete Success;

    UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit || Support Tickets")
    FOnTicketRequestComplete Failure;

    // Functions
    void Activate() override;

    UFUNCTION(BlueprintCallable, DisplayName = "Send EIK Support Ticket Request", meta = (BlueprintInternalUseOnly = "true"), Category = "EOS Integration Kit | Support Tickets")
    static UEIK_SendSupportTicket_Async* SendEIKTicketRequestAsyncFunction(FSupportTicketData TicketData);

    void SendTicketRequest();
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};

