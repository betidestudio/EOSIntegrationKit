// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_P2P_QueryNATType.h"

#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_P2P_QueryNATType* UEIK_P2P_QueryNATType::EIK_P2P_QueryNATType()
{
	UEIK_P2P_QueryNATType* BlueprintNode = NewObject<UEIK_P2P_QueryNATType>();
	return BlueprintNode;
}

void UEIK_P2P_QueryNATType::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_P2P_QueryNATTypeOptions Options = {};
			Options.ApiVersion = EOS_P2P_QUERYNATTYPE_API_LATEST;
			EOS_P2P_QueryNATType(EOSRef->SocketSubsystem->GetP2PHandle(), &Options, this, &UEIK_P2P_QueryNATType::EOS_P2P_QueryNATType_Callback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_P2P_QueryNATType::Activate: Unable to get EOS subsystem."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, EEIK_ENATType::EIK_NAT_Unknown);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_P2P_QueryNATType::EOS_P2P_QueryNATType_Callback(const EOS_P2P_OnQueryNATTypeCompleteInfo* Data)
{
	if(UEIK_P2P_QueryNATType* ThisNode = static_cast<UEIK_P2P_QueryNATType*>(Data->ClientData))
	{
		ThisNode->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), static_cast<EEIK_ENATType>(Data->NATType));
		ThisNode->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		ThisNode->MarkAsGarbage();
#else
		ThisNode->MarkPendingKill();
#endif
	}
}
