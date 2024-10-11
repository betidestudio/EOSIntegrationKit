// Copyright Epic Games, Inc. All Rights Reserved.


#include "EIK_GetPUIDFromEpicId_AsyncFunc.h"

#include "eos_connect.h"
#include "eos_connect_types.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOS.h"

UEIK_GetPUIDFromEpicId_AsyncFunc* UEIK_GetPUIDFromEpicId_AsyncFunc::EIKGetPUIDFromEpicId(TArray<FString> TargetEpicAccountIds, FString LocalProductUserId)
{
	UEIK_GetPUIDFromEpicId_AsyncFunc* EIK_GetPUIDFromEpicId_Obj = NewObject<UEIK_GetPUIDFromEpicId_AsyncFunc>();

	EIK_GetPUIDFromEpicId_Obj->Var_TargetUserIds = TargetEpicAccountIds;
	EIK_GetPUIDFromEpicId_Obj->Var_LocalProductUserId = LocalProductUserId;

	return EIK_GetPUIDFromEpicId_Obj;
}

void UEIK_GetPUIDFromEpicId_AsyncFunc::GetPUIDFromEpicId()
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if (EOSRef->ConnectHandle != nullptr)
			{

				TArray <const char*> ExternalAccountIdsArr;
				for (const FString& UserId : Var_TargetUserIds)
				{
					ExternalAccountIdsArr.Add(TCHAR_TO_UTF8(*UserId));
				}

				EOS_Connect_QueryExternalAccountMappingsOptions QueryExternalAccountMappingsOptions = {};
				QueryExternalAccountMappingsOptions.ApiVersion = EOS_CONNECT_QUERYEXTERNALACCOUNTMAPPINGS_API_LATEST;
				QueryExternalAccountMappingsOptions.LocalUserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*Var_LocalProductUserId));
				QueryExternalAccountMappingsOptions.AccountIdType = EOS_EExternalAccountType::EOS_EAT_EPIC;
				QueryExternalAccountMappingsOptions.ExternalAccountIds = ExternalAccountIdsArr.GetData();
				QueryExternalAccountMappingsOptions.ExternalAccountIdCount = ExternalAccountIdsArr.Num();

				// Call the function
				EOS_Connect_QueryExternalAccountMappings(EOSRef->ConnectHandle, &QueryExternalAccountMappingsOptions, this, OnQueryExternalAccountMappingsComplete);
			}
			else
			{
				TArray<FProductUserIdAndEpicId> EmptyProductUserIdAndEpicId;
				Failure.Broadcast(EmptyProductUserIdAndEpicId);
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
			TArray<FProductUserIdAndEpicId> EmptyProductUserIdAndEpicId;
			Failure.Broadcast(EmptyProductUserIdAndEpicId);
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
		TArray<FProductUserIdAndEpicId> EmptyProductUserIdAndEpicId;
		Failure.Broadcast(EmptyProductUserIdAndEpicId);
		SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		MarkAsGarbage();
#else
		MarkPendingKill();
#endif
	}
}

void UEIK_GetPUIDFromEpicId_AsyncFunc::OnQueryExternalAccountMappingsComplete(const EOS_Connect_QueryExternalAccountMappingsCallbackInfo* Data)
{
	if (Data->ResultCode == EOS_EResult::EOS_Success)
	{
		if (UEIK_GetPUIDFromEpicId_AsyncFunc* GetPUIDFromEpicId = static_cast<UEIK_GetPUIDFromEpicId_AsyncFunc*>(Data->ClientData))
		{
			GetPUIDFromEpicId->QueryExternalAccountMappingsSuccess();
		}
	}
	else
	{
		if (UEIK_GetPUIDFromEpicId_AsyncFunc* GetPUIDFromEpicId = static_cast<UEIK_GetPUIDFromEpicId_AsyncFunc*>(Data->ClientData))
		{
			GetPUIDFromEpicId->QueryExternalAccountMappingsFailure();
		}
	}
}

void UEIK_GetPUIDFromEpicId_AsyncFunc::QueryExternalAccountMappingsSuccess()
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if (EOSRef->ConnectHandle != nullptr)
			{
				TArray<FProductUserIdAndEpicId> ProductUserIdStrings;

				for (const FString& UserId : Var_TargetUserIds)
				{
					EOS_Connect_GetExternalAccountMappingsOptions GetExternalAccountMappingsOptions = {};
					GetExternalAccountMappingsOptions.ApiVersion = EOS_CONNECT_GETEXTERNALACCOUNTMAPPING_API_LATEST;
					GetExternalAccountMappingsOptions.AccountIdType = EOS_EExternalAccountType::EOS_EAT_EPIC;
					GetExternalAccountMappingsOptions.LocalUserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*Var_LocalProductUserId));
					GetExternalAccountMappingsOptions.TargetExternalUserId = TCHAR_TO_UTF8(*UserId); 

					EOS_ProductUserId PUID = EOS_Connect_GetExternalAccountMapping(EOSRef->ConnectHandle, &GetExternalAccountMappingsOptions);

					char PUIDString[EOS_PRODUCTUSERID_MAX_LENGTH + 1]; // +1 for the null-terminator
					int32_t BufferSize = sizeof(PUIDString);
					EOS_EResult Result = EOS_ProductUserId_ToString(PUID, PUIDString, &BufferSize);

					if (Result == EOS_EResult::EOS_Success)
					{
						FProductUserIdAndEpicId Var;

						Var.EpicAccountId = UserId;
						Var.ProductUserId = (UTF8_TO_TCHAR(PUIDString));

						ProductUserIdStrings.Add(Var);
					}
					else
					{
						continue;

					}
				}

				Success.Broadcast(ProductUserIdStrings);
				SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
				MarkAsGarbage();
#else
				MarkPendingKill();
#endif

			}
		}
	}
}

void UEIK_GetPUIDFromEpicId_AsyncFunc::QueryExternalAccountMappingsFailure()
{
	TArray<FProductUserIdAndEpicId> EmptyProductUserIdAndEpicId;

	Failure.Broadcast(EmptyProductUserIdAndEpicId);
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_GetPUIDFromEpicId_AsyncFunc::Activate()
{
	GetPUIDFromEpicId();
	Super::Activate();
}

