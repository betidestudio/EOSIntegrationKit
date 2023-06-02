//Copyright (c) 2023 Betide Studio. All Rights Reserved.


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
				StoreV2Ptr->QueryOffersByFilter(*IdentityPointerRef->GetUniquePlayerId(0).Get(), FOnlineStoreFilter(),
												FOnQueryOnlineStoreOffersComplete::CreateLambda([
														StoreV2Wk = TWeakPtr<IOnlineStoreV2, ESPMode::ThreadSafe>(
															StoreV2Ptr), this](
													bool bWasSuccessful,
													const TArray<FUniqueOfferId>& OfferIds,
													const FString& Error)
												{
													if (const auto StoreV2 = StoreV2Wk.Pin())
													{
														if (bWasSuccessful && StoreV2.IsValid())
														{
															TArray<FOnlineStoreOfferRef> Offers;
															StoreV2->GetOffers(Offers);
															TArray<FOffersStruct> OfferArray;
															for (int32 i = 0; i < Offers.Num(); ++i)
															{
																OfferArray[i].ItemID = Offers[i]->OfferId;
																OfferArray[i].ItemName = Offers[i]->Title;
																OfferArray[i].Description = Offers[i]->Description;
																OfferArray[i].ExpirationDate = Offers[i]->
																	ExpirationDate;
																OfferArray[i].LongDescription = Offers[i]->
																	LongDescription;
																OfferArray[i].NumericPrice = Offers[i]->
																	NumericPrice;
																OfferArray[i].PriceText = Offers[i]->PriceText;
																OfferArray[i].RegularPrice = Offers[i]->
																	RegularPrice;
																OfferArray[i].ReleaseDate = Offers[i]->ReleaseDate;
																OfferArray[i].RegularPriceText = Offers[i]->
																	RegularPriceText;
															}
															if(!bDelegateCalled)
															{
																OnSuccess.Broadcast(OfferArray);
																bDelegateCalled = true;
															}
														}
														else
														{
															if(!bDelegateCalled)
															{
																OnFail.Broadcast(TArray<FOffersStruct>());
																bDelegateCalled = true;
															}
														}
													}
													else
													{
														if(!bDelegateCalled)
														{
															OnFail.Broadcast(TArray<FOffersStruct>());
															bDelegateCalled = true;
														}
													}
												}));
			}
			else
			{
				if(!bDelegateCalled)
				{
					OnFail.Broadcast(TArray<FOffersStruct>());
					bDelegateCalled = true;
				}			}
		}
		else
		{
			if(!bDelegateCalled)
			{
				OnFail.Broadcast(TArray<FOffersStruct>());
				bDelegateCalled = true;
			}
		}
	}
	else
	{
		if(!bDelegateCalled)
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
