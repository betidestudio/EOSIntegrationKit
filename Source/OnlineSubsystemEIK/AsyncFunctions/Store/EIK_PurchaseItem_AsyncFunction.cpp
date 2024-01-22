// Copyright Epic Games, Inc. All Rights Reserved.


#include "EIK_PurchaseItem_AsyncFunction.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlinePurchaseInterface.h"

UEIK_PurchaseItem_AsyncFunction* UEIK_PurchaseItem_AsyncFunction::PurchaseItem(FString ItemID)
{
	UEIK_PurchaseItem_AsyncFunction* BlueprintNode = NewObject<UEIK_PurchaseItem_AsyncFunction>();
	BlueprintNode->ItemID = ItemID;
	return BlueprintNode;
}

void UEIK_PurchaseItem_AsyncFunction::Activate()
{
	PurchaseItem();
	Super::Activate();
}

void UEIK_PurchaseItem_AsyncFunction::PurchaseItem()
{
	if (const IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if (const IOnlineStoreV2Ptr StoreV2Ptr = SubsystemRef->GetStoreV2Interface())
		{
			if (const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
			{
				if (const IOnlinePurchasePtr Purchase = SubsystemRef->GetPurchaseInterface())
				{
					FPurchaseCheckoutRequest Request = {};
					Request.AddPurchaseOffer(TEXT(""), ItemID, 1);

					Purchase->Checkout(*IdentityPointerRef->GetUniquePlayerId(0).Get(),
						Request,
						FOnPurchaseCheckoutComplete::CreateLambda(
							[this](
							const FOnlineError& Result,
							const TSharedRef<FPurchaseReceipt>& Receipt)
							{
								if (Result.WasSuccessful())
								{
									if(!bDelegateCalled)
									{
										OnSuccess.Broadcast();
										bDelegateCalled = true;
									}
								}
								else
								{
									if(!bDelegateCalled)
									{
										OnFail.Broadcast();
										bDelegateCalled = true;
									}
								}
							})
					);
				}
				else
				{
					if(!bDelegateCalled)
					{
						OnFail.Broadcast();
						bDelegateCalled = true;
					}
				}
			}
			else
			{
				if(!bDelegateCalled)
				{
					OnFail.Broadcast();
					bDelegateCalled = true;
				}
			}
		}
		else
		{
			if(!bDelegateCalled)
			{
				OnFail.Broadcast();
				bDelegateCalled = true;
			}
		}
	}
	else
	{
		if(!bDelegateCalled)
		{
			OnFail.Broadcast();
			bDelegateCalled = true;
		}
	}
}
