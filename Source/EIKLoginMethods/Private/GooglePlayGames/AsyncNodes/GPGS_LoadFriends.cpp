#include "GooglePlayGames/AsyncNodes/GPGS_LoadFriends.h"
#include "GooglePlayGames/GooglePlayGamesMethods.h"

#if GPGS_SUPPORTED
#include "Android/Utils/AndroidJNIConvertor.h"
#endif

TWeakObjectPtr<UGPGS_LoadFriends> UGPGS_LoadFriends::StaticInstance;

UGPGS_LoadFriends* UGPGS_LoadFriends::LoadFriends(UObject* WorldContextObject, int Max, bool bForceReload)
{
	UGPGS_LoadFriends* Node = NewObject<UGPGS_LoadFriends>();
	Node->Var_Max = Max;
	Node->Var_bForceReload = bForceReload;
	return Node;
}

void UGPGS_LoadFriends::Activate()
{
	Super::Activate();

	UGPGS_LoadFriends::StaticInstance = this;

#if GPGS_SUPPORTED
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_loadFriends, "(IZ)V");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPGS_loadFriends, Var_Max, Var_bForceReload);
	}
#else
	TArray<FGPGS_Player> Friends;
	Failure.Broadcast(Friends, "ERROR: Google Play Games Not Supported!");
	SetReadyToDestroy();
#endif
}

void UGPGS_LoadFriends::BeginDestroy()
{
	UGPGS_LoadFriends::StaticInstance = nullptr;
	Super::BeginDestroy();
}

void UGPGS_LoadFriends::HandleCallback(bool bSuccess, const FString& FriendsJson, const FString& Error)
{
	if (bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Parsing: %s"), *FriendsJson);
		Success.Broadcast(FGPGS_Player::ParseFriendArrayFromJson(FriendsJson), Error);
	}
	else
	{
		TArray<FGPGS_Player> Friends;
		Failure.Broadcast(Friends, Error);
	}
	SetReadyToDestroy();
}

#if GPGS_SUPPORTED
extern "C"
{
	JNIEXPORT void Java_com_epicgames_unreal_GameActivity_nativeOnFriendsLoaded(JNIEnv* env, jclass clazz, jboolean success, jstring friendsArrayJson, jstring error)
	{
		FString FriendsJson = AndroidJNIConvertor::FromJavaString(friendsArrayJson);
		FString errorStr = AndroidJNIConvertor::FromJavaString(error);
		if (UGPGS_LoadFriends::StaticInstance.Get())
		{
			AsyncTask(ENamedThreads::GameThread, [success, FriendsJson, errorStr]() {
				UGPGS_LoadFriends::StaticInstance->HandleCallback(success, FriendsJson, errorStr);
			});
		}
	}
}
#endif
