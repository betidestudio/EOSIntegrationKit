// Copyright Epic Games, Inc. All Rights Reserved.


#include "EIK_SendReport_AsyncFunction.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOS.h"

// Constructor for creating an instance of the asynchronous function
UEIK_SendReport_AsyncFunction* UEIK_SendReport_AsyncFunction::SendEIKPlayerReportAsyncFunction(FString LocalReporterPUID, FString TargetPlayerPUID, E_PlayerReportCategory ReportCategory, FString Message)
{
	UEIK_SendReport_AsyncFunction* UEIK_SendReport_Object = NewObject < UEIK_SendReport_AsyncFunction>();
	UEIK_SendReport_Object->LocalReporterPUID = LocalReporterPUID;
	UEIK_SendReport_Object->TargetPlayerPUID = TargetPlayerPUID;
	UEIK_SendReport_Object->ReportCategory = ReportCategory;
	UEIK_SendReport_Object->Message = Message;
	return UEIK_SendReport_Object;
}

// Function to send the player behavior report
void UEIK_SendReport_AsyncFunction::SendReportFunc()
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if (EOSRef->ReportsHandle != nullptr)
			{
				EOS_Reports_SendPlayerBehaviorReportOptions ReportOptions;

				// Set API version and user IDs
				ReportOptions.ApiVersion = EOS_REPORTS_SENDPLAYERBEHAVIORREPORT_API_LATEST;
				ReportOptions.ReporterUserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*LocalReporterPUID));
				ReportOptions.ReportedUserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*TargetPlayerPUID));

				// Map the custom enumeration to EOS enumeration
				switch (ReportCategory)
				{
					/*case E_PlayerReportCategory::EOS_PRC_Invalid:
					ReportOptions.Category = EOS_EPlayerReportsCategory::EOS_PRC_Invalid; (This will give an error and report won't be sen't)
					break;*/

				case E_PlayerReportCategory::EOS_PRC_Cheating:
					ReportOptions.Category = EOS_EPlayerReportsCategory::EOS_PRC_Cheating;
					break;

				case E_PlayerReportCategory::EOS_PRC_Exploiting:
					ReportOptions.Category = EOS_EPlayerReportsCategory::EOS_PRC_Exploiting;
					break;

				case E_PlayerReportCategory::EOS_PRC_OffensiveProfile:
					ReportOptions.Category = EOS_EPlayerReportsCategory::EOS_PRC_OffensiveProfile;
					break;

				case E_PlayerReportCategory::EOS_PRC_VerbalAbuse:
					ReportOptions.Category = EOS_EPlayerReportsCategory::EOS_PRC_VerbalAbuse;
					break;

				case E_PlayerReportCategory::EOS_PRC_Scamming:
					ReportOptions.Category = EOS_EPlayerReportsCategory::EOS_PRC_Scamming;
					break;

				case E_PlayerReportCategory::EOS_PRC_Spamming:
					ReportOptions.Category = EOS_EPlayerReportsCategory::EOS_PRC_Spamming;
					break;

				case E_PlayerReportCategory::EOS_PRC_Other:
					ReportOptions.Category = EOS_EPlayerReportsCategory::EOS_PRC_Other;
					break;
				}

				//Set message and context
				ReportOptions.Context = "{}"; /*Not used but have to include*/
				ReportOptions.Message = TCHAR_TO_UTF8(*Message);

				// Call EOS SDK to send the report
				EOS_Reports_SendPlayerBehaviorReport(EOSRef->ReportsHandle, &ReportOptions, this, SendReportFuncCallback);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("EOSRef->ReportsHandle = nullptr"));
			}
		}
		else
		{
			ResultFaliure();
		}
	}
	else
	{
		ResultFaliure();
	}
}

// Callback function called when the player behavior report is complete
void UEIK_SendReport_AsyncFunction::SendReportFuncCallback(const EOS_Reports_SendPlayerBehaviorReportCompleteCallbackInfo* Data)
{
	if (Data->ClientData)
	{  
		// Check if the client data is valid
		if (UEIK_SendReport_AsyncFunction* SendReportFunction = static_cast<UEIK_SendReport_AsyncFunction*>(Data->ClientData))
		{
			// Check the result code and trigger appropriate actions
			if (Data->ResultCode == EOS_EResult::EOS_Success)
			{
				SendReportFunction->ResultSuccess();
			}
			else
			{
				SendReportFunction->ResultFaliure();
			}
		}
		else 
		{
			UE_LOG(LogTemp, Error, TEXT("UEIK_SendReport_AsyncFunction is null. No callback will be fired"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ClientData is null. No callback will be fired"));
	}
	
}

// Function to handle failure cases
void UEIK_SendReport_AsyncFunction::ResultFaliure()
{
	Failure.Broadcast();
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

// Function to handle success cases
void UEIK_SendReport_AsyncFunction::ResultSuccess()
{
	Success.Broadcast();
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

// Function to activate the asynchronous function
void UEIK_SendReport_AsyncFunction::Activate()
{
	SendReportFunc();
	Super::Activate();
}
