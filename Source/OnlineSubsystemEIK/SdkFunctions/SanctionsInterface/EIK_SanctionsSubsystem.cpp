// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_SanctionsSubsystem.h"

#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

TEnumAsByte<EEIK_Result> UEIK_SanctionsSubsystem::EIK_Sanctions_CopyPlayerSanctionByIndex(
	FEIK_ProductUserId LocalUserId, int32 Index, FEIK_Sanctions_PlayerSanction& OutSanction)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sanctions_CopyPlayerSanctionByIndexOptions Options = { };
			Options.ApiVersion = EOS_SANCTIONS_COPYPLAYERSANCTIONBYINDEX_API_LATEST;
			Options.SanctionIndex = Index;
			Options.TargetUserId = LocalUserId.GetValueAsEosType();
			EOS_Sanctions_PlayerSanction* OutSanctionPtr;
			auto Result = EOS_Sanctions_CopyPlayerSanctionByIndex(EOSRef->SanctionsHandle, &Options, &OutSanctionPtr);
			if (Result == EOS_EResult::EOS_Success)
			{
				OutSanction = *OutSanctionPtr;
				EOS_Sanctions_PlayerSanction_Release(OutSanctionPtr);
			}
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Sanctions_CopyPlayerSanctionByIndex: Failed to get EOS Platform Handle"));
	return EEIK_Result::EOS_ServiceFailure;
}

int32 UEIK_SanctionsSubsystem::EIK_Sanctions_GetPlayerSanctionCount(FEIK_ProductUserId LocalUserId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sanctions_GetPlayerSanctionCountOptions Options = { };
			Options.ApiVersion = EOS_SANCTIONS_GETPLAYERSANCTIONCOUNT_API_LATEST;
			Options.TargetUserId = LocalUserId.GetValueAsEosType();
			return EOS_Sanctions_GetPlayerSanctionCount(EOSRef->SanctionsHandle, &Options);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Sanctions_GetPlayerSanctionCount: Failed to get EOS Platform Handle"));
	return -1;
}
