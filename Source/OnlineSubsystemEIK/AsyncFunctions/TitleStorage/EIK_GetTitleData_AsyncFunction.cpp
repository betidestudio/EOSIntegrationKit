// Copyright Epic Games, Inc. All Rights Reserved.


#include "EIK_GetTitleData_AsyncFunction.h"

#include "EIKSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineTitleFileInterface.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_GetTitleData_AsyncFunction* UEIK_GetTitleData_AsyncFunction::GetTitleData(FString FileName)
{
	UEIK_GetTitleData_AsyncFunction* BlueprintNode = NewObject<UEIK_GetTitleData_AsyncFunction>();
	BlueprintNode->FileName = FileName;
	return BlueprintNode;
}

void UEIK_GetTitleData_AsyncFunction::Activate()
{
	GetTitleData();
	Super::Activate();
}

void UEIK_GetTitleData_AsyncFunction::OnGetFileProgress(const FString& FileName1, uint64 BytesRead)
{
	OnProgress.Broadcast(true, BytesRead, TArray<uint8>());
}

void UEIK_GetTitleData_AsyncFunction::GetTitleData()
{
	if (const IOnlineSubsystem* SubsystemRef = IOnlineSubsystem::Get())
	{
		if (const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			if (const IOnlineTitleFilePtr TitleFilePointerRef = SubsystemRef->GetTitleFileInterface())
			{
				if (!IdentityPointerRef->GetUniquePlayerId(0))
				{
					if (const UEIKSettings* EIKSettings = GetMutableDefault<UEIKSettings>())
					{
						{
							UE_LOG(LogEIK, Error,TEXT("EIK Log: GetTitleData: IdentityPointerRef->GetUniquePlayerId(0) is nullptr"));
						}
					}
					if (!bDelegateCalled)
					{
						bDelegateCalled = true;
						OnFail.Broadcast(false,0, TArray<uint8>());
						SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
						MarkAsGarbage();
#else
						MarkPendingKill();
#endif
						return;
					}
				}
				TSharedPtr<const FUniqueNetId> UserIDRef = IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef();
				TitleFilePointerRef->OnReadFileCompleteDelegates.AddUObject(this, &UEIK_GetTitleData_AsyncFunction::OnGetFileComplete);
				TitleFilePointerRef->OnReadFileProgressDelegates.AddUObject(this, &UEIK_GetTitleData_AsyncFunction::OnGetFileProgress);
				TitleFilePointerRef->ReadFile(FileName);
			}
			else
			{
				if (!bDelegateCalled)
				{
					bDelegateCalled = true;
					OnFail.Broadcast(false, 0,TArray<uint8>());
					SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
					MarkAsGarbage();
#else
					MarkPendingKill();
#endif
				}
			}
		}
		else
		{
			if (!bDelegateCalled)
			{
				bDelegateCalled = true;
				OnFail.Broadcast(false, 0, TArray<uint8>());
				SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
				MarkAsGarbage();
#else
				MarkPendingKill();
#endif
			}
		}
	}
	else
	{
		if (!bDelegateCalled)
		{
			bDelegateCalled = true;
			OnFail.Broadcast(false, 0,TArray<uint8>());
			SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
			MarkAsGarbage();
#else
			MarkPendingKill();
#endif
		}
	}
}

void UEIK_GetTitleData_AsyncFunction::OnGetFileComplete(bool bSuccess, const FString& V_FileName)
{
	if (bSuccess)
	{
		if (const IOnlineSubsystem* SubsystemRef = IOnlineSubsystem::Get())
		{
			if (const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
			{
				if (const IOnlineTitleFilePtr TitleFilePointerRef = SubsystemRef->GetTitleFileInterface())
				{
					TSharedPtr<const FUniqueNetId> UserIDRef = IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef();
					TArray<uint8> FileContents;
					TitleFilePointerRef->GetFileContents(V_FileName, FileContents);
					if (FileContents.Num() > 0)
					{
						if (!bDelegateCalled)
						{
							bDelegateCalled = true;
							OnSuccess.Broadcast(true,0, FileContents);
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
						if (!bDelegateCalled)
						{
							bDelegateCalled = true;
							OnFail.Broadcast(false, 0,TArray<uint8>());
							SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
							MarkAsGarbage();
#else
							MarkPendingKill();
#endif
						}
					}
				}
				else
				{
					if (!bDelegateCalled)
					{
						bDelegateCalled = true;
						OnFail.Broadcast(false, 0,TArray<uint8>());
						SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
						MarkAsGarbage();
#else
						MarkPendingKill();
#endif
					}
				}
			}
			else
			{
				if (!bDelegateCalled)
				{
					bDelegateCalled = true;
					OnFail.Broadcast(false,0, TArray<uint8>());
					SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
					MarkAsGarbage();
#else
					MarkPendingKill();
#endif
				}
			}
		}
		else
		{
			if (!bDelegateCalled)
			{
				bDelegateCalled = true;
				OnFail.Broadcast(false, 0,TArray<uint8>());
				SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
				MarkAsGarbage();
#else
				MarkPendingKill();
#endif
			}
		}
	}
	else
	{
		if (!bDelegateCalled)
		{
			bDelegateCalled = true;
			OnFail.Broadcast(false,0, TArray<uint8>());
			SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
			MarkAsGarbage();
#else
			MarkPendingKill();
#endif
		}
	}
}
