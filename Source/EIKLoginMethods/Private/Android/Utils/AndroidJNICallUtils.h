// Copyright (c) 2018 Nineva Studios

#pragma once

#include "Android/AndroidJava.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"

class AndroidJNICallUtils
{
public:

#if PLATFORM_ANDROID
	static FString CallStaticStringMethod(const ANSICHAR* ClassName, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...);

	static bool CallStaticBoolMethod(const ANSICHAR* ClassName, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...);

	static int CallStaticIntMethod(const ANSICHAR* ClassName, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...);

	static long CallStaticLongMethod(const ANSICHAR* ClassName, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...);

	static jobject CallStaticObjectMethod(const ANSICHAR* ClassName, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...);

	static void CallStaticVoidMethod(const ANSICHAR* ClassName, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...);

	static FString CallStringMethod(jobject object, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...);

	static bool CallBoolMethod(jobject object, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...);

	static int CallIntMethod(jobject object, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...);

	static jobject CallObjectMethod(jobject object, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...);

	static void CallVoidMethod(const ANSICHAR* className, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...);

	static void CallGameActivityVoidMethod(const ANSICHAR* className, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...);

	static long CallLongMethod(jobject object, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...);
#endif // PLATFORM_ANDROID

};
