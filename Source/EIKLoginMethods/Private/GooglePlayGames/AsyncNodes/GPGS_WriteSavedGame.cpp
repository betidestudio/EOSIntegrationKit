#include "GooglePlayGames/AsyncNodes/GPGS_WriteSavedGame.h"
#include "GooglePlayGames/GooglePlayGamesMethods.h"

#if GPGS_SUPPORTED
#include "Android/Utils/AndroidJNIConvertor.h"
#endif

TWeakObjectPtr<UGPGS_WriteSavedGame> UGPGS_WriteSavedGame::StaticInstance;

UGPGS_WriteSavedGame* UGPGS_WriteSavedGame::WriteSavedGame(UObject* WorldContextObject, const FString& ID, const TArray<uint8>& Data, const FString& Description)
{
	UGPGS_WriteSavedGame* Node = NewObject<UGPGS_WriteSavedGame>();
	Node->Var_ID = ID;
	Node->Var_Data = Data;
	Node->Var_Description = Description;
	return Node;
}

void UGPGS_WriteSavedGame::Activate()
{
	Super::Activate();

	UGPGS_WriteSavedGame::StaticInstance = this;

#if GPGS_SUPPORTED
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_writeSavedGame, "(Ljava/lang/String;Ljava/lang/String;[B)V");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPGS_writeSavedGame, AndroidJNIConvertor::GetJavaString(Var_ID), AndroidJNIConvertor::GetJavaString(Var_Description), AndroidJNIConvertor::ConvertToJByteArray(Var_Data));
	}
#else
	Failure.Broadcast("", "ERROR: Google Play Games Not Supported!");
	SetReadyToDestroy();
#endif
}

void UGPGS_WriteSavedGame::BeginDestroy()
{
	UGPGS_WriteSavedGame::StaticInstance = nullptr;
	Super::BeginDestroy();
}

#if GPGS_SUPPORTED
extern "C"
{
	JNIEXPORT void Java_com_epicgames_unreal_GameActivity_nativeOnWriteSavedGameSuccess(JNIEnv* env, jclass clazz, jstring id)
	{
		FString ID = AndroidJNIConvertor::FromJavaString(id);
		if (UGPGS_WriteSavedGame::StaticInstance.Get())
		{
			AsyncTask(ENamedThreads::GameThread, [ID]() {
				UGPGS_WriteSavedGame::StaticInstance->Success.Broadcast(ID, "");
				UGPGS_WriteSavedGame::StaticInstance->SetReadyToDestroy();
			});
		}
	}
	JNIEXPORT void Java_com_epicgames_unreal_GameActivity_nativeOnWriteSavedGameFailure(JNIEnv* env, jclass clazz, jstring id, jstring error)
	{
		FString ID = AndroidJNIConvertor::FromJavaString(id);
		FString Error = AndroidJNIConvertor::FromJavaString(error);
		if (UGPGS_WriteSavedGame::StaticInstance.Get())
		{
			AsyncTask(ENamedThreads::GameThread, [ID, Error]() {
				UGPGS_WriteSavedGame::StaticInstance->Failure.Broadcast(ID, Error);
				UGPGS_WriteSavedGame::StaticInstance->SetReadyToDestroy();
			});
		}
	}
}
#endif
