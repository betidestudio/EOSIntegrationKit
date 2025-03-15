#include "GooglePlayGames/BFL_GooglePlayGames.h"

#define GPGS_SUPPORTED (PLATFORM_ANDROID && GOOGLE_PLAYGAMES_ENABLED)

#if GPGS_SUPPORTED
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"

#include "Android/Utils/AndroidJNIConvertor.h"

#define INIT_JAVA_METHOD(name, signature) \
if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true)) { \
name = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, #name, signature, false); \
check(name != NULL); \
} else { \
check(0); \
}

#define DECLARE_JAVA_METHOD(name) \
static jmethodID name = NULL;

// ---- Methods ----
DECLARE_JAVA_METHOD(AndroidThunkJava_GPGS_manualSignIn);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPGS_getUsername);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPGS_getPlayerID);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPGS_unlockAchievement);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPGS_incrementAchievement);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPGS_displayAppAchievementsUI);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPGS_submitLeaderboardScore);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPGS_showLeaderboard);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPGS_loadFriends);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPGS_comparePlayerProfile);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPGS_showSavedGamesUI);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPGS_writeSavedGame);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPGS_readSavedGame);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPGS_submitEvent);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPGS_loadEvent);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPGS_getPlayerStats);
#endif

UBFL_GooglePlayGames::UBFL_GooglePlayGames()
{
#if GPGS_SUPPORTED
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_manualSignIn, "()V");
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_getUsername, "()Ljava/lang/String;");
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_getPlayerID,  "()Ljava/lang/String;");
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_unlockAchievement, "(Ljava/lang/String;)V");
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_incrementAchievement, "(Ljava/lang/String;I)V");
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_displayAppAchievementsUI, "()V");
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_submitLeaderboardScore, "(Ljava/lang/String;J)V");
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_showLeaderboard, "(Ljava/lang/String;)V");
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_loadFriends, "(IZ)V");
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_comparePlayerProfile, "(Ljava/lang/String;)V");
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_showSavedGamesUI, "(I)V");
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_writeSavedGame, "(Ljava/lang/String;Ljava/lang/String;B)V");
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_readSavedGame, "(Ljava/lang/String;)V");
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_submitEvent, "(Ljava/lang/String;I)V");
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_loadEvent, "(Ljava/lang/String;)V");
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_getPlayerStats, "()V");
#endif
}

void UBFL_GooglePlayGames::ManualSignIn()
{
#if GPGS_SUPPORTED
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPGS_manualSignIn);
	}
#endif
}

FString UBFL_GooglePlayGames::GetUsername()
{
#if GPGS_SUPPORTED
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		return AndroidJNIConvertor::FromJavaString(static_cast<jstring>(FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPGS_getUsername)));
	}
#endif
	return "Google Play Games Services Not Supported!";
}

FString UBFL_GooglePlayGames::GetPlayerID()
{
#if GPGS_SUPPORTED
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		return AndroidJNIConvertor::FromJavaString(static_cast<jstring>(FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPGS_getPlayerID)));
	}
#endif
	return "Google Play Games Services Not Supported!";
}
