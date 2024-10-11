// Copyright Epic Games, Inc. All Rights Reserved.

#include "EIK_GetLeaderboardForUserIds.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "eos_leaderboards.h"


UEIK_GetLeaderboardForUserIds* UEIK_GetLeaderboardForUserIds::GetEIKLeaderboardForUserIds(TArray<FString> TargetProductUserIds, FString LocalProductUserId, bool UseTime, FDateTime StartTime, FDateTime EndTime, ELeaderboardAggregation AggregationType, FString StatName)
{
	UEIK_GetLeaderboardForUserIds* GetLeaderboardForUserIdsObj = NewObject < UEIK_GetLeaderboardForUserIds >();

	GetLeaderboardForUserIdsObj->Var_LocalProductUserId = LocalProductUserId;
	GetLeaderboardForUserIdsObj->Var_TargetProductUserIds = TargetProductUserIds;
    GetLeaderboardForUserIdsObj->Var_bUseTime = UseTime;
    GetLeaderboardForUserIdsObj->Var_StartTime = StartTime;
    GetLeaderboardForUserIdsObj->Var_EndTime = EndTime;
    GetLeaderboardForUserIdsObj->Var_AggregationType = AggregationType;
    GetLeaderboardForUserIdsObj->Var_StatName = StatName;

	return GetLeaderboardForUserIdsObj;
}

void UEIK_GetLeaderboardForUserIds::GetLeaderboardForUserIds()
{
    if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
    {
        if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
        {
            if (EOSRef->LeaderboardsHandle != nullptr)
            {
                EOS_Leaderboards_QueryLeaderboardUserScoresOptions QueryOptions = {};

                QueryOptions.ApiVersion = EOS_LEADERBOARDS_QUERYLEADERBOARDUSERSCORES_API_LATEST;
                if (Var_bUseTime)
                {
                    QueryOptions.StartTime = Var_StartTime.ToUnixTimestamp();
                    QueryOptions.EndTime = Var_EndTime.ToUnixTimestamp();
                }
                else
                {
                    QueryOptions.StartTime = EOS_LEADERBOARDS_TIME_UNDEFINED;
                    QueryOptions.EndTime = EOS_LEADERBOARDS_TIME_UNDEFINED;
                }
                QueryOptions.LocalUserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*Var_LocalProductUserId));

                EOS_Leaderboards_UserScoresQueryStatInfo QueryStatInfo = {};

                QueryStatInfo.ApiVersion = EOS_LEADERBOARDS_USERSCORESQUERYSTATINFO_API_LATEST;

                switch (Var_AggregationType)
                {
                case ELeaderboardAggregation::EOS_LA_Min:
                    QueryStatInfo.Aggregation = EOS_ELeaderboardAggregation::EOS_LA_Min;
                    break;

                case ELeaderboardAggregation::EOS_LA_Max:
                    QueryStatInfo.Aggregation = EOS_ELeaderboardAggregation::EOS_LA_Max;
                    break;

                case ELeaderboardAggregation::EOS_LA_Sum:
                    QueryStatInfo.Aggregation = EOS_ELeaderboardAggregation::EOS_LA_Sum;
                    break;

                case ELeaderboardAggregation::EOS_LA_Latest:
                    QueryStatInfo.Aggregation = EOS_ELeaderboardAggregation::EOS_LA_Latest;
                    break;
                }

                QueryStatInfo.StatName = TCHAR_TO_UTF8(*Var_StatName);

                QueryOptions.StatInfo = &QueryStatInfo;

                QueryOptions.StatInfoCount = 1;

                TArray<EOS_ProductUserId> TempProductUserIdsArray;

                for (FString UserId : Var_TargetProductUserIds)
                {
                    TempProductUserIdsArray.Add(EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*UserId)));
                }

                QueryOptions.UserIds = TempProductUserIdsArray.GetData();
                QueryOptions.UserIdsCount = TempProductUserIdsArray.Num();

                EOS_Leaderboards_QueryLeaderboardUserScores(EOSRef->LeaderboardsHandle, &QueryOptions, this, &UEIK_GetLeaderboardForUserIds::GetLeaderboardForUserIdsCallback);

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
    else
    {
        ResultFaliure();
    }
}

