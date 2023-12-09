//Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_OwnedItems_AsyncFunction.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlinePurchaseInterface.h"

void UEIK_OwnedItems_AsyncFunction::Activate()
{
	GetOwnedItems();
	Super::Activate();
}

void UEIK_OwnedItems_AsyncFunction::GetOwnedItems()
{
	if (const IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if (const IOnlineStoreV2Ptr StoreV2Ptr = SubsystemRef->GetStoreV2Interface())
		{
			if (const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
			{
				if (const IOnlinePurchasePtr Purchase = SubsystemRef->GetPurchaseInterface())
				{
					const FUniqueNetIdPtr UserIdPtr{ IdentityPointerRef->GetUniquePlayerId(0) };
					if (UserIdPtr)
					{
						Purchase->QueryReceipts(*UserIdPtr.Get(), false,
							FOnQueryReceiptsComplete::CreateLambda(
								[this, SubsystemRef, IdentityPointerRef, Purchase
								](const FOnlineError& Error)
								{
									if (Error.WasSuccessful())
									{
										if (Purchase)
										{
											TArray<FString> ItemNames;
											TArray<FPurchaseReceipt> Receipts;
											Purchase->GetReceipts(
												*IdentityPointerRef->GetUniquePlayerId(0).Get(), Receipts);
											for (int i = 0; i < Receipts.Num(); i++)
											{
												ItemNames.Add(Receipts[i].ReceiptOffers[0].LineItems[0].ItemName);
											}
											if (!bDelegateCalled)
											{
												bDelegateCalled = true;
												OnSuccess.Broadcast(ItemNames);
											}
										}
										else
										{
											if (!bDelegateCalled)
											{
												bDelegateCalled = true;
												OnFail.Broadcast(TArray<FString>());
											}
										}
									}
								}));
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Invalid User ID"));
					}
				}
				else
				{
					if(!bDelegateCalled)
					{
						bDelegateCalled = true;
						OnFail.Broadcast(TArray<FString>());
					}
				}
			}
			else
			{
				if(!bDelegateCalled)
				{
					bDelegateCalled = true;
					OnFail.Broadcast(TArray<FString>());
				}
			}
		}
		else
		{
			if(!bDelegateCalled)
			{
				bDelegateCalled = true;
				OnFail.Broadcast(TArray<FString>());
			}
		}
	}
	else
	{
		if(!bDelegateCalled)
		{
			bDelegateCalled = true;
			OnFail.Broadcast(TArray<FString>());
		}
	}
}

UEIK_OwnedItems_AsyncFunction* UEIK_OwnedItems_AsyncFunction::GetEIKOwnedItems()
{
	UEIK_OwnedItems_AsyncFunction* BlueprintNode = NewObject<UEIK_OwnedItems_AsyncFunction>();
	return BlueprintNode;
}
