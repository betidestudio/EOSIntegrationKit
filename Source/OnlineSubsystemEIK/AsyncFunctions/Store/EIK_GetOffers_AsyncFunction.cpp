// Copyright Epic Games, Inc. All Rights Reserved.


#include "EIK_GetOffers_AsyncFunction.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineStoreInterfaceV2.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"

void UEIK_GetOffers_AsyncFunction::Activate()
{
	GetOffers();
	Super::Activate();
}

void UEIK_GetOffers_AsyncFunction::GetOffers()
{
	if (const IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if (const IOnlineStoreV2Ptr StoreV2Ptr = SubsystemRef->GetStoreV2Interface())
		{
			if (const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
			{
				const FUniqueNetIdPtr UserIdPtr{IdentityPointerRef->GetUniquePlayerId(0)};
				if (UserIdPtr)
				{
					StoreV2Ptr->QueryOffersByFilter(*UserIdPtr.Get(), FOnlineStoreFilter(),FOnQueryOnlineStoreOffersComplete::CreateLambda([StoreV2Wk = TWeakPtr<IOnlineStoreV2, ESPMode::ThreadSafe>(StoreV2Ptr), this](bool bWasSuccessful, const TArray<FUniqueOfferId>& OfferIds, const FString& Error)
					{
						UE_LOG(LogTemp, Warning, TEXT("QueryOffersByFilter Callback Called and if there is a error, it is: %s"), *Error);
						if (const auto StoreV2 = StoreV2Wk.Pin())
						{
							if (bWasSuccessful && StoreV2.IsValid())
							{
								TArray<FOnlineStoreOfferRef> Offers;
								StoreV2->GetOffers(Offers);
								UE_LOG(LogTemp, Warning, TEXT("Offers Array Size: %d"), Offers.Num());
								TArray<FOffersStruct> OfferArray;
								for (int32 i = 0; i < Offers.Num(); ++i)
								{
									FOffersStruct NewOfferStruct;
									NewOfferStruct.ItemID = Offers[i]->OfferId;
									NewOfferStruct.ItemName = Offers[i]->Title;
									NewOfferStruct.Description = Offers[i]->
										Description;
									NewOfferStruct.ExpirationDate = Offers[i]->
										ExpirationDate;
									NewOfferStruct.LongDescription = Offers[i]->
										LongDescription;
									NewOfferStruct.NumericPrice = Offers[i]->
										NumericPrice;
									NewOfferStruct.PriceText = Offers[i]->PriceText;
									NewOfferStruct.RegularPrice = Offers[i]->
										RegularPrice;
									NewOfferStruct.ReleaseDate = Offers[i]->
										ReleaseDate;
									NewOfferStruct.RegularPriceText = Offers[i]->
										RegularPriceText;
									OfferArray.Add(NewOfferStruct);
								}
								if (!bDelegateCalled)
								{
									OnSuccess.Broadcast(OfferArray);
									bDelegateCalled = true;
								}
							}
							else
							{
								if (!bDelegateCalled)
								{
									OnFail.Broadcast(TArray<FOffersStruct>());
									bDelegateCalled = true;
								}
							}
						}
						else
						{
							if (!bDelegateCalled)
							{
								OnFail.Broadcast(TArray<FOffersStruct>());
								bDelegateCalled = true;
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
				if (!bDelegateCalled)
				{
					OnFail.Broadcast(TArray<FOffersStruct>());
					bDelegateCalled = true;
				}
			}
		}
		else
		{
			if (!bDelegateCalled)
			{
				OnFail.Broadcast(TArray<FOffersStruct>());
				bDelegateCalled = true;
			}
		}
	}
	else
	{
		if (!bDelegateCalled)
		{
			OnFail.Broadcast(TArray<FOffersStruct>());
			bDelegateCalled = true;
		}
	}
}

UEIK_GetOffers_AsyncFunction* UEIK_GetOffers_AsyncFunction::GetEIKOffers()
{
	UEIK_GetOffers_AsyncFunction* BlueprintNode = NewObject<UEIK_GetOffers_AsyncFunction>();
	return BlueprintNode;
}