void UEIK_GetLeaderboardForUserIds::GetLeaderboardForUserIdsCallback(const EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallbackInfo* Data)
{
    if (Data->ClientData)
    {
        // Check if the client data is valid
        if (UEIK_GetLeaderboardForUserIds* GetLeaderboardForUserIdsFunc = static_cast<UEIK_GetLeaderboardForUserIds*>(Data->ClientData))
        {
            if (Data->ResultCode == EOS_EResult::EOS_Success)
            {
                GetLeaderboardForUserIdsFunc->ResultSuccess();

            }
            else
            {
                GetLeaderboardForUserIdsFunc->ResultFaliure();
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("No callback will be fired"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ClientData is null. No callback will be fired"));
    }
}

void UEIK_GetLeaderboardForUserIds::ResultSuccess()
{
    if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
    {
        if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
        {
            if (EOSRef->LeaderboardsHandle != nullptr)
            {
                TArray<FEIKExtendedLeaderboardValue> EIKExtendedLeaderboardValues;

                EOS_Leaderboards_GetLeaderboardUserScoreCountOptions GetLeaderboardRecordCountOptions = {};

                GetLeaderboardRecordCountOptions.ApiVersion = EOS_LEADERBOARDS_GETLEADERBOARDUSERSCORECOUNT_API_LATEST;
                GetLeaderboardRecordCountOptions.StatName = TCHAR_TO_UTF8(*Var_StatName);


                uint32_t LeaderboardRecordCount = EOS_Leaderboards_GetLeaderboardUserScoreCount(EOSRef->LeaderboardsHandle, &GetLeaderboardRecordCountOptions);

                for (uint32_t Index = 0; Index < LeaderboardRecordCount; ++Index)
                {

                    EOS_Leaderboards_CopyLeaderboardUserScoreByIndexOptions CopyLeaderboardRecordByIndexOptions = {};

                    CopyLeaderboardRecordByIndexOptions.ApiVersion = EOS_LEADERBOARDS_COPYLEADERBOARDUSERSCOREBYINDEX_API_LATEST;
                    CopyLeaderboardRecordByIndexOptions.LeaderboardUserScoreIndex = Index;
                    CopyLeaderboardRecordByIndexOptions.StatName = TCHAR_TO_UTF8(*Var_StatName);

                    EOS_Leaderboards_LeaderboardUserScore* OutLeaderboardRecord = nullptr;

                    if (EOS_Leaderboards_CopyLeaderboardUserScoreByIndex(EOSRef->LeaderboardsHandle, &CopyLeaderboardRecordByIndexOptions, &OutLeaderboardRecord) == EOS_EResult::EOS_Success)
                    {
                        FEIKExtendedLeaderboardValue TempEIKExtendedLeaderboardValue;

                        TempEIKExtendedLeaderboardValue.Score = OutLeaderboardRecord->Score;

                        char PUIDString[EOS_PRODUCTUSERID_MAX_LENGTH + 1]; // +1 for the null-terminator
                        int32_t BufferSize = sizeof(PUIDString);

                        EOS_EResult Result = EOS_ProductUserId_ToString(OutLeaderboardRecord->UserId, PUIDString, &BufferSize);

                        if (Result == EOS_EResult::EOS_Success)
                        {
                            TempEIKExtendedLeaderboardValue.UserId = (UTF8_TO_TCHAR(PUIDString));
                        }    

                        EIKExtendedLeaderboardValues.Add(TempEIKExtendedLeaderboardValue);

                        EOS_Leaderboards_LeaderboardUserScore_Release(OutLeaderboardRecord);
                    }

                }

                Success.Broadcast(EIKExtendedLeaderboardValues);

                SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
                MarkAsGarbage();
#else
                MarkPendingKill();
#endif

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
    else
    {
        ResultFaliure();
    }
}

void UEIK_GetLeaderboardForUserIds::ResultFaliure()
{
    Faliure.Broadcast(TArray<FEIKExtendedLeaderboardValue>());
    SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
    MarkAsGarbage();
#else
    MarkPendingKill();
#endif
}

void UEIK_GetLeaderboardForUserIds::Activate()
{
	GetLeaderboardForUserIds();
	Super::Activate();
}
