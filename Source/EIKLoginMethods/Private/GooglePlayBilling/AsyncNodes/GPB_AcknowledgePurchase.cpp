// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#include "GooglePlayBilling/AsyncNodes/GPB_AcknowledgePurchase.h"
#include "TimerManager.h"

#if GPB_SUPPORTED
#include "Android/Utils/AndroidJNIConvertor.h"
#endif

TWeakObjectPtr<UGPB_AcknowledgePurchase> UGPB_AcknowledgePurchase::StaticInstance;
FTimerHandle UGPB_AcknowledgePurchase::TimeoutTimerHandle;

UGPB_AcknowledgePurchase::UGPB_AcknowledgePurchase()
{
}

UGPB_AcknowledgePurchase* UGPB_AcknowledgePurchase::AcknowledgePurchase(const FString& PurchaseToken, UObject* WorldContextObject)
{
    UGPB_AcknowledgePurchase* Node = NewObject<UGPB_AcknowledgePurchase>();
    Node->PurchaseToken = PurchaseToken;
    Node->World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
    StaticInstance = Node;
    return Node;
}

void UGPB_AcknowledgePurchase::Activate()
{
    Super::Activate();

    UGPB_AcknowledgePurchase::StaticInstance = this;

    // Set up timeout
    if (World.IsValid())
    {
        World->GetTimerManager().SetTimer(
            TimeoutTimerHandle,
            this,
            &UGPB_AcknowledgePurchase::OnTimeout,
            GPB_AsyncTimeoutSeconds, // Use global configurable timeout
            false
        );
    }

#if GPB_SUPPORTED
    INIT_JAVA_METHOD(AndroidThunkJava_GPBL_acknowledgePurchase, "(Ljava/lang/String;)V");
    if (JNIEnv* JNIEnvPtr = FAndroidApplication::GetJavaEnv(true))
    {
        FJavaWrapper::CallVoidMethod(JNIEnvPtr, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPBL_acknowledgePurchase, 
            AndroidJNIConvertor::GetJavaString(PurchaseToken));
    }
#else
    OnComplete.Broadcast(false, PurchaseToken, "ERROR: Google Play Billing Not Supported!");
    SetReadyToDestroy();
#endif
}

void UGPB_AcknowledgePurchase::BeginDestroy()
{
    // Clear timeout timer
    if (World.IsValid())
    {
        World->GetTimerManager().ClearTimer(TimeoutTimerHandle);
    }

    UGPB_AcknowledgePurchase::StaticInstance = nullptr;
    Super::BeginDestroy();
}

void UGPB_AcknowledgePurchase::OnTimeout()
{
    if (StaticInstance.Get())
    {
        StaticInstance->OnComplete.Broadcast(false, StaticInstance->PurchaseToken, "Timeout! Check parameters and try again.");
        StaticInstance->SetReadyToDestroy();
    }
}

#if GPB_SUPPORTED
extern "C"
{
    JNIEXPORT void JNICALL
    Java_com_epicgames_unreal_GameActivity_nativeOnAcknowledgeResponse(
        JNIEnv* Env,
        jclass Clazz,
        jboolean bSuccess,
        jstring PurchaseToken,
        jstring Error)
    {
        bool bSuccessCpp = (bool)bSuccess;
        FString PurchaseTokenStr = AndroidJNIConvertor::FromJavaString(PurchaseToken);
        FString ErrorStr = AndroidJNIConvertor::FromJavaString(Error);

        AsyncTask(ENamedThreads::GameThread, [bSuccessCpp, PurchaseTokenStr, ErrorStr]()
        {
            if (UGPB_AcknowledgePurchase::StaticInstance.Get())
            {
                // Always clear timeout timer first
                if (UGPB_AcknowledgePurchase::StaticInstance->World.IsValid())
                {
                    UGPB_AcknowledgePurchase::StaticInstance->World->GetTimerManager().ClearTimer(UGPB_AcknowledgePurchase::TimeoutTimerHandle);
                }
                // Now broadcast and destroy
                UGPB_AcknowledgePurchase::StaticInstance->OnComplete.Broadcast(bSuccessCpp, PurchaseTokenStr, ErrorStr);
                UGPB_AcknowledgePurchase::StaticInstance->SetReadyToDestroy();
            }
        });
    }
}
#endif 