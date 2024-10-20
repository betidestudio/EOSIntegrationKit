// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "AntiCheatClient.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOS.h"

bool UAntiCheatClient::IsAntiCheatClientAvailable(const UObject* WorldContextObject)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(EOSRef->AntiCheatClientHandle)
			{
				return true;
			}
		}
	}
	return false;
}

bool UAntiCheatClient::RegisterAntiCheatClient(FString ClientProductID)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(!EOSRef->AntiCheatClientHandle)
			{
				PrintAdvancedLogs(FString::Printf(TEXT("RegisterAntiCheatClient-> AntiCheatClientHandle is null")));
				return false;
			}
			{
				EOS_AntiCheatClient_AddNotifyMessageToServerOptions Options = {};
				Options.ApiVersion = EOS_ANTICHEATCLIENT_ADDNOTIFYMESSAGETOSERVER_API_LATEST;
				MessageToServerId = EOS_AntiCheatClient_AddNotifyMessageToServer(EOSRef->AntiCheatClientHandle, &Options, this, OnMessageToServerCallback);
			}
			{
				EOS_AntiCheatClient_BeginSessionOptions Options = {};
				Options.ApiVersion = EOS_ANTICHEATCLIENT_BEGINSESSION_API_LATEST;
				Options.LocalUserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*ClientProductID));
				Options.Mode = EOS_EAntiCheatClientMode::EOS_ACCM_ClientServer;
				
				const EOS_EResult Result = EOS_AntiCheatClient_BeginSession(EOSRef->AntiCheatClientHandle, &Options);
				if(Result == EOS_EResult::EOS_Success)
				{
					return true;
				}
				PrintAdvancedLogs(FString::Printf(TEXT("EOS_AntiCheatClient_BeginSession Result: %hs"), EOS_EResult_ToString(Result)));
				return false;
			}
		}
		PrintAdvancedLogs(FString::Printf(TEXT("RegisterAntiCheatClient-> FOnlineSubsystemEOS is null")));
		return false;
	}
	PrintAdvancedLogs(FString::Printf(TEXT("RegisterAntiCheatClient-> IOnlineSubsystem is null")));
	return false;
}

bool UAntiCheatClient::RecievedMessageFromServer(const TArray<uint8>& Message)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(!EOSRef->AntiCheatClientHandle)
			{
				PrintAdvancedLogs(FString::Printf(TEXT("RecievedMessageFromServer-> AntiCheatClientHandle is null")));
				return false;
			}
			EOS_AntiCheatClient_ReceiveMessageFromServerOptions Options = {};
			Options.ApiVersion = EOS_ANTICHEATCLIENT_RECEIVEMESSAGEFROMSERVER_API_LATEST;
			Options.Data = Message.GetData();
			Options.DataLengthBytes = Message.Num();
			const EOS_EResult Result = EOS_AntiCheatClient_ReceiveMessageFromServer(EOSRef->AntiCheatClientHandle, &Options);
			if(Result == EOS_EResult::EOS_Success)
			{
				return true;
			}
			PrintAdvancedLogs(FString::Printf(TEXT("RecievedMessageFromServer-> %hs"), EOS_EResult_ToString(Result)));
			return false;
		}
		PrintAdvancedLogs(FString::Printf(TEXT("RecievedMessageFromServer-> FOnlineSubsystemEOS is null")));
		return false;
	}
	PrintAdvancedLogs(FString::Printf(TEXT("RecievedMessageFromServer-> IOnlineSubsystem is null")));
	return false;
}

bool UAntiCheatClient::UnregisterAntiCheatClient()
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(!EOSRef->AntiCheatClientHandle)
			{
				PrintAdvancedLogs(FString::Printf(TEXT("UnregisterAntiCheatClient-> AntiCheatClientHandle is null")));
				return false;
			}
			{
				EOS_AntiCheatClient_RemoveNotifyMessageToServer(EOSRef->AntiCheatClientHandle, MessageToServerId);
			}
			EOS_AntiCheatClient_EndSessionOptions Options = {};
			Options.ApiVersion = EOS_ANTICHEATCLIENT_ENDSESSION_API_LATEST;
			const EOS_EResult Result = EOS_AntiCheatClient_EndSession(EOSRef->AntiCheatClientHandle, &Options);
			if (Result == EOS_EResult::EOS_Success)
			{
				return true;
			}
			PrintAdvancedLogs(FString::Printf(TEXT("EOS_AntiCheatClient_EndSession Result: %hs"), EOS_EResult_ToString(Result)));
			return false;
		}
		PrintAdvancedLogs(FString::Printf(TEXT("UnregisterAntiCheatClient-> FOnlineSubsystemEOS is null")));
		return false;
	}
	PrintAdvancedLogs(FString::Printf(TEXT("UnregisterAntiCheatClient-> IOnlineSubsystem is null")));
	return false;
}

void UAntiCheatClient::OnMessageToServerCallback(const EOS_AntiCheatClient_OnMessageToServerCallbackInfo* Data)
{
	if(!Data->ClientData)
	{
		UE_LOG(LogTemp,Verbose, TEXT("OnMessageToServerCallback-> ClientData is null"));
		return;
	}
	if(const UAntiCheatClient* AntiCheatClient = static_cast<UAntiCheatClient*>(Data->ClientData))
	{
		TArray<uint8> MessageData;
		MessageData.Append((uint8*)Data->MessageData, Data->MessageDataSizeBytes);
		AntiCheatClient->OnSendMessageToServer.Broadcast(MessageData);
	}
	else
	{
		UE_LOG(LogTemp,Verbose, TEXT("OnMessageToClientCb-> AntiCheatServer is null"));
	}
	
}
