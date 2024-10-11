// Copyright Epic Games, Inc. All Rights Reserved.


#include "EIK_ExportTicketUserData_Async.h"


#include "EIKSettings.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "EIK_SendSupportTicket_Async.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

void UEIK_ExportTicketUserData_Async::Activate()
{
	ExportTicketData();
	Super::Activate();
}

UEIK_ExportTicketUserData_Async* UEIK_ExportTicketUserData_Async::ExportEIKTicketDataAsyncFunction(FString Email)
{	
	UEIK_ExportTicketUserData_Async* UEIK_ExportEIKUserDataRequest = NewObject<UEIK_ExportTicketUserData_Async>();

	UEIK_ExportEIKUserDataRequest->Var_Email = Email;

	return UEIK_ExportEIKUserDataRequest;
}

void UEIK_ExportTicketUserData_Async::ExportTicketData()
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL("https://dev.epicgames.com/portal/api/v1/services/tickets/datarights/" + FString(Var_Email));
    Request->SetVerb("GET");
    Request->SetHeader("Content-Type", "application/json");

    if (UEIKSettings* EIKSettings = GetMutableDefault<UEIKSettings>())
    {
        if (EIKSettings->ApiKey.IsEmpty())
        {
            // Log an error message
            UE_LOG(LogTemp, Error, TEXT("API key is empty. Cancelling function call."));

            Failure.Broadcast(TEXT("API key is empty."), TArray<FConversationData>(), 0);
            SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
            MarkAsGarbage();
#else
            MarkPendingKill();
#endif

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
        Failure.Broadcast(TEXT("Couldn't retrieve your API key from EIK settings."), TArray<FConversationData>(), 1);
        SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
        MarkAsGarbage();
#else
        MarkPendingKill();
#endif

        return;
    }

    Request->OnProcessRequestComplete().BindUObject(this, &UEIK_ExportTicketUserData_Async::OnResponseReceived);

    Request->ProcessRequest();
}

void UEIK_ExportTicketUserData_Async::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        FString ResponseStr = Response->GetContentAsString();

        // Parse the JSON response here and return the data
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);

        if (FJsonSerializer::Deserialize(Reader, JsonObject))
        {
            TArray<TSharedPtr<FJsonValue>> DataArray = JsonObject->GetArrayField(TEXT("data"));

            TArray<FConversationData> Conversations;

            for (int32 i = 0; i < DataArray.Num(); i++)
            {
                TSharedPtr<FJsonObject> DataObject = DataArray[i]->AsObject();

                FConversationData ConversationData;

                // Fill the ConversationData structure with the parsed data
                ConversationData.Guid = DataObject->GetStringField(TEXT("guid"));
                ConversationData.Subject = DataObject->GetStringField(TEXT("subject"));
                ConversationData.Message = DataObject->GetStringField(TEXT("message"));
                ConversationData.SenderName = DataObject->GetStringField(TEXT("sender_name"));
                ConversationData.SenderEmail = DataObject->GetStringField(TEXT("sender_email"));
                ConversationData.Timestamp = DataObject->GetStringField(TEXT("timestamp"));

                // Parse the "messages" array
                TArray<TSharedPtr<FJsonValue>> MessagesArray = DataObject->GetArrayField(TEXT("messages"));
                for (int32 j = 0; j < MessagesArray.Num(); j++)
                {
                    TSharedPtr<FJsonObject> MessageObject = MessagesArray[j]->AsObject();

                    FMessageData Message;
                    Message.TicketId = MessageObject->GetIntegerField(TEXT("ticket_id"));
                    Message.Message = MessageObject->GetStringField(TEXT("message"));
                    Message.SenderName = MessageObject->GetStringField(TEXT("sender_name"));
                    Message.SenderEmail = MessageObject->GetStringField(TEXT("sender_email"));
                    Message.Timestamp = MessageObject->GetStringField(TEXT("timestamp"));

                    ConversationData.Messages.Add(Message);
                }

                Conversations.Add(ConversationData);
            }
            Success.Broadcast(ResponseStr, Conversations, Response->GetResponseCode());
            SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
            MarkAsGarbage();
#else
            MarkPendingKill();
#endif
        }
    }
    else
    {
        Failure.Broadcast(TEXT("Request failed."), TArray<FConversationData>(), Response->GetResponseCode());
        SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
        MarkAsGarbage();
#else
        MarkPendingKill();
#endif
    }
}

