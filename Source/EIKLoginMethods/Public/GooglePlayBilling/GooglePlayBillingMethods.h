// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

// Global configurable timeout (in seconds) for Google Play Billing async nodes
static float GPB_AsyncTimeoutSeconds = 30.0f;

#define GPB_SUPPORTED (PLATFORM_ANDROID && GOOGLE_PLAY_BILLING_ENABLED)

#if GPB_SUPPORTED
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
DECLARE_JAVA_METHOD(AndroidThunkJava_GPBL_queryProduct);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPBL_queryPurchases);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPBL_launchPurchaseFlow);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPBL_consumePurchase);
DECLARE_JAVA_METHOD(AndroidThunkJava_GPBL_acknowledgePurchase);
#endif