// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_VerifyOwnershipVerificationToken.h"

UEIK_API_VerifyOwnershipVerificationToken* UEIK_API_VerifyOwnershipVerificationToken::
VerifyOwnershipVerificationToken(FString KeyId)
{
	UEIK_API_VerifyOwnershipVerificationToken* Node = NewObject<UEIK_API_VerifyOwnershipVerificationToken>();
	Node->Var_KeyId = KeyId;
	return Node;
}

void UEIK_API_VerifyOwnershipVerificationToken::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("https://ecommerceintegration-public-service-ecomprod02.ol.epicgames.com/ecommerceintegration/api/public/publickeys/%s"), *Var_KeyId);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetURL(URL);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_VerifyOwnershipVerificationToken::OnResponseReceived);
	HttpRequest->ProcessRequest();
}
