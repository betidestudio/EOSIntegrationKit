// Copyright Epic Games, Inc. All Rights Reserved.


#include "EIK_GetExternalAccountsFromPUID.h"

#include "eos_connect.h"
#include "eos_connect_types.h"
#include "eos_common.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOS.h"

UEIK_GetExternalAccountsFromPUID* UEIK_GetExternalAccountsFromPUID::EIKGetExternalAccountsFromPuid(TArray<FString> TargetProductUserIds, FString LocalProductUserId)
{
	UEIK_GetExternalAccountsFromPUID* EIK_GetPUIDFromEpicId_Obj = NewObject<UEIK_GetExternalAccountsFromPUID>();

	EIK_GetPUIDFromEpicId_Obj->Var_TargetUserIds = TargetProductUserIds;
	EIK_GetPUIDFromEpicId_Obj->Var_LocalProductUserId = LocalProductUserId;

	return EIK_GetPUIDFromEpicId_Obj;
}

void UEIK_GetExternalAccountsFromPUID::GetExternalAccountsFromPuid()
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if (EOSRef->ConnectHandle != nullptr)
			{
				TArray <EOS_ProductUserId> ProductUserIdsArr;
				for (const FString& UserId : Var_TargetUserIds)
				{
					ProductUserIdsArr.Add(EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*UserId)));
				}

				EOS_Connect_QueryProductUserIdMappingsOptions QueryProductUserIdMappingsOptions = {};
				QueryProductUserIdMappingsOptions.ApiVersion = EOS_CONNECT_QUERYPRODUCTUSERIDMAPPINGS_API_LATEST;
				QueryProductUserIdMappingsOptions.LocalUserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*Var_LocalProductUserId));
				QueryProductUserIdMappingsOptions.ProductUserIds = ProductUserIdsArr.GetData();
				QueryProductUserIdMappingsOptions.ProductUserIdCount = ProductUserIdsArr.Num();

				// Call the function
				EOS_Connect_QueryProductUserIdMappings(EOSRef->ConnectHandle, &QueryProductUserIdMappingsOptions, this, OnQueryProductUserIdMappingsComplete);
			}
			else
			{
				TArray<FProductUserIdAndExternalAccountIds> EmptyProductUserIdAndEpicId;
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
			TArray<FProductUserIdAndExternalAccountIds> EmptyProductUserIdAndEpicId;
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
		TArray<FProductUserIdAndExternalAccountIds> EmptyProductUserIdAndEpicId;
		Failure.Broadcast(EmptyProductUserIdAndEpicId);
		SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		MarkAsGarbage();
#else
		MarkPendingKill();
#endif
	}
}

void UEIK_GetExternalAccountsFromPUID::OnQueryProductUserIdMappingsComplete(const EOS_Connect_QueryProductUserIdMappingsCallbackInfo* Data)
{
	if (Data->ResultCode == EOS_EResult::EOS_Success)
	{
		if (UEIK_GetExternalAccountsFromPUID* GetPUIDFromEpicId = static_cast<UEIK_GetExternalAccountsFromPUID*>(Data->ClientData))
		{
			GetPUIDFromEpicId->QueryProductUserIdMappingsSuccess();
		}
	}
	else
	{
		if (UEIK_GetExternalAccountsFromPUID* GetPUIDFromEpicId = static_cast<UEIK_GetExternalAccountsFromPUID*>(Data->ClientData))
		{
			GetPUIDFromEpicId->QueryProductUserIdMappingsFailure();
		}
	}
}

