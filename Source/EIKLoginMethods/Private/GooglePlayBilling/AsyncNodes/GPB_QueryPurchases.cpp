// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#include "GooglePlayBilling/AsyncNodes/GPB_QueryPurchases.h"
#include "TimerManager.h"

#if GPB_SUPPORTED
#include "Android/Utils/AndroidJNIConvertor.h"
#endif

TWeakObjectPtr<UGPB_QueryPurchases> UGPB_QueryPurchases::StaticInstance;
FTimerHandle UGPB_QueryPurchases::TimeoutTimerHandle;

UGPB_QueryPurchases::UGPB_QueryPurchases()
{
}

UGPB_QueryPurchases* UGPB_QueryPurchases::QueryPurchases(bool bSubscription, UObject* WorldContextObject)
{
    UGPB_QueryPurchases* Node = NewObject<UGPB_QueryPurchases>();
    Node->bSubscription = bSubscription;
    Node->World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
    StaticInstance = Node;
    return Node;
}

void UGPB_QueryPurchases::Activate()
{
    Super::Activate();

    UGPB_QueryPurchases::StaticInstance = this;

    // Set up timeout
    if (World.IsValid())
    {
        World->GetTimerManager().SetTimer(
            TimeoutTimerHandle,
            this,
            &UGPB_QueryPurchases::OnTimeout,
            GPB_AsyncTimeoutSeconds, // Use global configurable timeout
            false
        );
    }

#if GPB_SUPPORTED
    INIT_JAVA_METHOD(AndroidThunkJava_GPBL_queryPurchases, "(Z)V");
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
    {
        FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPBL_queryPurchases, 
            (jboolean)bSubscription);
    }
#else
    TArray<FGPB_Purchase> Purchases;
    OnComplete.Broadcast(false, Purchases, "ERROR: Google Play Billing Not Supported!");
    SetReadyToDestroy();
#endif
}

void UGPB_QueryPurchases::BeginDestroy()
{
    // Clear timeout timer
    if (World.IsValid())
    {
        World->GetTimerManager().ClearTimer(TimeoutTimerHandle);
    }

    UGPB_QueryPurchases::StaticInstance = nullptr;
    Super::BeginDestroy();
}

void UGPB_QueryPurchases::OnTimeout()
{
    if (StaticInstance.Get())
    {
        TArray<FGPB_Purchase> EmptyPurchases;
        StaticInstance->OnComplete.Broadcast(false, EmptyPurchases, "Timeout! Check parameters and try again.");
        StaticInstance->SetReadyToDestroy();
    }
}

#if GPB_SUPPORTED
extern "C"
{
    JNIEXPORT void JNICALL
    Java_com_epicgames_unreal_GameActivity_nativeOnPurchasesQueryResponse(
        JNIEnv* Env,
        jclass Clazz,
        jboolean bSuccess,
        jobjectArray PurchasesJsons,
        jstring Error)
    {
        bool bSuccessCpp = (bool)bSuccess;
        FString ErrorStr = AndroidJNIConvertor::FromJavaString(Error);

        TArray<FString> PurchasesJsonArray;
        if (PurchasesJsons != nullptr)
        {
            jsize len = Env->GetArrayLength(PurchasesJsons);
            for (jsize i = 0; i < len; ++i)
            {
                jstring jStr = (jstring)Env->GetObjectArrayElement(PurchasesJsons, i);
                FString str = AndroidJNIConvertor::FromJavaString(jStr);
                PurchasesJsonArray.Add(str);
                Env->DeleteLocalRef(jStr);
            }
        }

        TArray<FGPB_Purchase> Purchases;
        for (const FString& Json : PurchasesJsonArray)
        {
            Purchases.Add(FGPB_Purchase::ParseFromJson(Json));
        }

        AsyncTask(ENamedThreads::GameThread, [bSuccessCpp, Purchases = MoveTemp(Purchases), ErrorStr]()
        {
            if (UGPB_QueryPurchases::StaticInstance.Get())
            {
                // Always clear timeout timer first
                if (UGPB_QueryPurchases::StaticInstance->World.IsValid())
                {
                    UGPB_QueryPurchases::StaticInstance->World->GetTimerManager().ClearTimer(UGPB_QueryPurchases::TimeoutTimerHandle);
                }
                // Now broadcast and destroy
                UGPB_QueryPurchases::StaticInstance->OnComplete.Broadcast(bSuccessCpp, Purchases, ErrorStr);
                UGPB_QueryPurchases::StaticInstance->SetReadyToDestroy();
            }
        });
    }
}
#endif 