// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_UpdatingSanctions.h"

UEIK_API_UpdatingSanctions* UEIK_API_UpdatingSanctions::UpdatingSanctions(FString Authorization, FString DeploymentId, TArray<FEIK_SanctionPatchPayload> SanctionPatchPayload)
{
	UEIK_API_UpdatingSanctions* Proxy = NewObject<UEIK_API_UpdatingSanctions>();
	Proxy->Var_Authorization = Authorization;
	Proxy->Var_DeploymentId = DeploymentId;
	Proxy->Var_SanctionPatchPayload = SanctionPatchPayload;
	return Proxy;
}

void UEIK_API_UpdatingSanctions::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/sanctions/v1/%s/sanctions"), *APIEndpoint, *Var_DeploymentId);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("PATCH"));
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
	TSharedPtr<FJsonObject> RequestBodyJson = MakeShareable(new FJsonObject);
	if (Var_SanctionPatchPayload.Num() > 0)
	{
		TArray<TSharedPtr<FJsonValue>> SanctionsJson;
		for (auto Sanction : Var_SanctionPatchPayload)
		{
			TSharedPtr<FJsonObject> SanctionJson = MakeShareable(new FJsonObject);
			if (!Sanction.ReferenceId.IsEmpty())
			{
				SanctionJson->SetStringField(TEXT("referenceId"), Sanction.ReferenceId);
			}
			for(auto UpdateItem : Sanction.Updates)
			{
				if (!UpdateItem.Justification.IsEmpty())
				{
					SanctionJson->SetStringField(TEXT("justification"), UpdateItem.Justification);
				}
				if (UpdateItem.Metadata.Num() > 0)
				{
					TSharedPtr<FJsonObject> MetadataJson = MakeShareable(new FJsonObject);
					for (auto MetadataItem : UpdateItem.Metadata)
					{
						MetadataJson->SetStringField(MetadataItem.Key, MetadataItem.Value);
					}
					SanctionJson->SetObjectField(TEXT("metadata"), MetadataJson);
				}
				if (UpdateItem.Tags.Num() > 0)
				{
					TArray<TSharedPtr<FJsonValue>> TagsJson;
					for (auto Tag : UpdateItem.Tags)
					{
						TagsJson.Add(MakeShareable(new FJsonValueString(Tag)));
					}
					SanctionJson->SetArrayField(TEXT("tags"), TagsJson);
				}
			}
			SanctionsJson.Add(MakeShareable(new FJsonValueObject(SanctionJson)));
		}
		RequestBodyJson->SetArrayField(TEXT("sanctions"), SanctionsJson);
	}
	FString RequestBody;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestBodyJson.ToSharedRef(), JsonWriter);
	HttpRequest->SetContentAsString(RequestBody);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_UpdatingSanctions::OnResponseReceived);
	HttpRequest->ProcessRequest();
}