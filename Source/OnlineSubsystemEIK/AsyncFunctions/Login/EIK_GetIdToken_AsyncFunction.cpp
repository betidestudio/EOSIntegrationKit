// Copyright Epic Games, Inc. All Rights Reserved.


#include "EIK_GetIdToken_AsyncFunction.h"

#include "eos_auth.h"
#include "eos_auth_types.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOS.h"

UEIK_GetIdToken_AsyncFunction* UEIK_GetIdToken_AsyncFunction::GetEIKAuthToken(FString EpicAccountId)
{
	UEIK_GetIdToken_AsyncFunction* EIK_GetAuthToken_Object = NewObject < UEIK_GetIdToken_AsyncFunction>();

	EIK_GetAuthToken_Object->Var_EpicAccountId = EpicAccountId;

	return EIK_GetAuthToken_Object;
}

void UEIK_GetIdToken_AsyncFunction::GetAuthToken()
{
    if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
    {
        if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
        {
            if (EOSRef->AuthHandle != nullptr)
            {
                EOS_Auth_CopyIdTokenOptions Options;
                Options.ApiVersion = EOS_AUTH_COPYIDTOKEN_API_LATEST;
                Options.AccountId = EOS_EpicAccountId_FromString(TCHAR_TO_UTF8(*Var_EpicAccountId));

                // Check the result of EOS_Auth_CopyIdToken
                EOS_EResult CopyIdTokenResult = EOS_Auth_CopyIdToken(EOSRef->AuthHandle, &Options, &Var_IdToken);
                if (CopyIdTokenResult == EOS_EResult::EOS_Success && Var_IdToken)
                {
                    char Buffer[EOS_EPICACCOUNTID_MAX_LENGTH + 1];
                    int32_t BufferLength = EOS_EPICACCOUNTID_MAX_LENGTH + 1;
                    FEIKCopyAuthToken EIKCopyAuthToken;

                    EIKCopyAuthToken.IdToken_JWT = FString(Var_IdToken->JsonWebToken);

                    if (EOS_EpicAccountId_ToString(Var_IdToken->AccountId, Buffer, &BufferLength) == EOS_EResult::EOS_Success)
                    {
                        EIKCopyAuthToken.EpicAccountId = FString(UTF8_TO_TCHAR(Buffer));
                    }

                    Success.Broadcast(EIKCopyAuthToken);

                    // Release
                    EOS_Auth_IdToken_Release(Var_IdToken);

                    SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
                    MarkAsGarbage();
#else
                    MarkPendingKill();
#endif
                }
                else
                {
                    // Broadcast failure event
                    Failure.Broadcast(FEIKCopyAuthToken());
                    SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
                    MarkAsGarbage();
#else
                    MarkPendingKill();
#endif
                }
            }
            else
            {
                // Broadcast failure event
                Failure.Broadcast(FEIKCopyAuthToken());
                SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
                MarkAsGarbage();
#else
                MarkPendingKill();
#endif
            }
        }
        else
        {
            // Broadcast failure event
            Failure.Broadcast(FEIKCopyAuthToken());
            SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
            MarkAsGarbage();
#else
            MarkPendingKill();
#endif
        }
    }
    else
    {
        // Broadcast failure event
        Failure.Broadcast(FEIKCopyAuthToken());
        SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
        MarkAsGarbage();
#else
        MarkPendingKill();
#endif
    }
}

void UEIK_GetIdToken_AsyncFunction::Activate()
{
	GetAuthToken();
	Super::Activate();
}
