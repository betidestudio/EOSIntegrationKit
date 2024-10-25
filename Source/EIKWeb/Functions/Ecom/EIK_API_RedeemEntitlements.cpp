// Copyright (c) 2024 Betide Studio. All Rights Reserved.


#include "EIK_API_RedeemEntitlements.h"

UEIK_API_RedeemEntitlements* UEIK_API_RedeemEntitlements::RedeemEntitlements(FString Authorization, FString IdentityId,
	TArray<FString> EntitlementIds, FString SandboxId)
{
	UEIK_API_RedeemEntitlements* Node = NewObject<UEIK_API_RedeemEntitlements>();
	Node->Var_Authorization = Authorization;
	Node->Var_IdentityId = IdentityId;
	Node->Var_EntitlementIds = EntitlementIds;
	Node->Var_SandboxId = SandboxId;
	return Node;
}

void UEIK_API_RedeemEntitlements::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/epic/ecom/v3/identities/%s/entitlements/redeem"), *APIEndpoint, *Var_IdentityId);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("PUT"));
	HttpRequest->SetURL(URL);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	if (Var_Authorization.Contains("Bearer"))
	{
		HttpRequest->SetHeader(TEXT("Authorization"), Var_Authorization);
	}
	else
	{
		HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *Var_Authorization));
	}
	//Make Json Body
	TSharedPtr<FJsonObject> RequestBodyJson = MakeShareable(new FJsonObject);
	if(!Var_SandboxId.IsEmpty())
	{
		RequestBodyJson->SetStringField(TEXT("sandboxId"), Var_SandboxId);
	}
	TArray<TSharedPtr<FJsonValue>> EntitlementIdsJson;
	if (Var_EntitlementIds.Num() > 0)
	{
		for (auto EntitlementId : Var_EntitlementIds)
		{
			TSharedPtr<FJsonValue> EntitlementIdJson = MakeShareable(new FJsonValueString(EntitlementId));
			EntitlementIdsJson.Add(EntitlementIdJson);
		}
	}
	RequestBodyJson->SetArrayField(TEXT("entitlementIds"), EntitlementIdsJson);
	FString RequestBodyString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&RequestBodyString);
	FJsonSerializer::Serialize(RequestBodyJson.ToSharedRef(), JsonWriter);
	HttpRequest->SetContentAsString(RequestBodyString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_RedeemEntitlements::OnResponseReceived);
	HttpRequest->ProcessRequest();
}
