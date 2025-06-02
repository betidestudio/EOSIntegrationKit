// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#include "GooglePlayBilling/AsyncNodes/GPB_LaunchPurchaseFlow.h"
#include "TimerManager.h"

#if GPB_SUPPORTED
#include "Android/Utils/AndroidJNIConvertor.h"
#endif

TWeakObjectPtr<UGPB_LaunchPurchaseFlow> UGPB_LaunchPurchaseFlow::StaticInstance = nullptr;
FTimerHandle UGPB_LaunchPurchaseFlow::TimeoutTimerHandle;

UGPB_LaunchPurchaseFlow::UGPB_LaunchPurchaseFlow()
{
    World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull);
}

UGPB_LaunchPurchaseFlow* UGPB_LaunchPurchaseFlow::LaunchPurchaseFlow(const FGPB_ProductDetails& ProductDetails, const FString& OfferToken)
{
    UGPB_LaunchPurchaseFlow* Node = NewObject<UGPB_LaunchPurchaseFlow>();
    Node->ProductDetails = ProductDetails;
    Node->OfferToken = OfferToken;
    StaticInstance = Node;
    return Node;
}

void UGPB_LaunchPurchaseFlow::Activate()
{
    Super::Activate();

    UGPB_LaunchPurchaseFlow::StaticInstance = this;

    // Set up timeout
    if (World.IsValid())
    {
        World->GetTimerManager().SetTimer(
            TimeoutTimerHandle,
            this,
            &UGPB_LaunchPurchaseFlow::OnTimeout,
            GPB_AsyncTimeoutSeconds, // Use global configurable timeout
            false
        );
    }

#if GPB_SUPPORTED
    INIT_JAVA_METHOD(AndroidThunkJava_GPBL_launchPurchaseFlow, "(Ljava/lang/String;Ljava/lang/String;)V");
    if (JNIEnv* JNIEnvPtr = FAndroidApplication::GetJavaEnv(true))
    {
        FJavaWrapper::CallVoidMethod(JNIEnvPtr, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPBL_launchPurchaseFlow, 
            AndroidJNIConvertor::GetJavaString(ProductDetails.ToJson()),
            AndroidJNIConvertor::GetJavaString(OfferToken));
    }
#else
    OnComplete.Broadcast(false, FGPB_Purchase(), "ERROR: Google Play Billing Not Supported!");
    SetReadyToDestroy();
#endif
}

void UGPB_LaunchPurchaseFlow::BeginDestroy()
{
    // Clear timeout timer
    if (World.IsValid())
    {
        World->GetTimerManager().ClearTimer(TimeoutTimerHandle);
    }

    UGPB_LaunchPurchaseFlow::StaticInstance = nullptr;
    Super::BeginDestroy();
}

void UGPB_LaunchPurchaseFlow::OnTimeout()
{
    if (StaticInstance.Get())
    {
        StaticInstance->OnComplete.Broadcast(false, FGPB_Purchase(), "Timeout! Check parameters and try again.");
        StaticInstance->SetReadyToDestroy();
    }
}

#if GPB_SUPPORTED
extern "C"
{
    JNIEXPORT void JNICALL
    Java_com_epicgames_unreal_GameActivity_nativeOnPurchasesUpdated(
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
            if (UGPB_LaunchPurchaseFlow::StaticInstance.Get())
            {
                // Clear timeout timer since we got a response
                if (UWorld* World = UGPB_LaunchPurchaseFlow::StaticInstance->GetWorld())
                {
                    World->GetTimerManager().ClearTimer(UGPB_LaunchPurchaseFlow::TimeoutTimerHandle);
                }

                if (!bSuccessCpp)
                {
                    // If the operation failed, broadcast failure immediately
                    UGPB_LaunchPurchaseFlow::StaticInstance->OnComplete.Broadcast(false, FGPB_Purchase(), ErrorStr);
                    UGPB_LaunchPurchaseFlow::StaticInstance->SetReadyToDestroy();
                    return;
                }

                // For success case, try to find our purchase
                for (const FGPB_Purchase& Purchase : Purchases)
                {
                    if (Purchase.Products.Contains(UGPB_LaunchPurchaseFlow::StaticInstance->ProductDetails.ProductId))
                    {
                        UGPB_LaunchPurchaseFlow::StaticInstance->OnComplete.Broadcast(true, Purchase, ErrorStr);
                        UGPB_LaunchPurchaseFlow::StaticInstance->SetReadyToDestroy();
                        return;
                    }
                }

                // If we get here, we didn't find our purchase in the success case
                UGPB_LaunchPurchaseFlow::StaticInstance->OnComplete.Broadcast(false, FGPB_Purchase(), ErrorStr);
                UGPB_LaunchPurchaseFlow::StaticInstance->SetReadyToDestroy();
            }
        });
    }
}
#endif 