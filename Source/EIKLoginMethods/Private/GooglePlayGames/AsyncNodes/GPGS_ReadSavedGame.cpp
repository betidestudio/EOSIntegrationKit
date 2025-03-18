#include "GooglePlayGames/AsyncNodes/GPGS_ReadSavedGame.h"
#include "GooglePlayGames/GooglePlayGamesMethods.h"

#if GPGS_SUPPORTED
#include "Android/Utils/AndroidJNIConvertor.h"
#endif

TWeakObjectPtr<UGPGS_ReadSavedGame> UGPGS_ReadSavedGame::StaticInstance;

UGPGS_ReadSavedGame* UGPGS_ReadSavedGame::ReadSavedGame(UObject* WorldContextObject, const FString& ID)
{
	UGPGS_ReadSavedGame* Node = NewObject<UGPGS_ReadSavedGame>();
	Node->Var_ID = ID;
	return Node;
}

void UGPGS_ReadSavedGame::Activate()
{
	Super::Activate();

	UGPGS_ReadSavedGame::StaticInstance = this;

#if GPGS_SUPPORTED
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_readSavedGame, "(Ljava/lang/String;)V");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPGS_readSavedGame, AndroidJNIConvertor::GetJavaString(Var_ID));
	}
#else
	TArray<uint8> Data;
	Failure.Broadcast("", Data, "ERROR: Google Play Games Not Supported!");
	SetReadyToDestroy();
#endif
}

void UGPGS_ReadSavedGame::BeginDestroy()
{
	UGPGS_ReadSavedGame::StaticInstance = nullptr;
	Super::BeginDestroy();
}

#if GPGS_SUPPORTED
extern "C"
{
	JNIEXPORT void Java_com_epicgames_unreal_GameActivity_nativeOnReadSavedGameSuccess(JNIEnv* env, jclass clazz, jstring id, jbyteArray data)
	{
		FString ID = AndroidJNIConvertor::FromJavaString(id);
		TArray<uint8> Data = AndroidJNIConvertor::ConvertToByteArray(data);
		if (UGPGS_ReadSavedGame::StaticInstance.Get())
		{
			AsyncTask(ENamedThreads::GameThread, [ID, Data]() {
				UGPGS_ReadSavedGame::StaticInstance->Success.Broadcast(ID, Data, "");
				UGPGS_ReadSavedGame::StaticInstance->SetReadyToDestroy();
			});
		}
	}
	JNIEXPORT void Java_com_epicgames_unreal_GameActivity_nativeOnReadSavedGameFailure(JNIEnv* env, jclass clazz, jstring id, jstring error)
	{
		FString ID = AndroidJNIConvertor::FromJavaString(id);
		FString Error = AndroidJNIConvertor::FromJavaString(error);
		if (UGPGS_ReadSavedGame::StaticInstance.Get())
		{
			AsyncTask(ENamedThreads::GameThread, [ID, Error]() {
				TArray<uint8> Data;
				UGPGS_ReadSavedGame::StaticInstance->Failure.Broadcast(ID, Data, Error);
				UGPGS_ReadSavedGame::StaticInstance->SetReadyToDestroy();
			});
		}
	}
}
#endif
