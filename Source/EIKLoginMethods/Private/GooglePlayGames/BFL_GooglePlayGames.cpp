#include "GooglePlayGames/BFL_GooglePlayGames.h"
#include "GooglePlayGames/GooglePlayGamesMethods.h"

UBFL_GooglePlayGames::UBFL_GooglePlayGames()
{
#if GPGS_SUPPORTED
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_manualSignIn, "()V");
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_getIsSignedIn, "()Z");
	INIT_JAVA_METHOD(AndroidThunkJava_GPGS_getPlayer, "()Ljava/lang/String;");
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

bool UBFL_GooglePlayGames::IsSignedIn()
{
#if GPGS_SUPPORTED
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		return FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPGS_getIsSignedIn);
	}
#endif
	return false;
}

FGPGS_Player UBFL_GooglePlayGames::GetPlayer()
{
#if GPGS_SUPPORTED
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		return FGPGS_Player::ParseFromJson(AndroidJNIConvertor::FromJavaString((jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPGS_getPlayer)));
	}
#endif
	return FGPGS_Player();
}

void UBFL_GooglePlayGames::UnlockAchievement(const FString& AchievementID)
{
#if GPGS_SUPPORTED
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
    {
        jstring JavaAchievementID = Env->NewStringUTF(TCHAR_TO_UTF8(*AchievementID));
        FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPGS_unlockAchievement, JavaAchievementID);
        Env->DeleteLocalRef(JavaAchievementID);
    }
#endif
}

void UBFL_GooglePlayGames::IncrementAchievement(const FString& AchievementID, int32 Value)
{
#if GPGS_SUPPORTED
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
    {
        jstring JavaAchievementID = Env->NewStringUTF(TCHAR_TO_UTF8(*AchievementID));
        FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPGS_incrementAchievement, JavaAchievementID, Value);
        Env->DeleteLocalRef(JavaAchievementID);
    }
#endif
}

void UBFL_GooglePlayGames::DisplayAchievementsUI()
{
#if GPGS_SUPPORTED
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
    {
        FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPGS_displayAppAchievementsUI);
    }
#endif
}

void UBFL_GooglePlayGames::SubmitLeaderboardScore(const FString& LeaderboardID, int64 Value)
{
#if GPGS_SUPPORTED
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
    {
        jstring JavaLeaderboardID = Env->NewStringUTF(TCHAR_TO_UTF8(*LeaderboardID));
        FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPGS_submitLeaderboardScore, JavaLeaderboardID, (jlong)Value);
        Env->DeleteLocalRef(JavaLeaderboardID);
    }
#endif
}

void UBFL_GooglePlayGames::ShowLeaderboard(const FString& LeaderboardID)
{
#if GPGS_SUPPORTED
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
    {
        jstring JavaLeaderboardID = Env->NewStringUTF(TCHAR_TO_UTF8(*LeaderboardID));
        FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPGS_showLeaderboard, JavaLeaderboardID);
        Env->DeleteLocalRef(JavaLeaderboardID);
    }
#endif
}

void UBFL_GooglePlayGames::ComparePlayerProfile(const FString& PlayerID)
{
#if GPGS_SUPPORTED
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
    {
        jstring JavaPlayerID = Env->NewStringUTF(TCHAR_TO_UTF8(*PlayerID));
        FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPGS_comparePlayerProfile, JavaPlayerID);
        Env->DeleteLocalRef(JavaPlayerID);
    }
#endif
}

void UBFL_GooglePlayGames::SubmitEvent(const FString& EventID, int32 NumberOfOccurrences)
{
#if GPGS_SUPPORTED
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
    {
        jstring JavaEventID = Env->NewStringUTF(TCHAR_TO_UTF8(*EventID));
        FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_GPGS_submitEvent, JavaEventID, NumberOfOccurrences);
        Env->DeleteLocalRef(JavaEventID);
    }
#endif
}
