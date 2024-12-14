// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_CreateSanctions.h"

#include "JsonObjectConverter.h"

UEIK_API_CreateSanctions* UEIK_API_CreateSanctions::CreateSanctions(FString Authorization, FString DeploymentId,
                                                                    TArray<FEIK_SanctionPostPayload> Sanctions)
{
	UEIK_API_CreateSanctions* Proxy = NewObject<UEIK_API_CreateSanctions>();
	Proxy->Var_Authorization = Authorization;
	Proxy->Var_DeploymentId = DeploymentId;
	Proxy->Var_Sanctions = Sanctions;
	return Proxy;
}

void UEIK_API_CreateSanctions::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/sanctions/v1/%s/sanctions"), *APIEndpoint, *Var_DeploymentId);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("POST"));
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
	TArray<TSharedPtr<FJsonValue>> SanctionsJson;
	TSharedPtr<FJsonObject> RequestBodyJson = MakeShareable(new FJsonObject);
	if(Var_Sanctions.Num() > 0)
	{
		for (auto Sanction : Var_Sanctions)
		{
			TSharedPtr<FJsonObject> SanctionJson = MakeShareable(new FJsonObject);
			if(!Sanction.ProductUserId.IsEmpty())
			{
				SanctionJson->SetStringField(TEXT("productUserId"), Sanction.ProductUserId);
			}
			if(!Sanction.Action.IsEmpty())
			{
				SanctionJson->SetStringField(TEXT("action"), Sanction.Action);
			}
			if(!Sanction.Justification.IsEmpty())
			{
				SanctionJson->SetStringField(TEXT("justification"), Sanction.Justification);
			}
			if(!Sanction.Source.IsEmpty())
			{
				SanctionJson->SetStringField(TEXT("source"), Sanction.Source);
			}
			if(Sanction.Tags.Num() > 0)
			{
				TArray<TSharedPtr<FJsonValue>> TagsJson;
				for (auto Tag : Sanction.Tags)
				{
					TagsJson.Add(MakeShareable(new FJsonValueString(Tag)));
				}
				SanctionJson->SetArrayField(TEXT("tags"), TagsJson);
			}
			SanctionJson->SetBoolField(TEXT("pending"), Sanction.bPending);
			if(Sanction.Metadata.Num() > 0)
			{
				//Arbitrary metadata key/value pairs associated with this sanction
				TSharedPtr<FJsonObject> MetadataJson = MakeShareable(new FJsonObject);
				for (auto Meta : Sanction.Metadata)
				{
					MetadataJson->SetStringField(Meta.Key, Meta.Value);
				}
				SanctionJson->SetObjectField(TEXT("metadata"), MetadataJson);
			}
			if(!Sanction.DisplayName.IsEmpty())
			{
				SanctionJson->SetStringField(TEXT("displayName"), Sanction.DisplayName);
			}
			if(!Sanction.IdentityProvider.IsEmpty())
			{
				SanctionJson->SetStringField(TEXT("identityProvider"), Sanction.IdentityProvider);
			}
			if(!Sanction.AccountId.IsEmpty())
			{
				SanctionJson->SetStringField(TEXT("accountId"), Sanction.AccountId);
			}
			SanctionJson->SetNumberField(TEXT("duration"), Sanction.Duration);
			SanctionsJson.Add(MakeShareable(new FJsonValueObject(SanctionJson)));
		}
	}
	FString RequestBody;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(SanctionsJson, JsonWriter);
	HttpRequest->SetContentAsString(RequestBody);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_CreateSanctions::OnResponseReceived);
	HttpRequest->ProcessRequest();
}
