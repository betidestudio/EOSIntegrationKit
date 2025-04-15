#include "GooglePlayGames/AsyncNodes/GPGS_GetPlayerStats.h"
#include "GooglePlayGames/GooglePlayGamesMethods.h"

#if GPGS_SUPPORTED
#include "Android/Utils/AndroidJNIConvertor.h"
#endif

TWeakObjectPtr<UGPGS_GetPlayerStats> UGPGS_GetPlayerStats::StaticInstance;

UGPGS_GetPlayerStats* UGPGS_GetPlayerStats::GetPlayerStats(UObject* WorldContextObject)
{
	UGPGS_GetPlayerStats* Node = NewObject<UGPGS_GetPlayerStats>();
	return Node;
}

void UGPGS_GetPlayerStats::Activate()
{
	Super::Activate();

	UGPGS_GetPlayerStats::StaticInstance = this;

#if GPGS_SUPPORTED
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_getPlayerStats, "()V");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPGS_getPlayerStats);
	}
#else
	Failure.Broadcast(FGPGS_PlayerStats(), "ERROR: Google Play Games Not Supported!");
	SetReadyToDestroy();
#endif
}

void UGPGS_GetPlayerStats::BeginDestroy()
{
	UGPGS_GetPlayerStats::StaticInstance = nullptr;
	Super::BeginDestroy();
}

#if GPGS_SUPPORTED
extern "C"
{
	JNIEXPORT void Java_com_epicgames_unreal_GameActivity_nativeOnPlayerStatsSuccess(JNIEnv* env, jclass clazz, jstring jsonStr)
	{
		FString JsonStr = AndroidJNIConvertor::FromJavaString(jsonStr);
		if (UGPGS_GetPlayerStats::StaticInstance.Get())
		{
			AsyncTask(ENamedThreads::GameThread, [JsonStr]() {
				UGPGS_GetPlayerStats::StaticInstance->Success.Broadcast(FGPGS_PlayerStats::ParseFromJson(JsonStr), "");
				UGPGS_GetPlayerStats::StaticInstance->SetReadyToDestroy();
			});
		}
	}
	JNIEXPORT void Java_com_epicgames_unreal_GameActivity_nativeOnPlayerStatsFailure(JNIEnv* env, jclass clazz, jstring error)
	{
		FString Error = AndroidJNIConvertor::FromJavaString(error);
		if (UGPGS_GetPlayerStats::StaticInstance.Get())
		{
			AsyncTask(ENamedThreads::GameThread, [Error]() {
				UGPGS_GetPlayerStats::StaticInstance->Failure.Broadcast(FGPGS_PlayerStats(), Error);
				UGPGS_GetPlayerStats::StaticInstance->SetReadyToDestroy();
			});
		}
	}
}
#endif
