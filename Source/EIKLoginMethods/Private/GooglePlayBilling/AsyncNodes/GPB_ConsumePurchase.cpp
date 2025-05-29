// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#include "GooglePlayBilling/AsyncNodes/GPB_ConsumePurchase.h"
#include "TimerManager.h"

#if GPB_SUPPORTED
#include "Android/Utils/AndroidJNIConvertor.h"
#endif

TWeakObjectPtr<UGPB_ConsumePurchase> UGPB_ConsumePurchase::StaticInstance;
FTimerHandle UGPB_ConsumePurchase::TimeoutTimerHandle;

UGPB_ConsumePurchase::UGPB_ConsumePurchase()
{
}

UGPB_ConsumePurchase* UGPB_ConsumePurchase::ConsumePurchase(const FString& PurchaseToken, UObject* WorldContextObject)
{
    UGPB_ConsumePurchase* Node = NewObject<UGPB_ConsumePurchase>();
    Node->PurchaseToken = PurchaseToken;
    Node->World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
    StaticInstance = Node;
    return Node;
}

void UGPB_ConsumePurchase::Activate()
{
    Super::Activate();

    UGPB_ConsumePurchase::StaticInstance = this;

    // Set up timeout
    if (World.IsValid())
    {
        World->GetTimerManager().SetTimer(
            TimeoutTimerHandle,
            this,
            &UGPB_ConsumePurchase::OnTimeout,
            GPB_AsyncTimeoutSeconds, // Use global configurable timeout
            false
        );
    }

#if GPB_SUPPORTED
    INIT_JAVA_METHOD(AndroidThunkJava_GPBL_consumePurchase, "(Ljava/lang/String;)V");
    if (JNIEnv* JNIEnvPtr = FAndroidApplication::GetJavaEnv(true))
    {
        FJavaWrapper::CallVoidMethod(JNIEnvPtr, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPBL_consumePurchase, 
            AndroidJNIConvertor::GetJavaString(PurchaseToken));
    }
#else
    OnComplete.Broadcast(false, PurchaseToken, "ERROR: Google Play Billing Not Supported!");
    SetReadyToDestroy();
#endif
}

void UGPB_ConsumePurchase::BeginDestroy()
{
    // Clear timeout timer
    if (World.IsValid())
    {
        World->GetTimerManager().ClearTimer(TimeoutTimerHandle);
    }

    UGPB_ConsumePurchase::StaticInstance = nullptr;
    Super::BeginDestroy();
}

void UGPB_ConsumePurchase::OnTimeout()
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
    Java_com_epicgames_unreal_GameActivity_nativeOnConsumeResponse(
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
            if (UGPB_ConsumePurchase::StaticInstance.Get())
            {
                // Always clear timeout timer first
                if (UGPB_ConsumePurchase::StaticInstance->World.IsValid())
                {
                    UGPB_ConsumePurchase::StaticInstance->World->GetTimerManager().ClearTimer(UGPB_ConsumePurchase::TimeoutTimerHandle);
                }
                // Now broadcast and destroy
                UGPB_ConsumePurchase::StaticInstance->OnComplete.Broadcast(bSuccessCpp, PurchaseTokenStr, ErrorStr);
                UGPB_ConsumePurchase::StaticInstance->SetReadyToDestroy();
            }
        });
    }
}
#endif 