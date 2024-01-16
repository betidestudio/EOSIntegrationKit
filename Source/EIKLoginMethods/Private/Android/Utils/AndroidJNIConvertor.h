// Copyright (c) 2018 Nineva Studios

#pragma once

#include "Android/AndroidJava.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"

class AndroidJNIConvertor
{
public:

#if PLATFORM_ANDROID
	static jobjectArray ConvertToJStringArray(const TArray<FString>& stringArray);

	static jbooleanArray ConvertToJBooleanArray(const TArray<bool>& boolArray);

	static jintArray ConvertToJIntArray(const TArray<int>& intArray);

	static jbyteArray ConvertToJByteArray(const TArray<uint8>& byteArray);

	static jlongArray ConvertToJLongArray(const TArray<long>& longArray);

	static TArray<uint8> ConvertToByteArray(jbyteArray javaArray);

	static TArray<long> ConvertToLongArray(jlongArray javaArray);

	static TArray<FString> ConvertToStringArray(jobjectArray javaStringArray);

	static jstring GetJavaString(FString string);

	static FString FromJavaString(jstring javaString);
#endif // PLATFORM_ANDROID

};