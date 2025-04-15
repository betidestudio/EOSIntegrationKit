#include "GooglePlayGames/AsyncNodes/GPGS_ShowSavedGamesUI.h"
#include "GooglePlayGames/GooglePlayGamesMethods.h"

#if GPGS_SUPPORTED
#include "Android/Utils/AndroidJNIConvertor.h"
#endif

TWeakObjectPtr<UGPGS_ShowSavedGamesUI> UGPGS_ShowSavedGamesUI::StaticInstance;

UGPGS_ShowSavedGamesUI* UGPGS_ShowSavedGamesUI::ShowSavedGamesUI(UObject* WorldContextObject, int Max)
{
	UGPGS_ShowSavedGamesUI* Node = NewObject<UGPGS_ShowSavedGamesUI>();
	Node->Var_Max = Max;
	return Node;
}

void UGPGS_ShowSavedGamesUI::Activate()
{
	Super::Activate();

	UGPGS_ShowSavedGamesUI::StaticInstance = this;

#if GPGS_SUPPORTED
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_showSavedGamesUI, "(I)V");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPGS_showSavedGamesUI, Var_Max);
	}
#else
	Failure.Broadcast("", "ERROR: Google Play Games Not Supported!");
	SetReadyToDestroy();
#endif
}

void UGPGS_ShowSavedGamesUI::BeginDestroy()
{
	UGPGS_ShowSavedGamesUI::StaticInstance = nullptr;
	Super::BeginDestroy();
}

#if GPGS_SUPPORTED
extern "C"
{
	JNIEXPORT void Java_com_epicgames_unreal_GameActivity_nativeOnSavedGameSelected(JNIEnv* env, jclass clazz, jstring id)
	{
		FString ID = AndroidJNIConvertor::FromJavaString(id);
		if (UGPGS_ShowSavedGamesUI::StaticInstance.Get())
		{
			AsyncTask(ENamedThreads::GameThread, [ID]() {
				UGPGS_ShowSavedGamesUI::StaticInstance->Selected.Broadcast(ID, "");
				UGPGS_ShowSavedGamesUI::StaticInstance->SetReadyToDestroy();
			});
		}
	}
	JNIEXPORT void Java_com_epicgames_unreal_GameActivity_nativeOnNewSavedGameRequest(JNIEnv* env, jclass clazz)
	{
		if (UGPGS_ShowSavedGamesUI::StaticInstance.Get())
		{
			AsyncTask(ENamedThreads::GameThread, []() {
				UGPGS_ShowSavedGamesUI::StaticInstance->CreateNew.Broadcast("", "");
				UGPGS_ShowSavedGamesUI::StaticInstance->SetReadyToDestroy();
			});
		}
	}
	JNIEXPORT void Java_com_epicgames_unreal_GameActivity_nativeOnSavedGamesUIBackClicked(JNIEnv* env, jclass clazz)
	{
		if (UGPGS_ShowSavedGamesUI::StaticInstance.Get())
		{
			AsyncTask(ENamedThreads::GameThread, []() {
				UGPGS_ShowSavedGamesUI::StaticInstance->ClickedBack.Broadcast("", "");
				UGPGS_ShowSavedGamesUI::StaticInstance->SetReadyToDestroy();
			});
		}
	}
	JNIEXPORT void Java_com_epicgames_unreal_GameActivity_nativeOnSavedGamesUIFailure(JNIEnv* env, jclass clazz, jstring error)
	{
		FString Error = AndroidJNIConvertor::FromJavaString(error);
		if (UGPGS_ShowSavedGamesUI::StaticInstance.Get())
		{
			AsyncTask(ENamedThreads::GameThread, [Error]() {
				UGPGS_ShowSavedGamesUI::StaticInstance->Failure.Broadcast("", Error);
				UGPGS_ShowSavedGamesUI::StaticInstance->SetReadyToDestroy();
			});
		}
	}
}
#endif
