#include "GooglePlayGames/AsyncNodes/GPGS_LoadEvent.h"
#include "GooglePlayGames/GooglePlayGamesMethods.h"

#if GPGS_SUPPORTED
#include "Android/Utils/AndroidJNIConvertor.h"
#endif

TWeakObjectPtr<UGPGS_LoadEvent> UGPGS_LoadEvent::StaticInstance;

UGPGS_LoadEvent* UGPGS_LoadEvent::LoadEvent(UObject* WorldContextObject, const FString& ID)
{
	UGPGS_LoadEvent* Node = NewObject<UGPGS_LoadEvent>();
	Node->Var_ID = ID;
	return Node;
}

void UGPGS_LoadEvent::Activate()
{
	Super::Activate();

	UGPGS_LoadEvent::StaticInstance = this;

#if GPGS_SUPPORTED
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_loadEvent, "(Ljava/lang/String;)V");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPGS_loadEvent, AndroidJNIConvertor::GetJavaString(Var_ID));
	}
#else
	Failure.Broadcast("", FGPGS_Event(), "ERROR: Google Play Games Not Supported!");
	SetReadyToDestroy();
#endif
}

void UGPGS_LoadEvent::BeginDestroy()
{
	UGPGS_LoadEvent::StaticInstance = nullptr;
	Super::BeginDestroy();
}

#if GPGS_SUPPORTED
extern "C"
{
	JNIEXPORT void Java_com_epicgames_unreal_GameActivity_nativeOnEventLoadSuccess(JNIEnv* env, jclass clazz, jstring jsonStr)
	{
		FString JsonStr = AndroidJNIConvertor::FromJavaString(jsonStr);
		if (UGPGS_LoadEvent::StaticInstance.Get())
		{
			AsyncTask(ENamedThreads::GameThread, [JsonStr]() {
				FGPGS_Event e = FGPGS_Event::ParseFromJson(JsonStr);
				UGPGS_LoadEvent::StaticInstance->Success.Broadcast(e.EventID, e, "");
				UGPGS_LoadEvent::StaticInstance->SetReadyToDestroy();
			});
		}
	}
	JNIEXPORT void Java_com_epicgames_unreal_GameActivity_nativeOnEventLoadFailure(JNIEnv* env, jclass clazz, jstring id, jstring error)
	{
		FString ID = AndroidJNIConvertor::FromJavaString(id);
		FString Error = AndroidJNIConvertor::FromJavaString(error);
		if (UGPGS_LoadEvent::StaticInstance.Get())
		{
			AsyncTask(ENamedThreads::GameThread, [ID, Error]() {
				UGPGS_LoadEvent::StaticInstance->Failure.Broadcast(ID, FGPGS_Event(), Error);
				UGPGS_LoadEvent::StaticInstance->SetReadyToDestroy();
			});
		}
	}
}
#endif
