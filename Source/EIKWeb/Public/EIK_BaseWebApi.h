// Copyright (c) 2024 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/BlueprintAsyncActionBase.h"

USTRUCT(BlueprintType)
struct FEIK_BaseWebApiResponse
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit|Web")
	int32 StatusCode;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit|Web")
	FString Response;

	FEIK_BaseWebApiResponse()
	{
		StatusCode = -1;
		Response = "";
	}
	FEIK_BaseWebApiResponse(int32 InStatusCode, const FString& InResponse)
	{
		StatusCode = InStatusCode;
		Response = InResponse;
	}

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_BaseWebApiDelegate, bool, bSuccess, const FEIK_BaseWebApiResponse&, Response);
UCLASS()
class UEIK_BaseWebApi : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit|Web")
	FEIK_BaseWebApiDelegate OnResponse;

protected:
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	FString APIEndpoint = "https://api.epicgames.dev";
};