void UEIK_GetExternalAccountsFromPUID::QueryProductUserIdMappingsSuccess()
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if (EOSRef->ConnectHandle != nullptr)
			{

				TArray<FProductUserIdAndExternalAccountIds> ProductUserIdAndExternalAccountIds;

				for (const FString& UserId : Var_TargetUserIds)
				{
					FProductUserIdAndExternalAccountIds TempProductUserIdAndExternalAccountIds;

					EOS_Connect_GetProductUserExternalAccountCountOptions GetProductUserExternalAccountCountOptions = {};

					GetProductUserExternalAccountCountOptions.ApiVersion = EOS_CONNECT_GETPRODUCTUSEREXTERNALACCOUNTCOUNT_API_LATEST;
					GetProductUserExternalAccountCountOptions.TargetUserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*UserId));

					uint32_t NumAccounts = EOS_Connect_GetProductUserExternalAccountCount(EOSRef->ConnectHandle, &GetProductUserExternalAccountCountOptions);

					for (uint32_t i = 0; i < NumAccounts; ++i)
					{
						EOS_Connect_CopyProductUserExternalAccountByIndexOptions CopyProductUserExternalAccountByIndexOptions = {};

						CopyProductUserExternalAccountByIndexOptions.ApiVersion = EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYINDEX_API_LATEST;
						CopyProductUserExternalAccountByIndexOptions.TargetUserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*UserId));
						CopyProductUserExternalAccountByIndexOptions.ExternalAccountInfoIndex = i;

						EOS_Connect_ExternalAccountInfo* OutExternalAccountInfo = nullptr;

						if (EOS_Connect_CopyProductUserExternalAccountByIndex(EOSRef->ConnectHandle, &CopyProductUserExternalAccountByIndexOptions, &OutExternalAccountInfo) == EOS_EResult::EOS_Success)
						{
							FExternalAccountIdAndType TempExternalAccountIdAndType;

							TempExternalAccountIdAndType.AccountId = UTF8_TO_TCHAR(OutExternalAccountInfo->AccountId);
							TempExternalAccountIdAndType.DisplayName = UTF8_TO_TCHAR(OutExternalAccountInfo->DisplayName);
							TempExternalAccountIdAndType.LastLogin = FDateTime::FromUnixTimestamp(OutExternalAccountInfo->LastLoginTime);

							switch (OutExternalAccountInfo->AccountIdType)
							{
							case EOS_EExternalAccountType::EOS_EAT_EPIC:
								TempExternalAccountIdAndType.ExternalAccountType = ExternalAccountTypes::EOS_EAT_EPIC;
								break;
							case EOS_EExternalAccountType::EOS_EAT_STEAM:
								TempExternalAccountIdAndType.ExternalAccountType = ExternalAccountTypes::EOS_EAT_STEAM;
								break;
							case EOS_EExternalAccountType::EOS_EAT_PSN:
								TempExternalAccountIdAndType.ExternalAccountType = ExternalAccountTypes::EOS_EAT_PSN;
								break;
							case EOS_EExternalAccountType::EOS_EAT_XBL:
								TempExternalAccountIdAndType.ExternalAccountType = ExternalAccountTypes::EOS_EAT_XBL;
								break;
							case EOS_EExternalAccountType::EOS_EAT_DISCORD:
								TempExternalAccountIdAndType.ExternalAccountType = ExternalAccountTypes::EOS_EAT_DISCORD;
								break;
							case EOS_EExternalAccountType::EOS_EAT_GOG:
								TempExternalAccountIdAndType.ExternalAccountType = ExternalAccountTypes::EOS_EAT_GOG;
								break;
							case EOS_EExternalAccountType::EOS_EAT_NINTENDO:
								TempExternalAccountIdAndType.ExternalAccountType = ExternalAccountTypes::EOS_EAT_NINTENDO;
								break;
							case EOS_EExternalAccountType::EOS_EAT_UPLAY:
								TempExternalAccountIdAndType.ExternalAccountType = ExternalAccountTypes::EOS_EAT_UPLAY;
								break;
							case EOS_EExternalAccountType::EOS_EAT_OPENID:
								TempExternalAccountIdAndType.ExternalAccountType = ExternalAccountTypes::EOS_EAT_OPENID;
								break;
							case EOS_EExternalAccountType::EOS_EAT_APPLE:
								TempExternalAccountIdAndType.ExternalAccountType = ExternalAccountTypes::EOS_EAT_APPLE;
								break;
							case EOS_EExternalAccountType::EOS_EAT_GOOGLE:
								TempExternalAccountIdAndType.ExternalAccountType = ExternalAccountTypes::EOS_EAT_GOOGLE;
								break;
							case EOS_EExternalAccountType::EOS_EAT_OCULUS:
								TempExternalAccountIdAndType.ExternalAccountType = ExternalAccountTypes::EOS_EAT_OCULUS;
								break;
							case EOS_EExternalAccountType::EOS_EAT_ITCHIO:
								TempExternalAccountIdAndType.ExternalAccountType = ExternalAccountTypes::EOS_EAT_ITCHIO;
								break;
							case EOS_EExternalAccountType::EOS_EAT_AMAZON:
								TempExternalAccountIdAndType.ExternalAccountType = ExternalAccountTypes::EOS_EAT_AMAZON;
								break;
							default:
								TempExternalAccountIdAndType.ExternalAccountType = ExternalAccountTypes::EOS_EAT_EPIC;
								break;
							}

							char PUIDString[EOS_PRODUCTUSERID_MAX_LENGTH + 1]; // +1 for the null-terminator
							int32_t BufferSize = sizeof(PUIDString);
							EOS_EResult Result = EOS_ProductUserId_ToString(OutExternalAccountInfo->ProductUserId, PUIDString, &BufferSize);

							if (Result == EOS_EResult::EOS_Success)
							{
								TempProductUserIdAndExternalAccountIds.ProductUserId = (UTF8_TO_TCHAR(PUIDString));
							}
							TempProductUserIdAndExternalAccountIds.ExternalAccountIds.Add(TempExternalAccountIdAndType);

							EOS_Connect_ExternalAccountInfo_Release(OutExternalAccountInfo);
						}
						
						ProductUserIdAndExternalAccountIds.Add(TempProductUserIdAndExternalAccountIds);
					}
				}

				Success.Broadcast(ProductUserIdAndExternalAccountIds);
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

void UEIK_GetExternalAccountsFromPUID::QueryProductUserIdMappingsFailure()
{
	TArray<FProductUserIdAndExternalAccountIds> EmptyProductUserIdAndEpicId;

	Failure.Broadcast(EmptyProductUserIdAndEpicId);
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_GetExternalAccountsFromPUID::Activate()
{
	GetExternalAccountsFromPuid();
	Super::Activate();
}
