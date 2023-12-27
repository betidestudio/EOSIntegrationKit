// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "AntiCheatServer.h"

#include "EIKSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOS.h"

bool UAntiCheatServer::IsAntiCheatServerAvailable(const UObject* WorldContextObject)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(EOSRef->AntiCheatServerHandle)
			{
				return true;
			}
		}
	}
	return false;
}

void UAntiCheatServer::PrintAdvancedLogs(FString Log)
{
	if(UEIKSettings* EIKSettings = GetMutableDefault<UEIKSettings>())
	{
		if(EIKSettings->bShowAdvancedLogs)
		{
			UE_LOG(LogOnline,Warning, TEXT("EIK: %s"), *Log);
		}
	}
}
bool UAntiCheatServer::RegisterAntiCheatServer(FString ClientProductID)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			{
				EOS_AntiCheatServer_AddNotifyMessageToClientOptions Options = {};
				Options.ApiVersion = EOS_ANTICHEATSERVER_ADDNOTIFYMESSAGETOCLIENT_API_LATEST;
				MessageToClientId = EOS_AntiCheatServer_AddNotifyMessageToClient(EOSRef->AntiCheatServerHandle, &Options, this, OnMessageToClientCb);
			}

			{
				EOS_AntiCheatServer_AddNotifyClientActionRequiredOptions Options = {};
				Options.ApiVersion = EOS_ANTICHEATSERVER_ADDNOTIFYCLIENTACTIONREQUIRED_API_LATEST;
				ClientActionRequiredId = EOS_AntiCheatServer_AddNotifyClientActionRequired(EOSRef->AntiCheatServerHandle, &Options, this, OnClientActionRequiredCb);
			}
			{
				EOS_AntiCheatServer_BeginSessionOptions Options = {};
				Options.ApiVersion = EOS_ANTICHEATSERVER_BEGINSESSION_API_LATEST;
				Options.RegisterTimeoutSeconds = EOS_ANTICHEATSERVER_BEGINSESSION_MAX_REGISTERTIMEOUT;
				const FString LocalServerName = "GameSession";
				Options.ServerName = TCHAR_TO_UTF8(*LocalServerName);
				Options.bEnableGameplayData = EOS_FALSE;
#if UE_SERVER
				Options.LocalUserId = nullptr;
#else
				Options.LocalUserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*ClientProductID));
#endif
				const EOS_EResult Result = EOS_AntiCheatServer_BeginSession(EOSRef->AntiCheatServerHandle, &Options);
				if(Result == EOS_EResult::EOS_Success)
				{
					return true;
				}
				PrintAdvancedLogs(FString::Printf(TEXT("RegisterAntiCheatServer-> %hs"), EOS_EResult_ToString(Result)));
				return false;
			}
		}
		PrintAdvancedLogs(FString::Printf(TEXT("RegisterAntiCheatServer-> FOnlineSubsystemEOS is null")));
		return false;
	}
	PrintAdvancedLogs(FString::Printf(TEXT("RegisterAntiCheatServer-> IOnlineSubsystem is null")));
	return false;
}

bool UAntiCheatServer::RegisterClientForAntiCheat(FString ClientProductID, APlayerController* ControllerRef, TEnumAsByte<EUserPlatform> UserPlatform, TEnumAsByte<EEOS_ClientType> ClientType)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_AntiCheatServer_RegisterClientOptions Options = {};
			Options.ApiVersion = EOS_ANTICHEATSERVER_REGISTERCLIENT_API_LATEST;
			Options.ClientHandle = ControllerRef;
			Options.ClientType = static_cast<EOS_EAntiCheatCommonClientType>(ClientType.GetValue());
			Options.ClientPlatform = static_cast<EOS_EAntiCheatCommonClientPlatform>(UserPlatform.GetValue());
			Options.UserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*ClientProductID));
			const EOS_EResult Result = EOS_AntiCheatServer_RegisterClient(EOSRef->AntiCheatServerHandle, &Options);
			if(Result == EOS_EResult::EOS_Success)
			{
				return true;
			}
			PrintAdvancedLogs(FString::Printf(TEXT("RegisterClientForAntiCheat-> %hs"), EOS_EResult_ToString(Result)));
			return false;
		}
		PrintAdvancedLogs(FString::Printf(TEXT("RegisterClientForAntiCheat-> FOnlineSubsystemEOS is null")));
		return false;
	}
	PrintAdvancedLogs(FString::Printf(TEXT("RegisterClientForAntiCheat-> IOnlineSubsystem is null")));
	return false;
}

bool UAntiCheatServer::RecievedMessageFromClient(APlayerController* Controller,const TArray<uint8>& Message)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_AntiCheatServer_ReceiveMessageFromClientOptions Options = {};
			Options.ApiVersion = EOS_ANTICHEATSERVER_RECEIVEMESSAGEFROMCLIENT_API_LATEST;
			Options.Data = Message.GetData();
			Options.DataLengthBytes = Message.Num();
			Options.ClientHandle = Controller;
			const EOS_EResult Result = EOS_AntiCheatServer_ReceiveMessageFromClient(EOSRef->AntiCheatServerHandle, &Options);
			if(Result == EOS_EResult::EOS_Success)
			{
				return true;
			}
			PrintAdvancedLogs(FString::Printf(TEXT("RecievedMessageFromClient-> %hs"), EOS_EResult_ToString(Result)));
			return false;
		}
		PrintAdvancedLogs(FString::Printf(TEXT("RecievedMessageFromClient-> FOnlineSubsystemEOS is null")));
		return false;
	}
	PrintAdvancedLogs(FString::Printf(TEXT("RecievedMessageFromClient-> IOnlineSubsystem is null")));
	return false;
}

void UAntiCheatServer::OnMessageToClientCb(const EOS_AntiCheatCommon_OnMessageToClientCallbackInfo* Data)
{
	TArray<uint8> MessageData;
	MessageData.Append((uint8*)Data->MessageData, Data->MessageDataSizeBytes);
	if(const UAntiCheatServer* AntiCheatServer = static_cast<UAntiCheatServer*>(Data->ClientData))
	{
		AntiCheatServer->OnAntiCheatRegisterClient.Broadcast(static_cast<APlayerController*>(Data->ClientHandle), MessageData);
	}
	else
	{
		UE_LOG(LogTemp,Verbose, TEXT("OnMessageToClientCb-> AntiCheatServer is null"));
	}
}

void UAntiCheatServer::OnClientActionRequiredCb(const EOS_AntiCheatCommon_OnClientActionRequiredCallbackInfo* Data)
{
	if(const UAntiCheatServer* AntiCheatServer = static_cast<UAntiCheatServer*>(Data->ClientData))
	{
		AntiCheatServer->OnAntiCheatActionRequired.Broadcast(static_cast<APlayerController*>(Data->ClientHandle), Data->ClientAction == EOS_EAntiCheatCommonClientAction::EOS_ACCCA_RemovePlayer);
	}
	else
	{
		UE_LOG(LogTemp,Verbose, TEXT("OnClientActionRequiredCb-> AntiCheatServer is null"));
	}
}
