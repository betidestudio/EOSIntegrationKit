// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#include "GooglePlayBilling/AsyncNodes/GPB_QueryProduct.h"
#include "TimerManager.h"

#if GPB_SUPPORTED
#include "Android/Utils/AndroidJNIConvertor.h"
#endif

TWeakObjectPtr<UGPB_QueryProduct> UGPB_QueryProduct::StaticInstance;
FTimerHandle UGPB_QueryProduct::TimeoutTimerHandle;

UGPB_QueryProduct::UGPB_QueryProduct()
{
}

UGPB_QueryProduct* UGPB_QueryProduct::QueryProduct(const FString& ProductID, bool bSubscription, UObject* WorldContextObject)
{
    UGPB_QueryProduct* Node = NewObject<UGPB_QueryProduct>();
    Node->ProductID = ProductID;
    Node->bSubscription = bSubscription;
    Node->World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
    StaticInstance = Node;
    return Node;
}

void UGPB_QueryProduct::Activate()
{
    Super::Activate();

    UGPB_QueryProduct::StaticInstance = this;

    // Set up timeout
    if (World.IsValid())
    {
        World->GetTimerManager().SetTimer(
            TimeoutTimerHandle,
            this,
            &UGPB_QueryProduct::OnTimeout,
            GPB_AsyncTimeoutSeconds, // Use global configurable timeout
            false
        );
    }

#if GPB_SUPPORTED
    INIT_JAVA_METHOD(AndroidThunkJava_GPBL_queryProduct, "(Ljava/lang/String;Z)V");
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
    {
        FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPBL_queryProduct, 
            AndroidJNIConvertor::GetJavaString(ProductID), 
            (jboolean)bSubscription);
    }
#else
    TArray<FGPB_ProductDetails> ProductDetails;
    OnComplete.Broadcast(false, ProductDetails, "ERROR: Google Play Billing Not Supported!");
    SetReadyToDestroy();
#endif
}

void UGPB_QueryProduct::BeginDestroy()
{
    // Clear timeout timer
    if (World.IsValid())
    {
        World->GetTimerManager().ClearTimer(TimeoutTimerHandle);
    }

    UGPB_QueryProduct::StaticInstance = nullptr;
    Super::BeginDestroy();
}

void UGPB_QueryProduct::OnTimeout()
{
    if (StaticInstance.Get())
    {
        TArray<FGPB_ProductDetails> EmptyProductDetails;
        StaticInstance->OnComplete.Broadcast(false, EmptyProductDetails, "Timeout! Check parameters and try again.");
        StaticInstance->SetReadyToDestroy();
    }
}

#if GPB_SUPPORTED
extern "C"
{
    JNIEXPORT void JNICALL
    Java_com_epicgames_unreal_GameActivity_nativeOnProductQueryResponse(
        JNIEnv* Env,
        jclass Clazz,
        jboolean bSuccess,
        jobjectArray ProductDetailsJsons,
        jstring Error)
    {
        bool bSuccessCpp = (bool)bSuccess;
        FString ErrorStr = AndroidJNIConvertor::FromJavaString(Error);

        TArray<FString> ProductDetailsJsonArray;
        if (ProductDetailsJsons != nullptr)
        {
            jsize len = Env->GetArrayLength(ProductDetailsJsons);
            for (jsize i = 0; i < len; ++i)
            {
                jstring jStr = (jstring)Env->GetObjectArrayElement(ProductDetailsJsons, i);
                FString str = AndroidJNIConvertor::FromJavaString(jStr);
                ProductDetailsJsonArray.Add(str);
                Env->DeleteLocalRef(jStr);
            }
        }

        TArray<FGPB_ProductDetails> ProductDetails;
        for (const FString& Json : ProductDetailsJsonArray)
        {
            ProductDetails.Add(FGPB_ProductDetails::ParseFromJson(Json));
        }

        AsyncTask(ENamedThreads::GameThread, [bSuccessCpp, ProductDetails = MoveTemp(ProductDetails), ErrorStr]()
        {
            if (UGPB_QueryProduct::StaticInstance.Get())
            {
                // Always clear timeout timer first
                if (UGPB_QueryProduct::StaticInstance->World.IsValid())
                {
                    UGPB_QueryProduct::StaticInstance->World->GetTimerManager().ClearTimer(UGPB_QueryProduct::TimeoutTimerHandle);
                }
                // Now broadcast and destroy
                UGPB_QueryProduct::StaticInstance->OnComplete.Broadcast(bSuccessCpp, ProductDetails, ErrorStr);
                UGPB_QueryProduct::StaticInstance->SetReadyToDestroy();
            }
        });
    }
}
#endif 