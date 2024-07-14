// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "EIK_PlayerDataStorage_DuplicateFile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_PlayerDataStorage_DuplicateFileDelegate, const TEnumAsByte<EEIK_Result>&, Result, const FEIK_ProductUserId&, LocalUserId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_PlayerDataStorage_DuplicateFile : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Copies the data of an existing file to a new filename. This action happens entirely on the server and will not upload the contents of the source destination file from the host. This function paired with a subsequent EOS_PlayerDataStorage_DeleteFile can be used to rename a file. If successful, the destination file's metadata will be updated in our local cache.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface", DisplayName="EOS_PlayerDataStorage_DuplicateFile")
	static UEIK_PlayerDataStorage_DuplicateFile* EIK_PlayerDataStorage_DuplicateFile(FEIK_ProductUserId LocalUserId, FString SourceFilename, FString DestinationFilename);

	UPROPERTY(BlueprintAssignable)
	FEIK_PlayerDataStorage_DuplicateFileDelegate OnCallback;
	
private:
	virtual void Activate() override;
	static void EOS_CALL EOS_PlayerDataStorage_OnDuplicateFileComplete(const EOS_PlayerDataStorage_DuplicateFileCallbackInfo* Data);
	FEIK_ProductUserId Var_LocalUserId;
	FString Var_SourceFilename;
	FString Var_DestinationFilename;
};
