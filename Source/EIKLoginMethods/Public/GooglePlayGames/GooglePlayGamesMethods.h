// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

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
DECLARE_JAVA_METHOD(AndroidThunkJava_GPGS_getIsSignedIn);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPGS_getPlayer);
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