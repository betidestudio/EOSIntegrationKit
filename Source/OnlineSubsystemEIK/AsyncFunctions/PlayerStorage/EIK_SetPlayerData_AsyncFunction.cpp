//Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_SetPlayerData_AsyncFunction.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineUserCloudInterface.h"
#include "Kismet/GameplayStatics.h"

UEIK_SetPlayerData_AsyncFunction* UEIK_SetPlayerData_AsyncFunction::SetPlayerData(FString FileName,
                                                                                  USaveGame* SavedGame)
{
	UEIK_SetPlayerData_AsyncFunction* BlueprintNode = NewObject<UEIK_SetPlayerData_AsyncFunction>();
	BlueprintNode->FileName = FileName;
	BlueprintNode->SavedGame = SavedGame;
	return BlueprintNode;
}

void UEIK_SetPlayerData_AsyncFunction::Activate()
{
	SetPlayerData();
	Super::Activate();
}

void UEIK_SetPlayerData_AsyncFunction::SetPlayerData()
{
	if(SavedGame)
	{
		TArray<uint8> LocalArray;
		UGameplayStatics::SaveGameToMemory(SavedGame,LocalArray);
		if(!LocalArray.IsEmpty())
		{
			if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get() )
			{
				if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
				{
					if(const IOnlineUserCloudPtr CloudPointerRef = SubsystemRef->GetUserCloudInterface())
					{
						const TSharedPtr<const FUniqueNetId> UserIDRef = IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef();
						CloudPointerRef->OnWriteUserFileCompleteDelegates.AddUObject(this, &UEIK_SetPlayerData_AsyncFunction::OnWriteFileComplete);
						CloudPointerRef->WriteUserFile(*UserIDRef,FileName,LocalArray);
					}
					else
					{
						if(!bDelegateCalled)
						{
							bDelegateCalled = true;
							OnFail.Broadcast();
						}
					}
				}
				else
				{
					if(!bDelegateCalled)
					{
						bDelegateCalled = true;
						OnFail.Broadcast();
					}
				}
			}
			else
			{
				if(!bDelegateCalled)
				{
					bDelegateCalled = true;
					OnFail.Broadcast();
				}
			}
		}
		else
		{
			if(!bDelegateCalled)
			{
				bDelegateCalled = true;
				OnFail.Broadcast();
			}
		}
	}
	else
	{
		if(!bDelegateCalled)
		{
			bDelegateCalled = true;
			OnFail.Broadcast();
		}
	}
}

void UEIK_SetPlayerData_AsyncFunction::OnWriteFileComplete(bool bSuccess, const FUniqueNetId& UserID,
	const FString& Var_FileName)
{
	if(bSuccess)
	{
		if(!bDelegateCalled)
		{
			bDelegateCalled = true;
			OnSuccess.Broadcast();
		}
	}
	else
	{
		if(!bDelegateCalled)
		{
			bDelegateCalled = true;
			OnFail.Broadcast();
		}
	}
}
