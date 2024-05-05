// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "eos_reports.h"
#include "eos_reports_types.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "EIK_SendReport_AsyncFunction.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class E_PlayerReportCategory : uint8
{
	/** Not used, gives error
	EOS_PRC_Invalid  UMETA(DisplayName = "Invalid"),*/

	/** The reported player is cheating */
	EOS_PRC_Cheating  UMETA(DisplayName = "Cheating"),
	/** The reported player is exploiting the game */
	EOS_PRC_Exploiting  UMETA(DisplayName = "Exploiting"),
	/** The reported player has an offensive profile, name, etc */
	EOS_PRC_OffensiveProfile  UMETA(DisplayName = "OffensiveProfile"),
	/** The reported player is being abusive in chat */
	EOS_PRC_VerbalAbuse  UMETA(DisplayName = "VerbalAbuse"),
	/** The reported player is scamming other players */
	EOS_PRC_Scamming  UMETA(DisplayName = "Scamming"),
	/** The reported player is spamming chat */
	EOS_PRC_Spamming  UMETA(DisplayName = "Spamming"),
	/** The player is being reported for something else */
	EOS_PRC_Other  UMETA(DisplayName = "Other"),
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReportsDelegate);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_SendReport_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:

	//PUID's'
	FString LocalReporterPUID;
	FString TargetPlayerPUID;

	//ReportCategory Enum
	E_PlayerReportCategory ReportCategory;

	//Message
	FString Message;

	//Delegates
	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit || Reports")
	FReportsDelegate Success;

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit || Reports")
	FReportsDelegate Failure;


	//Functions
	UFUNCTION(BlueprintCallable, DisplayName = "Send EIK Player Report", meta = (BlueprintInternalUseOnly = "true"), Category = "EOS Integration Kit || Reports")
	static UEIK_SendReport_AsyncFunction* SendEIKPlayerReportAsyncFunction(FString LocalReporterPUID, FString TargetPlayerPUID, E_PlayerReportCategory ReportCategory, FString Message);

	void SendReportFunc();

	static void EOS_CALL SendReportFuncCallback(const EOS_Reports_SendPlayerBehaviorReportCompleteCallbackInfo* Data);

	void ResultFaliure();

	void ResultSuccess();

	void Activate() override;

};
