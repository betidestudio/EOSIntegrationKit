// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "GoogleLogin_SLK.h"
#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h" 
#include "Android/Utils/AndroidJNICallUtils.h"
#include "Android/Utils/AndroidJNIConvertor.h"
#include "Runtime/Core/Public/Async/TaskGraphInterfaces.h"
#endif
TWeakObjectPtr<UGoogleLogin_SLK> UGoogleLogin_SLK::staticInstance = nullptr;
static const ANSICHAR* UnrealInterface = "com/example/googleonetap/UnrealInterface";
static const ANSICHAR* GameActivity = "com/epicgames/unreal/GameActivity";


UGoogleLogin_SLK* UGoogleLogin_SLK::GoogleLogin(UObject* WorldContextObject, const FString& ClientID)
{
	UGoogleLogin_SLK* Action = NewObject<UGoogleLogin_SLK>();
	Action->Var_ClientID = ClientID;
	return Action;
}

void UGoogleLogin_SLK::Activate()
{
	UGoogleLogin_SLK::staticInstance = this;
	GoogleLoginLocal();
	Super::Activate();
}

void UGoogleLogin_SLK::BeginDestroy()
{
	UGoogleLogin_SLK::staticInstance = nullptr;
	Super::BeginDestroy();
}

void UGoogleLogin_SLK::GoogleLoginLocal()
{
#if PLATFORM_ANDROID
	//AndroidJNICallUtils::CallStaticVoidMethod(UnrealInterface, "InitInterface", "(Landroid/app/Activity;Ljava/lang/String;)V", FJavaWrapper::GameActivityThis, AndroidJNIConvertor::GetJavaString(ClientID));
	AndroidJNICallUtils::CallGameActivityVoidMethod(GameActivity,"AndroidThunkJava_GoogleSubsystem_Init", "(Ljava/lang/String;)V", AndroidJNIConvertor::GetJavaString(Var_ClientID));
	AndroidJNICallUtils::CallStaticVoidMethod(UnrealInterface, "SignIn", "()V");
	return;
#endif
	UGoogleLogin_SLK::staticInstance.Get()->Failure.Broadcast("", "Google Login Failed to initialise");
}

#if PLATFORM_ANDROID
JNI_METHOD void Java_com_example_googleonetap_UnrealInterface_OnGoogleCallback(JNIEnv* env, jclass clazz, jint status, jstring email, jstring username, jstring idToken, jstring error)
{
	UE_LOG(LogTemp, Warning, TEXT("OnGoogleCallback"));
	int32 lstatus = static_cast<int32>(status);
	FString lemail = AndroidJNIConvertor::FromJavaString(email);
	FString lusername = AndroidJNIConvertor::FromJavaString(username);
	FString lidToken = AndroidJNIConvertor::FromJavaString(idToken);
	FString lerror = AndroidJNIConvertor::FromJavaString(error);
	if (UGoogleLogin_SLK::staticInstance.Get())
	{
		FGraphEventRef task = FFunctionGraphTask::CreateAndDispatchWhenReady([lstatus, lemail, lusername, lidToken, lerror]
			{
			if(lstatus== 0)
			{
				UGoogleLogin_SLK::staticInstance.Get()->Failure.Broadcast("",lerror);
			}
			else
			{
				UGoogleLogin_SLK::staticInstance.Get()->Success.Broadcast(lidToken, "");
			}
				

			}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}
#endif
