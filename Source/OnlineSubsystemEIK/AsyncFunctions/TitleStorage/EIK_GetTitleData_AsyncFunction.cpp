//Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_GetTitleData_AsyncFunction.h"

#include "EIKSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineTitleFileInterface.h"

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

void UEIK_GetTitleData_AsyncFunction::GetTitleData()
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			if(const IOnlineTitleFilePtr TitleFilePointerRef = SubsystemRef->GetTitleFileInterface())
			{
				if(!IdentityPointerRef->GetUniquePlayerId(0))
				{
					if(const UEIKSettings* EIKSettings = GetMutableDefault<UEIKSettings>())
					{
						if (EIKSettings->bShowAdvancedLogs)
						{
							UE_LOG(LogTemp, Error, TEXT("EIK Log: GetTitleData: IdentityPointerRef->GetUniquePlayerId(0) is nullptr"));
						}
					}
					if(!bDelegateCalled)
					{
						bDelegateCalled = true;
						OnFail.Broadcast(false, TArray<uint8>());
					}
				}
				TSharedPtr<const FUniqueNetId> UserIDRef = IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef();
				TitleFilePointerRef->OnReadFileCompleteDelegates.AddUObject(this, &UEIK_GetTitleData_AsyncFunction::OnGetFileComplete);
				TitleFilePointerRef->ReadFile(FileName);
			}
			else
			{
				if(!bDelegateCalled)
				{
					bDelegateCalled = true;
					OnFail.Broadcast(false, TArray<uint8>());
					SetReadyToDestroy();
				}
			}
		}
		else
		{
			if(!bDelegateCalled)
			{
				bDelegateCalled = true;
				OnFail.Broadcast(false, TArray<uint8>());
				SetReadyToDestroy();
			}
		}
	}
	else
	{
		if(!bDelegateCalled)
		{
			bDelegateCalled = true;
			OnFail.Broadcast(false, TArray<uint8>());
			SetReadyToDestroy();
		}
	}
}

void UEIK_GetTitleData_AsyncFunction::OnGetFileComplete(bool bSuccess, const FString& V_FileName)
{
	if(bSuccess)
	{
		if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
		{
			if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
			{
				if(const IOnlineTitleFilePtr TitleFilePointerRef = SubsystemRef->GetTitleFileInterface())
				{
					TSharedPtr<const FUniqueNetId> UserIDRef = IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef();
					TArray<uint8> FileContents;
					TitleFilePointerRef->GetFileContents(V_FileName, FileContents);
					if(!FileContents.IsEmpty())
					{
						if(!bDelegateCalled)
						{
							bDelegateCalled = true;
							OnSuccess.Broadcast(true, FileContents);
							SetReadyToDestroy();
						}
					}
					else
					{
						if(!bDelegateCalled)
						{
							bDelegateCalled = true;
							OnFail.Broadcast(false, TArray<uint8>());
							SetReadyToDestroy();
						}
					}
				}
				else
				{
					if(!bDelegateCalled)
					{
						bDelegateCalled = true;
						OnFail.Broadcast(false, TArray<uint8>());
						SetReadyToDestroy();
					}
				}
			}
			else
			{
				if(!bDelegateCalled)
				{
					bDelegateCalled = true;
					OnFail.Broadcast(false, TArray<uint8>());
					SetReadyToDestroy();
				}
			}
		}
		else
		{
			if(!bDelegateCalled)
			{
				bDelegateCalled = true;
				OnFail.Broadcast(false, TArray<uint8>());
				SetReadyToDestroy();
			}
		}
	}
	else
	{
		if(!bDelegateCalled)
		{
			bDelegateCalled = true;
			OnFail.Broadcast(false, TArray<uint8>());
			SetReadyToDestroy();
		}
	}
}
