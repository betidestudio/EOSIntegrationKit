// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_CreateDeviceId_AsyncFunction.h"

#include "EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"

UEIK_CreateDeviceId_AsyncFunction* UEIK_CreateDeviceId_AsyncFunction::CreateDeviceId(FString DeviceModel)
{
	UEIK_CreateDeviceId_AsyncFunction* Node = NewObject<UEIK_CreateDeviceId_AsyncFunction>();
	Node->Var_DeviceModel = DeviceModel;
	return Node;
}

void UEIK_CreateDeviceId_AsyncFunction::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_CreateDeviceIdOptions CreateDeviceIdOptions = { };
			CreateDeviceIdOptions.ApiVersion = EOS_CONNECT_CREATEDEVICEID_API_LATEST;
			if(Var_DeviceModel.IsEmpty())
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create device id, DeviceModel is empty."));
				OnCallback.Broadcast(EEIK_Result::EOS_InvalidCredentials);
				SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
				return;
			}
			CreateDeviceIdOptions.DeviceModel = TCHAR_TO_ANSI(*Var_DeviceModel);
			EOS_Connect_CreateDeviceId(EOSRef->ConnectHandle, &CreateDeviceIdOptions, this, &UEIK_CreateDeviceId_AsyncFunction::OnCreateDeviceIdCallback);
			return;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Failed to create device id either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotConfigured);
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_CreateDeviceId_AsyncFunction::OnCreateDeviceIdCallback(const EOS_Connect_CreateDeviceIdCallbackInfo* Data)
{
	UEIK_CreateDeviceId_AsyncFunction* Node = static_cast<UEIK_CreateDeviceId_AsyncFunction*>(Data->ClientData);
	if(Node)
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode));
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}
