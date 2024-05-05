// Copyright Epic Games, Inc. All Rights Reserved.


#include "EIK_DeleteTicketData_AsyncFunct.h"

#include "EIKSettings.h"

#include "EIKSettings.h"
#include "Runtime/Online/HTTP/Public/Http.h"

void UEIK_DeleteTicketData_AsyncFunct::Activate()
{
    DeleteTicketData();
    Super::Activate();
}

UEIK_DeleteTicketData_AsyncFunct* UEIK_DeleteTicketData_AsyncFunct::DeleteEIKTicketDataAsyncFunction(FString Email)
{
    UEIK_DeleteTicketData_AsyncFunct* UEIK_DeleteEIKTicketData = NewObject<UEIK_DeleteTicketData_AsyncFunct>();

    UEIK_DeleteEIKTicketData->Var_Email = Email;

    return UEIK_DeleteEIKTicketData;
}

void UEIK_DeleteTicketData_AsyncFunct::DeleteTicketData()
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL("https://dev.epicgames.com/portal/api/v1/services/tickets/datarights/" + FString(Var_Email));
    Request->SetVerb("DELETE");
    Request->SetHeader("Content-Type", "application/json");

    if (UEIKSettings* EIKSettings = GetMutableDefault<UEIKSettings>())
    {
        if (EIKSettings->ApiKey.IsEmpty())
        {
            // Log an error message
            UE_LOG(LogTemp, Error, TEXT("API key is empty. Cancelling function call."));

            Failure.Broadcast(TEXT("API key is empty."), 0, false);

            return;
        }
        else
        {
            FString APIKey = EIKSettings->ApiKey;
            Request->SetHeader("Authorization", "Token " + FString(APIKey));
        }
    }
    else
    {
        Failure.Broadcast(TEXT("Couldn't retrieve your API key from EIK settings."), 0, false);
        return;
    }

    Request->OnProcessRequestComplete().BindUObject(this, &UEIK_DeleteTicketData_AsyncFunct::OnResponseReceived);

    Request->ProcessRequest();
}

void UEIK_DeleteTicketData_AsyncFunct::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        // Handle a successful delete request here
        int32 HTTP_ResponseCode = Response->GetResponseCode();
        if (HTTP_ResponseCode == 200)
        {
            UE_LOG(LogTemp, Log, TEXT("User data deleted successfully."));
            Success.Broadcast(TEXT("User data deleted successfully."), HTTP_ResponseCode, true);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to delete user data. HTTP response code: %d"), HTTP_ResponseCode);
            Failure.Broadcast(TEXT("Failed to delete user data."), HTTP_ResponseCode, false);
        }
    }
    else
    {
        // Handle a failed delete request here
        UE_LOG(LogTemp, Error, TEXT("Failed to delete user data."));
        Failure.Broadcast(TEXT("Failed to delete user data."), 0, false);
    }
}
